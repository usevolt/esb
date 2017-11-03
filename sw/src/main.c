/*
===============================================================================
 Name        : main.c
 Author      : Olli Usenius
 Version     :
 Copyright   : $(copyright)
 Description :
===============================================================================
*/

#include "main.h"
#include "messages.h"
#include "pin_mappings.h"
#include "can_fsb.h"

#include <uv_utilities.h>
#include <uv_rtos.h>

dev_st dev = {};
#define this ((dev_st*) &dev)


bool adc_get_temp(uv_adc_channels_e adc_chn, int16_t *dest);
bool adc_get_level(uv_adc_channels_e adc_chn, int16_t *dest);
uint16_t adc_get_voltage_mv(const uv_adc_channels_e adc_chn);


#define GET_MOTOR_WATER()	(!uv_gpio_get(MOTOR_WATER_TEMP_I))
#define GET_MOTOR_OIL_PRESS() (!uv_gpio_get(MOTOR_OIL_PRESS_I))


void init(dev_st* me) {

	// initialize outputs
	uv_output_init(&this->glow, GLOW_SENSE_AIN, GLOW_PLUGS_O, OUTPUT_2_MOHM,
			0, 25000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_GLOW_OVERLOAD, ESB_EMCY_GLOW_FAULT);
	uv_output_init(&this->starter, STARTER_SENSE_AIN, STARTER_O, OUTPUT_2_MOHM,
			0, 2000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_STARTER_OVERLOAD, ESB_EMCY_STARTER_FAULT);
	uv_output_init(&this->ac, AC_SENSE_AIN, AC_O, OUTPUT_2_MOHM,
			0, 2000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_AC_OVERLOAD, ESB_EMCY_AC_FAULT);
	uv_output_init(&this->engine_start1, ENGINE_START1_SENSE_AIN, ENGINE_START_1_O, OUTPUT_2_MOHM,
			0, 20000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_STOP_OVERLOAD, ESB_EMCY_ENGINE_STOP_FAULT);
	uv_output_init(&this->engine_start2, ENGINE_START2_SENSE_AIN, ENGINE_START_2_O, OUTPUT_2_MOHM,
			0, 20000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_STOP_OVERLOAD, ESB_EMCY_ENGINE_STOP_FAULT);
	uv_output_init(&this->pump, PUMP_SENSE_AIN, PUMP_O, OUTPUT_15_MOHM,
			0, 3000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_PUMP_OVERLOAD, ESB_EMCY_PUMP_FAULT);
	uv_output_init(&this->alt_ig, ALT_IG_SENSE_AIN, ALT_IG_O, OUTPUT_2_MOHM,
			0, 2000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ALT_IG_OVERLOAD, ESB_EMCY_ALT_IG_FAULT);

	// initialize inputs
	UV_GPIO_INIT_INPUT(ALT_L_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(ALT_P_RPM_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(MOTOR_WATER_TEMP_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(MOTOR_OIL_PRESS_I, PULL_UP_ENABLED);

	// motor temp
	sensor_init(&this->motor_temp, MOTOR_TEMP_AIN, MOTOR_TEMP_AVG_COUNT,
			ESB_EMCY_MTEMP_SENSOR_FAULT, &adc_get_temp);
	sensor_set_warning(&this->motor_temp, MOTOR_TEMP_WARN_VALUE,
			MOTOR_TEMP_HYSTERESIS, false, ESB_EMCY_MTEMP_WARNING);
	sensor_set_error(&this->motor_temp, MOTOR_TEMP_ERR_VALUE,
			MOTOR_TEMP_HYSTERESIS, false, ESB_EMCY_MTEMP_ERROR);

	// oil temp
	sensor_init(&this->oil_temp, OIL_TEMP_AIN, OIL_TEMP_AVG_COUNT,
			ESB_EMCY_OTEMP_SENSOR_FAULT, &adc_get_temp);
	sensor_set_warning(&this->oil_temp, OIL_TEMP_WARN_VALUE,
			OIL_TEMP_HYSTERESIS, false, ESB_EMCY_OTEMP_WARNING);
	sensor_set_error(&this->oil_temp, OIL_TEMP_ERR_VALUE,
			OIL_TEMP_HYSTERESIS, false, ESB_EMCY_OTEMP_ERROR);

	// fuel level
	sensor_init(&this->fuel_level, FUEL_LEVEL_AIN, FUEL_LEVEL_AVG_COUNT,
			ESB_EMCY_FLEVEL_FAULT, &adc_get_level);
	sensor_set_warning(&this->fuel_level, FUEL_LEVEL_WARN_VALUE,
			FUEL_LEVEL_HYSTERESIS, true, ESB_EMCY_FLEVEL_WARNING);
	sensor_set_error(&this->fuel_level, FUEL_LEVEL_ERR_VALUE,
			FUEL_LEVEL_HYSTERESIS, true, ESB_EMCY_FLEVEL_ERROR);

	// oil level
	sensor_init(&this->oil_level, OIL_LEVEL_AIN, OIL_LEVEL_AVG_COUNT,
			ESB_EMCY_OLEVEL_SENSOR_FAULT, &adc_get_level);
	sensor_set_warning(&this->oil_level, OIL_LEVEL_WARN_VALUE,
			OIL_LEVEL_HYSTERESIS, true, ESB_EMCY_OLEVEL_WARNING);
	sensor_set_error(&this->oil_level, OIL_LEVEL_ERR_VALUE,
			OIL_LEVEL_HYSTERESIS, true, ESB_EMCY_OLEVEL_ERROR);

	// vdd
	uv_moving_aver_init(&this->vdd_avg, VDD_AVG_COUNT);
	uv_hysteresis_init(&this->vdd_warning, VDD_WARN_VALUE_MV, VDD_WARN_HYSTERESIS_MV, true);
	this->vdd = 0;
	this->vdd_warn_req = true;

	this->motor_water_temp = 0;
	this->motor_oil_press = 0;
	this->alt_p_rpm = 0;
	uv_delay_init(MOTOR_DELAY_MS, &this->motor_delay);

	//init terminal and pass application terminal commands array as a parameter
	uv_terminal_init(terminal_commands, commands_size());

	// load non-volatile data
	if (uv_memory_load()) {

		uv_memory_save();
	}

	this->fsb.ignkey_state = FSB_IGNKEY_STATE_OFF;
	this->fsb.emcy = 0;

	uv_canopen_set_state(CANOPEN_OPERATIONAL);

}

/// @brief: PT100 resistor teaching values in milliohms
#define PT100_RES_0C_MOHM		100000
#define PT100_RES_100C_MOHM		139000
#define PT100_RES_NEG50C_MOHM	80500


/// @brief: Returns the ADC's read value as celsius degrees
bool adc_get_temp(uv_adc_channels_e adc_chn, int16_t *dest) {
	int32_t adc = uv_adc_read(adc_chn);
	bool ret = true;
	if ((adc == ADC_MAX_VALUE) || (adc == 0) || (!dest)) {
		ret = false;
	}
	else {
		int64_t mv = adc * 3300 / ADC_MAX_VALUE;
		if (mv >= 3300) {
			mv = 3300 - 1;
		}
		int64_t res_mohm = 10000000 * mv / (3300 - mv) / 50;


		int32_t t = uv_reli(res_mohm, PT100_RES_NEG50C_MOHM, PT100_RES_100C_MOHM);
		*dest = uv_lerpi(t, -50, 100);
	}
	return ret;
}

#define LEVEL_0_MV				0
#define LEVEL_100_MV			500

/// @brief: Reads the adc channel and converts it to liquid level percentage
bool adc_get_level(uv_adc_channels_e adc_chn, int16_t *dest) {
	bool ret = true;
	int64_t mv = adc_get_voltage_mv(adc_chn);
	if ((mv >= (LEVEL_100_MV * 2)) || (!dest)) {
		ret = false;
	}
	else {
		int32_t t = uv_reli(mv, LEVEL_0_MV, LEVEL_100_MV);
		*dest = uv_lerpi(t, 0, 100);
	}
	return ret;
}

uint16_t adc_get_voltage_mv(const uv_adc_channels_e adc_chn) {
	int32_t adc = uv_adc_read(adc_chn);
	return adc * 3300 / ADC_MAX_VALUE;
}



void step(void* me) {

	init(this);

	while (true) {
		unsigned int step_ms = 20;
		// update watchdog timer value to prevent a hard reset
		// uw_wdt_update();

		uv_output_step(&this->glow, step_ms);
		uv_output_step(&this->starter, step_ms);
		uv_output_step(&this->ac, step_ms);
		uv_output_step(&this->engine_start1, step_ms);
		uv_output_step(&this->engine_start2,step_ms);
		uv_output_step(&this->pump, step_ms);
		uv_output_step(&this->alt_ig, step_ms);

		// terminal step function
		uv_terminal_step();

		this->total_current = uv_output_get_current(&this->glow) +
				uv_output_get_current(&this->starter) +
				uv_output_get_current(&this->ac) +
				uv_output_get_current(&this->engine_start1) +
				uv_output_get_current(&this->engine_start2) +
				uv_output_get_current(&this->pump) +
				uv_output_get_current(&this->alt_ig);


		// motor temperature
//		todo: Temperature sensors commented until HW implements it
//		sensor_step(&this->motor_temp, step_ms);
//		sensor_step(&this->oil_temp, step_ms);
		sensor_step(&this->fuel_level, step_ms);
		sensor_step(&this->oil_level, step_ms);

		// kubota sensors
		this->motor_water_temp = GET_MOTOR_WATER();
		this->motor_oil_press = GET_MOTOR_OIL_PRESS();
		this->alt_l = !uv_gpio_get(ALT_L_I);

		// vdd voltage
		// note: Multiplier 11 comes from 10k/1k voltage divider resistors
		this->vdd = uv_moving_aver_step(&this->vdd_avg,
				adc_get_voltage_mv(VDD_SENSE_AIN) * 11);
		// warning when VDD is too low
		if (uv_hysteresis_step(&this->vdd_warning, this->vdd)) {
			if (this->vdd_warn_req) {
				uv_canopen_emcy_send(CANOPEN_EMCY_DEVICE_SPECIFIC, ESB_EMCY_VDD_LOW_WARNING);
				this->vdd_warn_req = false;
			}
		}
		else {
			this->vdd_warn_req = true;
		}

		// ignition key states
		if ((this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) ||
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_PREHEAT)) {
			if (uv_output_get_current(&this->engine_start1) < 200) {
				uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
			}
			// too big solenoid current might indicate a mismatch in solenoids
			else if (uv_output_get_current(&this->engine_start1) > 2000) {
				uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
			}
			else {

			}
			if (uv_output_get_current(&this->engine_start2) < 200) {
				uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
			}
			// too big solenoid current might indicate a mismatch in solenoids
			else if (uv_output_get_current(&this->engine_start2) > 2000) {
				uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
			}
			else {

			}
		}
		else if (this->fsb.ignkey_state == FSB_IGNKEY_STATE_START) {
			uv_output_set_state(&this->engine_start1, OUTPUT_STATE_ON);
			uv_output_set_state(&this->engine_start2, OUTPUT_STATE_ON);
		}
		else {
			uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
		}

		// motor sensor shut down
		if (this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) {
			if (this->motor_water_temp ||
					this->motor_oil_press) {
				if (uv_delay(step_ms, &this->motor_delay)) {
					uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
					uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
					// send EMCY message from motor protection
					uv_canopen_emcy_send(CANOPEN_EMCY_DEVICE_SPECIFIC,
							ESB_EMCY_ENGINE_PROTECTION_SHUTDOWN);
				}
			}
			else {
				uv_delay_init(MOTOR_DELAY_MS, &this->motor_delay);
			}
		}

//		// glow is active only when ignition key is in PREHEAT position
//		uv_output_set_state(&this->glow,
//				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_PREHEAT) ?
//						OUTPUT_STATE_ON : OUTPUT_STATE_OFF);
//
//		// starter is active only when ignition key is in START position
//		uv_output_set_state(&this->starter,
//				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_START) ?
//				OUTPUT_STATE_ON : OUTPUT_STATE_OFF);
//
//		// ac is controlled by CSB when ignition key is in ON position
//		uv_output_set_state(&this->ac,
//				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) ?
//				uv_output_get_state(&this->ac) : OUTPUT_STATE_OFF);

		// alternator ignition is on if engine is running and starter is not running
		uv_output_set_state(&this->alt_ig,
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) ?
				OUTPUT_STATE_ON : OUTPUT_STATE_OFF);


		// emcy
		if (this->fsb.emcy) {
			uv_output_set_state(&this->glow, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->starter, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->ac, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->pump, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->alt_ig, OUTPUT_STATE_OFF);
		}

		uv_rtos_task_delay(step_ms);
	}
}






int main(void) {

	// init the watchdog timer
//	uw_wdt_init(5);

	uv_init(&dev);

	uv_rtos_task_create(step, "step", UV_RTOS_MIN_STACK_SIZE * 5,
			&dev, UV_RTOS_IDLE_PRIORITY + 1, NULL);


	uv_rtos_start_scheduler();


    // Enter an infinite loop
	// never should end up here
    while(1) {
    }
    return 0;
}
