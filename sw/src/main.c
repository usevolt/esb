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
#include <uv_timer.h>
#include <string.h>

dev_st dev = {};
#define this ((dev_st*) &dev)
static bool initialized = false;

int16_t adc_get_temp(uv_adc_channels_e adc_chn);
int16_t adc_get_level(uv_adc_channels_e adc_chn);
uint16_t adc_get_voltage_mv(const uv_adc_channels_e adc_chn);
void sdo_callback(uint16_t mindex, uint8_t sindex);


#define GET_MOTOR_WATER()	(!uv_gpio_get(MOTOR_WATER_TEMP_I))
#define GET_MOTOR_OIL_PRESS() (!uv_gpio_get(MOTOR_OIL_PRESS_I))

#define OILC_RESTART_DELAY_MS		10000


#define VND5050_CURRENT_AMPL_UA		1619
#define VN5E01_CURRENT_AMPL_UA		5600

#define PWR_LIMIT_DIVIDER			20


static bool can_callb(void *user_ptr, uv_can_msg_st *msg) {
	bool ret = true;

	if (msg->type == CAN_EXT) {
		if (msg->id == 0x0CF00400 &&
				msg->data_length >= 5) {
			uint16_t rpm = msg->data_8bit[3] + (msg->data_8bit[4] << 8);
			rpm /= 8;
			this->alt_p_rpm = rpm;
		}
		else if (msg->id == 0x18FD7C00 &&
				msg->data_length >= 7) {
			// diesel particulate filter status message
			this->partfilter_status = (msg->data_8bit[1] >> 4) & 0b11;

			this->partfilter_inhibits = 0;
			if (msg->data_8bit[2] & (0b11 << 0)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_STATUS;
			}
			if (msg->data_8bit[2] & (0b11 << 2)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_SW;
			}
			if (msg->data_8bit[3] & (0b11 << 2)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_ACCPEDAL;
			}
			if (msg->data_8bit[3] & (0b11 << 4)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_NEUTRAL;
			}
			if (msg->data_8bit[4] & (0b11 << 0)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_PARKINGBRAKE;
			}
			if (msg->data_8bit[4] & (0b11 << 2)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_EXHGASTEMP;
			}
			if (msg->data_8bit[4] & (0b11 << 4)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_SYSFAULT;
			}
			if (msg->data_8bit[4] & (0b11 << 6)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_TIMEOUT;
			}
			if (msg->data_8bit[5] & (0b11 << 2)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_LOCKOUT;
			}
			if (msg->data_8bit[5] & (0b11 << 4)) {
				this->partfilter_inhibits |= PARTFILTER_INHIBIT_NOTWARM;
			}

		}
		else {

		}
	}

	return ret;
}

