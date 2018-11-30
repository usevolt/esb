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
#include <uv_eeprom.h>
#include <string.h>

dev_st dev = {};
#define this ((dev_st*) &dev)


int16_t adc_get_temp(uv_adc_channels_e adc_chn);
int16_t adc_get_level(uv_adc_channels_e adc_chn);
uint16_t adc_get_voltage_mv(const uv_adc_channels_e adc_chn);
void rpm_callb(uv_gpios_e);
void sdo_callback(uint16_t mindex, uint8_t sindex);


#define GET_MOTOR_WATER()	(!uv_gpio_get(MOTOR_WATER_TEMP_I))
#define GET_MOTOR_OIL_PRESS() (!uv_gpio_get(MOTOR_OIL_PRESS_I))


#define VND5050_CURRENT_AMPL_UA		1619
#define VN5E01_CURRENT_AMPL_UA		5600

#define PWR_LIMIT_DIVIDER			50

void init(dev_st* me) {
	uv_gpio_interrupt_init(&rpm_callb);

	// load non-volatile data
	if (uv_memory_load()) {
		this->oilcooler_trigger_temp = OIL_TEMP_DEFAULT_TRIGGER_VALUE_C;
		uv_solenoid_output_conf_reset(&this->pump_conf);

		// initialize non-volatile memory to default settings
		this->starter_enabled = true;
		this->glow_enabled = true;
		this->ac_enabled = true;
		this->engine_start_enabled = true;
		this->alt_ig_enabled = true;
		this->oilcooler_enabled = true;
		this->pump_enabled = true;

		this->dither_ampl = 0;
		this->dither_freq = 50;
		this->engine_power_usage = ENGINE_POWER_USAGE_DEFAULT;
		this->pwr_rising_p = PWR_RISING_P_DEFAULT;
		uv_memory_save();
	}
	// initialize outputs
	uv_output_init(&this->glow, GLOW_SENSE_AIN,
			GLOW_PLUGS_O, VN5E01_CURRENT_AMPL_UA, 50000, 60000,
			OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_GLOW_OVERLOAD, ESB_EMCY_GLOW_FAULT);
	uv_output_init(&this->starter, STARTER_SENSE_AIN,
			STARTER_O, VN5E01_CURRENT_AMPL_UA, 30000, 45000,
			OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_STARTER_OVERLOAD, ESB_EMCY_STARTER_FAULT);
	uv_output_init(&this->ac, AC_SENSE_AIN,
			AC_O, VN5E01_CURRENT_AMPL_UA, 10000, 15000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_AC_OVERLOAD, ESB_EMCY_AC_FAULT);
	uv_output_init(&this->engine_start1, ENGINE_START1_SENSE_AIN,
			ENGINE_START_1_O, VN5E01_CURRENT_AMPL_UA, 30000, 40000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_STOP_OVERLOAD, ESB_EMCY_ENGINE_STOP_FAULT);
	uv_output_init(&this->engine_start2, ENGINE_START2_SENSE_AIN,
			ENGINE_START_2_O, VN5E01_CURRENT_AMPL_UA, 30000, 40000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_STOP_OVERLOAD, ESB_EMCY_ENGINE_STOP_FAULT);
	uv_output_init(&this->alt_ig, ALT_IG_SENSE_AIN,
			ALT_IG_O, VND5050_CURRENT_AMPL_UA, 2000, 5000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ALT_IG_OVERLOAD, ESB_EMCY_ALT_IG_FAULT);
	uv_output_init(&this->oilcooler, OILCOOLER_AIN, OILCOOLER_O,
			VN5E01_CURRENT_AMPL_UA, 15000, 30000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_OILCOOLER_OVERCURRENT, ESB_EMCY_OILCOOLER_FAULT);
	uv_solenoid_output_init(&this->pump, &this->pump_conf, PUMP_PWM, this->dither_freq,
			DUTY_CYCLEPPT(this->dither_ampl), PUMP_SENSE_AIN,
			VND5050_CURRENT_AMPL_UA, 3500, 5000,
			ESB_EMCY_PUMP_OVERLOAD, ESB_EMCY_PUMP_FAULT);
	uv_solenoid_output_get_conf(&this->pump)->max_ma = PUMP_CURRENT_MAX_MA;
	uv_solenoid_output_get_conf(&this->pump)->min_ma = PUMP_CURRENT_MIN_MA;

	// initialize inputs
	UV_GPIO_INIT_INPUT(ALT_L_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(ALT_P_RPM_I, PULL_UP_ENABLED);
	uv_gpio_init_int(ALT_P_RPM_I, INT_RISING_EDGE);
	UV_GPIO_INIT_INPUT(MOTOR_WATER_TEMP_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(MOTOR_OIL_PRESS_I, PULL_UP_ENABLED);

	uv_hysteresis_init(&this->oil_temp_hyst, this->oilcooler_trigger_temp,
			OIL_TEMP_HYSTERESIS_C, false);


	// motor temp
	uv_sensor_init(&this->motor_temp, MOTOR_TEMP_AIN, MOTOR_TEMP_AVG_COUNT, &adc_get_temp);
	uv_sensor_set_warning(&this->motor_temp, MOTOR_TEMP_WARN_VALUE,
			MOTOR_TEMP_HYSTERESIS, false, ESB_EMCY_MTEMP_WARNING);
	uv_sensor_set_fault(&this->motor_temp, TEMP_FAULT_MIN_VAL, TEMP_FAULT_MAX_VAL,
			MOTOR_TEMP_HYSTERESIS, ESB_EMCY_MTEMP_SENSOR_FAULT);
	this->motor_temp_value = 0;

	// oil temp
	uv_sensor_init(&this->oil_temp, OIL_TEMP_AIN, OIL_TEMP_AVG_COUNT, &adc_get_temp);
	uv_sensor_set_warning(&this->oil_temp, OIL_TEMP_WARN_VALUE,
			OIL_TEMP_HYSTERESIS, false, ESB_EMCY_OTEMP_WARNING);
	uv_sensor_set_fault(&this->oil_temp, TEMP_FAULT_MIN_VAL, TEMP_FAULT_MAX_VAL,
			OIL_TEMP_HYSTERESIS, ESB_EMCY_OTEMP_SENSOR_FAULT);
	this->oil_temp_value = 0;

	// oil level
	uv_sensor_init(&this->oil_level, OIL_LEVEL_AIN, OIL_LEVEL_AVG_COUNT, &adc_get_level);
	uv_sensor_set_warning(&this->oil_level, OIL_LEVEL_WARN_VALUE,
			OIL_LEVEL_HYSTERESIS, true, ESB_EMCY_OLEVEL_WARNING);
	uv_sensor_set_fault(&this->oil_level, LEVEL_FAULT_MIN_VAL, LEVEL_FAULT_MAX_VAL,
			OIL_LEVEL_HYSTERESIS, ESB_EMCY_OLEVEL_SENSOR_FAULT);
	this->oil_level_value = 0;

	// vdd
	uv_moving_aver_init(&this->vdd_avg, VDD_AVG_COUNT);
	uv_hysteresis_init(&this->vdd_warning, VDD_WARN_VALUE_MV,
			VDD_WARN_HYSTERESIS_MV, true);
	this->vdd = 0;
	this->vdd_warn_req = true;

	this->motor_water_temp = 0;
	this->motor_oil_press = 0;
	this->alt_p_rpm = 0;
	// start RPM timer
	uv_timer_init(RPM_TIMER);
	uv_timer_start(RPM_TIMER);

	uv_delay_init(&this->motor_delay, MOTOR_DELAY_MS);

	this->pwr.last_limit = 0;
	this->pwr.limit = 0;
	uv_pid_init(&this->pwr.pid, this->pwr_rising_p, 3, 0);
	this->pwr.pump_angle = PWR_USAGE_MAX;

	this->engine_stop_cause = ESB_STOP_NONE;

	//init terminal and pass application terminal commands array as a parameter
	uv_terminal_init(terminal_commands, commands_size());

	this->fsb.ignkey_state = FSB_IGNKEY_STATE_OFF;
	this->fsb.emcy = 0;
	this->hcu.hydr_pressure = 0;
	this->csb.ac_req = 0;
	this->ac_override = false;

	// fetch the display hour counter value from EEPROM
	uv_eeprom_read(&this->hour_counter, sizeof(this->hour_counter), HOUR_ADDR);

	uv_canopen_set_state(CANOPEN_OPERATIONAL);
	uv_canopen_set_sdo_write_callback(&sdo_callback);
}




/// @brief: PT100 resistor teaching values in milliohms
#define PT100_RES_0C_MOHM		100000
#define PT100_RES_100C_MOHM		139000

/// @brief: Indicates the ADC reading when PT100 sensor is
/// in series with 1k5 resistor (voltage divider) and that
/// voltage is amplified by 11 (op amp with 10k + 1k negative feedback)
#define PT100_ADC_0C			2816
#define PT100_ADC_100C			3821
/// @brief: Returns the ADC's read value as celsius degrees
int16_t adc_get_temp(uv_adc_channels_e adc_chn) {
	int16_t ret;
	int32_t adc = uv_adc_read(adc_chn);
	int32_t t = uv_reli(adc, PT100_ADC_0C, PT100_ADC_100C);
	int16_t result_c = uv_lerpi(t, 0, 100);
	if (result_c < TEMP_FAULT_MIN_VAL) {
		ret = TEMP_FAULT_MIN_VAL;
	}
	else if (result_c > TEMP_FAULT_MAX_VAL) {
		ret = TEMP_FAULT_MAX_VAL;
	}
	else {
		ret = result_c;
	}
	return ret;
}




#define LEVEL_0_MV				0
#define LEVEL_100_MV			500
/// @brief: Reads the adc channel and converts it to liquid level percentage
int16_t adc_get_level(uv_adc_channels_e adc_chn) {
	int16_t ret;
	int64_t mv = adc_get_voltage_mv(adc_chn);
	int32_t t = uv_reli(mv, LEVEL_0_MV, LEVEL_100_MV);
	int32_t result = uv_lerpi(t, 0, 100);

	if (result < LEVEL_FAULT_MIN_VAL) {
		ret = LEVEL_FAULT_MIN_VAL;
	}
	else if (result > LEVEL_FAULT_MAX_VAL) {
		ret = LEVEL_FAULT_MAX_VAL;
	}
	else {
		if (result > 100) {
			result = 100;
		}
		ret = result;
	}
	return ret;
}




uint16_t adc_get_voltage_mv(const uv_adc_channels_e adc_chn) {
	int32_t adc = uv_adc_read(adc_chn);
	return adc * 3300 / ADC_MAX_VALUE;
}

void rpm_callb(uv_gpios_e gpio) {
	uint32_t us = uv_timer_get_us(RPM_TIMER);
	uv_timer_clear(RPM_TIMER);
	if (us != 0) {
		uint32_t freq = 1000000 / us;
		this->alt_p_rpm = freq * 5.5f;
	}
}



void step(void* me) {

	init(this);

	while (true) {
		unsigned int step_ms = 2;
		// update watchdog timer value to prevent a hard reset
		// uw_wdt_update();

		uv_output_step(&this->glow, step_ms);
		uv_output_step(&this->starter, step_ms);
		uv_output_step(&this->ac, step_ms);
		uv_output_step(&this->engine_start1, step_ms);
		uv_output_step(&this->engine_start2,step_ms);
		uv_output_step(&this->alt_ig, step_ms);
		uv_output_step(&this->oilcooler, step_ms);
		uv_solenoid_output_step(&this->pump, step_ms);


		// terminal step function
		uv_terminal_step();

		this->total_current = uv_output_get_current(&this->glow) +
				uv_output_get_current(&this->starter) +
				uv_output_get_current(&this->ac) +
				uv_output_get_current(&this->engine_start1) +
				uv_output_get_current(&this->engine_start2) +
				uv_output_get_current(&this->alt_ig) +
				uv_output_get_current(&this->oilcooler) +
				uv_solenoid_output_get_current(&this->pump);

		// motor temperature
		uv_sensor_step(&this->motor_temp, step_ms);
		this->motor_temp_value = (int8_t) uv_sensor_get_value(&this->motor_temp);
		uv_sensor_step(&this->oil_temp, step_ms);
		this->oil_temp_value = (int8_t) uv_sensor_get_value(&this->oil_temp);
		uv_sensor_step(&this->oil_level, step_ms);
		this->oil_level_value = (uint8_t) uv_sensor_get_value(&this->oil_level);

		// kubota sensors
		this->motor_water_temp = GET_MOTOR_WATER();
		this->motor_oil_press = GET_MOTOR_OIL_PRESS();
		this->alt_l = !uv_gpio_get(ALT_L_I);

		// vdd voltage
		// note: Multiplier 11 comes from 10k/1k voltage divider resistors
		// note2: Add 0.7 V since voltage is measured after diode
		this->vdd = uv_moving_aver_step(&this->vdd_avg,
				adc_get_voltage_mv(VDD_SENSE_AIN) * 11) + 700;
		// round vdd up
		this->vdd /= 100;
		this->vdd *= 100;
		// warning when VDD is too low
		if (uv_hysteresis_step(&this->vdd_warning, this->vdd)) {
			if (this->vdd_warn_req &&
					(this->fsb.ignkey_state != FSB_IGNKEY_STATE_PREHEAT) &&
					(this->fsb.ignkey_state != FSB_IGNKEY_STATE_START)) {
				uv_canopen_emcy_send(CANOPEN_EMCY_DEVICE_SPECIFIC, ESB_EMCY_VDD_LOW_WARNING);
			}
			this->vdd_warn_req = false;
		}
		else {
			this->vdd_warn_req = true;
		}


		// rpm
		uint32_t val = uv_timer_get_us(RPM_TIMER);
		// more than second has passed since last pulse
		if (val > 1000000) {
			this->alt_p_rpm = 0;
			uv_timer_clear(RPM_TIMER);
		}

		// **** power usage ****

		uint16_t pressure = this->hcu.hydr_pressure;
		if (pressure == 0) {
			// prevent dividing with zero
			pressure = 1;
		}
		this->pwr.limit = (int64_t) this->alt_p_rpm * this->alt_p_rpm / pressure;
		// scale value to predefined range
		this->pwr.limit /= PWR_LIMIT_DIVIDER;
		// add user calibration parameter into calculations
		this->pwr.limit = this->pwr.limit * this->engine_power_usage / ENGINE_POWER_USAGE_DEFAULT;
		if (this->pwr.limit > PWR_USAGE_MAX) {
			this->pwr.limit = PWR_USAGE_MAX;
		}
		if (this->pwr.limit < this->pwr.last_limit) {
			this->pwr.pump_angle = this->pwr.limit;
		}
		else {
			// slow P controller
			uv_pid_set_p(&this->pwr.pid, this->pwr_rising_p);
			uv_pid_set_target(&this->pwr.pid, this->pwr.limit);
			uv_pid_step(&this->pwr.pid, step_ms, this->pwr.pump_angle);
			this->pwr.pump_angle += uv_pid_get_output(&this->pwr.pid);
		}
		if (this->pwr.pump_angle > PWR_USAGE_MAX) {
			this->pwr.pump_angle = PWR_USAGE_MAX;
		}
		if (this->pwr.pump_angle < 1) {
			this->pwr.pump_angle = 1;
		}
		// pump control is disabled if alternator signals an error. This is to make sure that
		// the machine works (more or less) even if alternator is not working, otherwise
		// the RPM would seem to be 0 and thus pump angle would also be 0.
		if (this->alt_l ||
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_OFF)) {
			this->pwr.pump_angle = 0;
		}
		else if (this->fsb.ignkey_state != FSB_IGNKEY_STATE_ON) {
			this->pwr.pump_angle = 1;
		}
		else {

		}

		// set the pump solenoid output based on the power usage calculations
		uv_solenoid_output_set(&this->pump, this->pwr.pump_angle);
		this->pwr.last_limit = this->pwr.limit;

//		printf("%i %i\n", this->pwr.pump_angle, uv_solenoid_output_get_current(&this->pump));


		// **** ignition key states ****

		if (this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) {
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
		else if ((this->fsb.ignkey_state == FSB_IGNKEY_STATE_START) ||
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_PREHEAT)) {
			uv_output_set_state(&this->engine_start1, OUTPUT_STATE_ON);
			uv_output_set_state(&this->engine_start2, OUTPUT_STATE_ON);
		}
		else {
			if (uv_output_get_state(&this->engine_start1) != OUTPUT_STATE_OFF ||
					uv_output_get_state(&this->engine_start2) != OUTPUT_STATE_OFF) {
				this->engine_stop_cause = ESB_STOP_IGNKEY;
			}
			uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
			uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
		}
		if (uv_output_get_state(&this->engine_start1) == OUTPUT_STATE_OVERLOAD) {
			this->engine_stop_cause = ESB_STOP_SOLENOID1_OVERCURRENT;
		}
		else if (uv_output_get_state(&this->engine_start2) == OUTPUT_STATE_OVERLOAD) {
			this->engine_stop_cause = ESB_STOP_SOLENOID2_OVERCURRENT;
		}
		else {

		}

		// motor sensor shut down
		if ((this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) &&
				(this->alt_p_rpm != 0)) {
			if (this->motor_water_temp ||
					this->motor_oil_press) {
				if (uv_delay(&this->motor_delay, step_ms)) {
					uv_output_set_state(&this->engine_start1, OUTPUT_STATE_OFF);
					uv_output_set_state(&this->engine_start2, OUTPUT_STATE_OFF);
					// send EMCY message from motor protection
					uv_canopen_emcy_send(CANOPEN_EMCY_DEVICE_SPECIFIC,
							ESB_EMCY_ENGINE_PROTECTION_SHUTDOWN);
					// store engine stop cause
					if (this->motor_water_temp) {
						this->engine_stop_cause = ESB_STOP_WATER_TEMP;
					}
					else if (this->motor_oil_press) {
						this->engine_stop_cause = ESB_STOP_OIL_PRESS;
					}
					else {

					}
				}
			}
			else {
				uv_delay_init(&this->motor_delay, MOTOR_DELAY_MS);
			}
		}


		// glow is active only when ignition key is in PREHEAT position
		uv_output_set_state(&this->glow,
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_PREHEAT) ?
						OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// starter is active only when ignition key is in START position
		uv_output_set_state(&this->starter,
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_START) ?
				OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// ac is controlled by CSB when ignition key is in ON position
		uv_output_state_e state;
		if (this->ac_override) {
			// state machine override from terminal
			state = OUTPUT_STATE_ON;
		}
		else if (this->fsb.ignkey_state != FSB_IGNKEY_STATE_ON) {
			// ignition key state
			state = OUTPUT_STATE_OFF;
		}
		else {
			// csb request
			state = (this->csb.ac_req) ? OUTPUT_STATE_ON : OUTPUT_STATE_OFF;
		}
		uv_output_set_state(&this->ac, state);

		// alternator ignition is on if engine is running and starter is not running
		uv_output_set_state(&this->alt_ig,
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON) ?
				OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// oil cooler control
		uv_hysteresis_set_trigger_value(&this->oil_temp_hyst, this->oilcooler_trigger_temp);
		if (uv_sensor_get_state(&this->oil_temp) == SENSOR_STATE_OK) {
			uv_hysteresis_step(&this->oil_temp_hyst, uv_sensor_get_value(&this->oil_temp));
			uv_output_set_state(&this->oilcooler, (uv_hysteresis_get_output(&this->oil_temp_hyst)) ?
					OUTPUT_STATE_ON : OUTPUT_STATE_OFF);
		}
		else {
			uv_output_set_state(&this->oilcooler, OUTPUT_STATE_OFF);
		}

		// if FSB heartbeat message is not received in a given time,
		// it indicates that FSB is not in the system. As FSB takes care of the EMCY switch,
		// the best practice would be to assume that the EMCY switch is turned on.
		if (uv_canopen_heartbeat_producer_is_expired(FSB_NODE_ID)) {
			this->fsb.ignkey_state = FSB_IGNKEY_STATE_OFF;
			this->engine_stop_cause = ESB_STOP_FSB_DISCONNECTED;
			this->fsb.emcy = 1;
		}
		else if (this->fsb.emcy) {
			this->engine_stop_cause = ESB_STOP_EMCY;
		}
		else {

		}

		// emcy
		if (this->fsb.emcy) {
			uv_output_disable(&this->glow);
			uv_output_disable(&this->starter);
			uv_output_disable(&this->ac);
			uv_output_disable(&this->engine_start1);
			uv_output_disable(&this->engine_start2);
			uv_output_disable(&this->alt_ig);
			uv_output_disable(&this->oilcooler);
			uv_solenoid_output_disable(&this->pump);
		}
		else {
			uv_output_set_enabled(&this->glow, this->glow_enabled);
			uv_output_set_enabled(&this->starter, this->starter_enabled);
			uv_output_set_enabled(&this->ac, this->ac_enabled);
			uv_output_set_enabled(&this->engine_start1, this->engine_start_enabled);
			uv_output_set_enabled(&this->engine_start2, this->engine_start_enabled);
			uv_output_set_enabled(&this->alt_ig, this->alt_ig_enabled);
			uv_output_set_enabled(&this->oilcooler, this->oilcooler_enabled);
			uv_output_set_enabled((uv_output_st*) &this->pump, this->pump_enabled);
		}

		uv_rtos_task_delay(step_ms);
	}
}


void sdo_callback(uint16_t mindex, uint8_t sindex) {
	if ((mindex == ESB_HOUR_INDEX) && (sindex == ESB_HOUR_SUBINDEX)) {
		// hour counter value updated, save the value to EEPROM
		uv_eeprom_write(&this->hour_counter, sizeof(this->hour_counter), HOUR_ADDR);
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