void init(dev_st* me) {

	// load non-volatile data
	if (uv_memory_load(MEMORY_ALL_PARAMS)) {
		this->oilcooler_trigger_temp = OIL_TEMP_DEFAULT_TRIGGER_VALUE_C;
		uv_solenoid_output_conf_reset(&this->pump_conf);

		// initialize non-volatile memory to default settings
		this->starter_enabled = true;
		this->glow_enabled = true;
		this->ac_enabled = true;
		this->oilcooler_enabled = true;
		this->pump_enabled = true;
		this->radiator_enabled = true;

		this->idle_rpm = IDLE_RPM_DEF;
		this->work_rpm = WORK_RPM_DEF;
		this->drive_rpm = DRIVE_RPM_DEF;

		this->dither_ampl = 0;
		this->dither_freq = 50;
		this->engine_power_usage = ENGINE_POWER_USAGE_DEFAULT;
		this->pwr_rising_p = PWR_RISING_P_DEFAULT;
		uv_memory_save();
	}
	// initialize outputs
	uv_output_init(&this->glow, GLOW_SENSE_AIN,
			GLOW_PLUGS_O, VN5E01_CURRENT_AMPL_UA * 2, 50000, 60000,
			OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_GLOW_OVERLOAD, ESB_EMCY_GLOW_FAULT);
	uv_output_init(&this->starter, STARTER_SENSE_AIN,
			STARTER_O, VN5E01_CURRENT_AMPL_UA, 30000, 45000,
			OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_STARTER_OVERLOAD, ESB_EMCY_STARTER_FAULT);
	uv_output_init(&this->ac, AC_SENSE_AIN,
			AC_O, VN5E01_CURRENT_AMPL_UA, 10000, 15000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_AC_OVERLOAD, ESB_EMCY_AC_FAULT);
	uv_output_init(&this->oilcooler, OILCOOLER_AIN, OILCOOLER_O,
			VN5E01_CURRENT_AMPL_UA, 15000, 30000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_OILCOOLER_OVERCURRENT, ESB_EMCY_OILCOOLER_FAULT);
	uv_solenoid_output_init(&this->pump, &this->pump_conf, PUMP_PWM, this->dither_freq,
			DUTY_CYCLEPPT(this->dither_ampl), PUMP_SENSE_AIN,
			VND5050_CURRENT_AMPL_UA, 3500, 5000,
			ESB_EMCY_PUMP_OVERLOAD, ESB_EMCY_PUMP_FAULT);
	uv_solenoid_output_get_conf(&this->pump)->max_ma = PUMP_CURRENT_MAX_MA;
	uv_solenoid_output_get_conf(&this->pump)->min_ma = PUMP_CURRENT_MIN_MA;

	uv_output_init(&this->radiator, RADIATOR_AIN, RADIATOR_O,
			VN5E01_CURRENT_AMPL_UA * 2, 50000, 60000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_RADIATOR_OVERCURRENT, ESB_EMCY_RADIATOR_FAULT);
	uv_output_init(&this->engine_start, ENGINE_START_SENSE_AIN, ENGINE_START_O,
			VN5E01_CURRENT_AMPL_UA, 10000, 15000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_START_OVERCURRENT, ESB_EMCY_ENGINE_START_FAULT);
	uv_output_init(&this->engine_ig, ENGINE_IG_SENSE_AIN, ENGINE_IG_O,
			VND5050_CURRENT_AMPL_UA, 5000, 10000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_IG_OVERCURRENT, ESB_EMCY_ENGINE_IG_FAULT);
	uv_output_init(&this->engine_main, ENGINE_MAIN_SENSE_AIN, ENGINE_MAIN_O,
			VN5E01_CURRENT_AMPL_UA, 15000, 20000, OUTPUT_MOVING_AVG_COUNT,
			ESB_EMCY_ENGINE_MAIN_OVERCURRENT, ESB_EMCY_ENGINE_MAIN_FAULT);

	// initialize inputs
	UV_GPIO_INIT_INPUT(ALT_L_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(ALT_P_RPM_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(ENGINE_MAIN_RELAY_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(STARTER_RELAY_I, PULL_UP_ENABLED);

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
	this->engine_main_relay_i = 0;
	this->starter_relay_i = 0;
	this->alt_p_rpm = 0;
	this->engine_rpm_req = this->idle_rpm;
	this->partfilter_status = 0;
	this->partfilter_inhibits = 0;
	this->regen_force = 0;

	uv_delay_init(&this->radiator_delay, RADIATOR_DELAY_MS);

	uv_delay_init(&this->motor_delay, MOTOR_DELAY_MS);

	uv_delay_init(&this->oilc_restart_delay, OILC_RESTART_DELAY_MS);

	this->pwr.last_limit = 0;
	this->pwr.limit = 0;
	uv_pid_init(&this->pwr.pid, this->pwr_rising_p, 6, 0);
	this->pwr.pump_angle = PWR_USAGE_MAX;

	this->engine_stop_cause = ESB_STOP_NONE;

	//init terminal and pass application terminal commands array as a parameter
	uv_terminal_init(terminal_commands, commands_size());

	this->fsb.ignkey_state = FSB_IGNKEY_STATE_OFF;
	this->fsb.emcy = 0;
	this->hcu.hydr_pressure = 0;
	this->hcu.work_active = 0;
	this->csb.ac_req = 0;
	this->ccu.work_active = 0;
	this->ccu.drive_active = 0;

	this->ac_override = false;

	// fetch the display hour counter value from EEPROM
	uv_eeprom_read(&this->hour_counter, sizeof(this->hour_counter), HOUR_ADDR);

	uv_canopen_set_state(CANOPEN_OPERATIONAL);
	uv_canopen_set_sdo_write_callback(&sdo_callback);

	uv_can_config_rx_message(CAN0, 0x0CF00400, CAN_ID_MASK_DEFAULT, CAN_EXT);
	uv_can_config_rx_message(CAN0, 0x18FD7C00, CAN_ID_MASK_DEFAULT, CAN_EXT);
	uv_can_add_rx_callback(CAN0, &can_callb);

	initialized = true;

}




/// @brief: PT100 resistor teaching values in milliohms
#define PT100_RES_0C_MOHM		100000
#define PT100_RES_100C_MOHM		139000

/// @brief: Indicates the ADC reading when PT100 sensor is
/// in series with 1k5 resistor (voltage divider) and that
/// voltage is amplified by 11 (op amp with 10k + 1k negative feedback)
#define PT100_ADC_0C			2816
#define PT100_ADC_100C			3821


int temp_compare(const void * val1, const void *val2) {
	int ret;
	if (*((int32_t*) val1) > *((int32_t*) val2)) {
		ret = 1;
	}
	else if (*((int32_t*) val1) < *((int32_t*) val2)) {
		ret = -1;
	}
	else {
		ret = 0;
	}
	return ret;
}
#define ADC_COUNT				5
/// @brief: Returns the ADC's read value as celsius degrees
int16_t adc_get_temp(uv_adc_channels_e adc_chn) {
	int16_t ret;
	int32_t adc[ADC_COUNT];
	for (uint8_t i = 0; i < ADC_COUNT; i++) {
		adc[i] = uv_adc_read(adc_chn);
	}
	qsort(adc, ADC_COUNT, sizeof(adc[0]), &temp_compare);

	int32_t t = uv_reli(adc[ADC_COUNT/2], PT100_ADC_0C, PT100_ADC_100C);
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


void solenoid_step(void* me) {

	// wait until solenoid structures have been initialized
	while (!initialized) {
		uv_rtos_task_delay(1);
	}

	while (true) {
		uint32_t step_ms = 2;

		uv_solenoid_output_step(&this->pump, step_ms);

		uv_rtos_task_delay(step_ms);
	}
}


void step(void* me) {

	init(this);

	while (true) {
		unsigned int step_ms = 10;
		// update watchdog timer value to prevent a hard reset
		// uw_wdt_update();

		uv_output_step(&this->glow, step_ms);
		uv_output_step(&this->starter, step_ms);
		uv_output_step(&this->ac, step_ms);
		uv_output_step(&this->oilcooler, step_ms);
		uv_output_step(&this->radiator, step_ms);
		uv_output_step(&this->engine_start, step_ms);
		uv_output_step(&this->engine_ig, step_ms);
		uv_output_step(&this->engine_main, step_ms);


		// terminal step function
		uv_terminal_step();

		this->total_current = uv_output_get_current(&this->glow) +
				uv_output_get_current(&this->starter) +
				uv_output_get_current(&this->ac) +
				uv_output_get_current(&this->oilcooler) +
				uv_solenoid_output_get_current(&this->pump) +
				uv_output_get_current(&this->radiator) +
				uv_output_get_current(&this->engine_start) +
				uv_output_get_current(&this->engine_ig) +
				uv_output_get_current(&this->engine_main);

		// motor temperature
		uv_sensor_step(&this->motor_temp, step_ms);
		this->motor_temp_value = (int8_t) uv_sensor_get_value(&this->motor_temp);
		uv_sensor_step(&this->oil_temp, step_ms);
		this->oil_temp_value = (int8_t) uv_sensor_get_value(&this->oil_temp);
		uv_sensor_step(&this->oil_level, step_ms);
		this->oil_level_value = (uint8_t) uv_sensor_get_value(&this->oil_level);

		// engine rpm handling
		if (uv_canopen_heartbeat_producer_is_expired(HCU_NODE_ID)) {
			memset(&this->hcu, 0, sizeof(this->hcu));
		}
		if (uv_canopen_heartbeat_producer_is_expired(CCU_NODE_ID)) {
			memset(&this->ccu, 0, sizeof(this->ccu));
		}
		if (this->ccu.drive_active) {
			this->engine_rpm_req = this->drive_rpm;
		}
		else if (this->ccu.work_active ||
				this->hcu.work_active) {
			this->engine_rpm_req = this->work_rpm;
		}
		else {
			this->engine_rpm_req = this->idle_rpm;
		}

		// vdd voltage
		// note: Multiplier 11 comes from 10k/1k voltage divider resistors
		this->vdd = uv_moving_aver_step(&this->vdd_avg,
				adc_get_voltage_mv(VDD_SENSE_AIN) * 11);
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


		// **** power usage ****
		uint16_t pressure = this->hcu.hydr_pressure;
		if (pressure <= 0) {
			// prevent dividing with zero
			pressure = 1;
		}
		int64_t rpm = this->alt_p_rpm;
		this->pwr.limit = (int64_t) rpm * rpm / pressure;
		// scale value to predefined range
		this->pwr.limit /= PWR_LIMIT_DIVIDER;
		// add user calibration parameter into calculations
		this->pwr.limit = this->pwr.limit * this->engine_power_usage / ENGINE_POWER_USAGE_DEFAULT;
		if (this->pwr.limit > PWR_USAGE_MAX) {
			this->pwr.limit = PWR_USAGE_MAX;
		}
		if (this->pwr.limit < this->pwr.pump_angle) {
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

//		printf("%i %i\n", this->pwr.pump_angle, this->pwr.limit);



		this->engine_main_relay_i = !uv_gpio_get(ENGINE_MAIN_RELAY_I);
		this->starter_relay_i = !uv_gpio_get(STARTER_RELAY_I);

		// *** main relay output ****
		uv_output_set_state(&this->engine_main, this->engine_main_relay_i ?
				OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// **** ignition key states ****
		uv_output_set_state(&this->engine_ig,
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON ||
				this->fsb.ignkey_state == FSB_IGNKEY_STATE_PREHEAT ||
				this->fsb.ignkey_state == FSB_IGNKEY_STATE_START) ?
						OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// glow is active only when ignition key is in PREHEAT position
		uv_output_set_state(&this->glow,
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_PREHEAT) ?
						OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// starter is active only when ignition key is in START position
		uv_output_set_state(&this->starter,
				((this->fsb.ignkey_state == FSB_IGNKEY_STATE_START) &&
						!!this->starter_relay_i == true) ?
				OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// engine start is active when they key is turned
		uv_output_set_state(&this->engine_start,
				this->fsb.ignkey_state == FSB_IGNKEY_STATE_START ?
						OUTPUT_STATE_ON : OUTPUT_STATE_OFF);

		// radiator logic
		// radiator should go on little after the engine has been turned on
		uv_output_state_e radiator_on = OUTPUT_STATE_OFF;
		if (this->radiator_enabled &&
				(this->fsb.ignkey_state == FSB_IGNKEY_STATE_ON)) {
			if (uv_delay(&this->radiator_delay, step_ms) ||
					uv_delay_has_ended(&this->radiator_delay)) {
				radiator_on = OUTPUT_STATE_ON;
			}
		}
		else {
			uv_delay_init(&this->radiator_delay, RADIATOR_DELAY_MS);
		}
		uv_output_set(&this->radiator, radiator_on);

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

		// oil cooler control
		uv_hysteresis_set_trigger_value(&this->oil_temp_hyst, this->oilcooler_trigger_temp);
		if (uv_sensor_get_state(&this->oil_temp) == SENSOR_STATE_OK &&
				this->fsb.ignkey_state != FSB_IGNKEY_STATE_OFF) {
			uv_output_state_e state = uv_output_get_state(&this->oilcooler);
			if (state == OUTPUT_STATE_FAULT ||
					state == OUTPUT_STATE_OVERLOAD) {
				if (uv_delay(&this->oilc_restart_delay, step_ms)) {
					uv_output_set_state(&this->oilcooler, OUTPUT_STATE_OFF);
					uv_delay_init(&this->oilc_restart_delay, OILC_RESTART_DELAY_MS);
				}
			}
			else {
				uv_hysteresis_step(&this->oil_temp_hyst, uv_sensor_get_value(&this->oil_temp));
				uv_output_set_state(&this->oilcooler,
						(uv_hysteresis_get_output(&this->oil_temp_hyst)) ?
								OUTPUT_STATE_ON : OUTPUT_STATE_OFF);
			}
		}
		else {
			uv_output_set_state(&this->oilcooler, OUTPUT_STATE_OFF);
		}


		// *** engine RPM ***
		uv_can_msg_st msg = {
				.type = CAN_EXT,
				.id = 0x18FF5300,
				.data_length = 8,
				.data_64bit = 0
		};
		uint16_t r = this->engine_rpm_req;
		msg.data_8bit[1] = (r & 0xFF);
		msg.data_8bit[2] = (r >> 8);
		msg.data_8bit[3] = 1; // ICR mode
		uv_can_send(CAN0, &msg);

		uint8_t engine_addr = 0xE4;
		// parking switch
		msg.id = 0x18FEF100 + engine_addr;
		memset(msg.data_8bit, 0, sizeof(msg.data_8bit));
		msg.data_8bit[0] = (1 << 2);
		uv_can_send(CAN0, &msg);

		// neutral switch
		msg.id = 0x1CFEC300 + engine_addr;
		memset(msg.data_8bit, 0, sizeof(msg.data_8bit));
		msg.data_8bit[1] = (1 << 2);
		uv_can_send(CAN0, &msg);

		// engine speed
		msg.id = 0x18FEF100 + engine_addr;
		memset(msg.data_8bit, 0, sizeof(msg.data_8bit));
		msg.data_8bit[1] = 0;
		msg.data_8bit[2] = 0;
		uv_can_send(CAN0, &msg);

		if (this->partfilter_status == 0) {
			this->regen_force = 0;
		}
		memset(msg.data_8bit, 0, sizeof(msg.data_8bit));
		msg.id = 0x18E00000 + engine_addr;
		msg.data_length = 8;
		if (this->regen_force) {
			msg.data_8bit[5] = (1 << 2); // regen inhibit off, regen force active
		}
		uv_can_send(CAN0, &msg);

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
			uv_output_disable(&this->oilcooler);
			uv_solenoid_output_disable(&this->pump);
			uv_output_disable(&this->radiator);
			uv_output_disable(&this->engine_start);
			uv_output_disable(&this->engine_ig);
			uv_output_disable(&this->engine_main);
		}
		else {
			uv_output_set_enabled(&this->glow, this->glow_enabled);
			uv_output_set_enabled(&this->starter, this->starter_enabled);
			uv_output_set_enabled(&this->ac, this->ac_enabled);
			uv_output_set_enabled(&this->oilcooler, this->oilcooler_enabled);
			uv_output_set_enabled((uv_output_st*) &this->pump, this->pump_enabled);
			uv_output_enable(&this->radiator);
			uv_output_enable(&this->engine_start);
			uv_output_enable(&this->engine_ig);
			uv_output_enable(&this->engine_main);
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

	uv_init(&dev);

	uv_rtos_task_create(step, "step", UV_RTOS_MIN_STACK_SIZE * 5,
			&dev, UV_RTOS_IDLE_PRIORITY + 1, NULL);

	uv_rtos_task_create(&solenoid_step, "solstep", UV_RTOS_MIN_STACK_SIZE,
			&dev, UV_RTOS_IDLE_PRIORITY + 1, NULL);


	uv_rtos_start_scheduler();


    // Enter an infinite loop
	// never should end up here
    while(1) {
    }
    return 0;
}
