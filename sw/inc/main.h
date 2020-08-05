/*
 * main.h
 *
 *  Created on: Apr 26, 2015
 *      Author: usevolt
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <uv_memory.h>
#include <uv_filters.h>
#include <uv_output.h>
#include <uv_solenoid_output.h>
#include <uv_sensor.h>
#include "can_fsb.h"

/// brief: Delay for motor_oil_press and motor_water_temp
/// warning signals
#define MOTOR_DELAY_MS						10000

#define TEMP_FAULT_MAX_VAL					125
#define TEMP_FAULT_MIN_VAL					-50

/// @brief: Motor temperature max limit in celsius.
/// Warning is generated after this has been exceeded
#define MOTOR_TEMP_WARN_VALUE				100
/// @brief: Motor temperature max limit in celsius.
/// Error is generated after this has been exceeded
#define MOTOR_TEMP_ERR_VALUE				108
#define MOTOR_TEMP_HYSTERESIS				10
#define MOTOR_TEMP_AVG_COUNT				500


/// @brief: Hydraulic oil temperature max limit in celsius.
/// Warning is generated after this has been exceeded
#define OIL_TEMP_WARN_VALUE					90
/// @brief: Motor temperature max limit in celsius.
/// Error is generated after this has been exceeded
#define OIL_TEMP_ERR_VALUE					98
#define OIL_TEMP_HYSTERESIS					10
#define OIL_TEMP_AVG_COUNT					100


#define LEVEL_FAULT_MAX_VAL					110
#define LEVEL_FAULT_MIN_VAL					0

/// @brief: Oil level min limit in percents.
/// Warning is generated after this has been exceeded
#define OIL_LEVEL_WARN_VALUE				50
/// @brief: oil level min limit in percents.
/// Error is generated after this has been exceeded
#define OIL_LEVEL_ERR_VALUE					30
#define OIL_LEVEL_HYSTERESIS				15
#define OIL_LEVEL_AVG_COUNT					100

#define VDD_AVG_COUNT						10
#define VDD_WARN_VALUE_MV					11500
#define VDD_WARN_HYSTERESIS_MV				500

#define OUTPUT_MOVING_AVG_COUNT				50

#define OIL_TEMP_HYSTERESIS_C				15
#define OIL_TEMP_DEFAULT_TRIGGER_VALUE_C	70

#define ENGINE_POWER_USAGE_DEFAULT			55
#define PUMP_CURRENT_MIN_MA					400
#define PUMP_CURRENT_MAX_MA					1200
#define PWR_RISING_P_DEFAULT				10
#define PWR_USAGE_MAX						1000


#define RADIATOR_DELAY_MS					10000
#define RADIATOR_TRIGGER_C					75
#define RADIATOR_HYSTERESIS					13

/// @brief: EEPROM address for the hour counter
#define HOUR_ADDR							0


/// @brief: main data structure.
/// This struct can be save to non-volatile flash memory with
/// a terminal command 'save'.
typedef struct _dev_st {

	uv_output_st glow;
	uv_output_st starter;
	uv_output_st ac;
	uv_output_st engine_start1;
	uv_output_st engine_start2;
	uv_output_st alt_ig;
	uv_output_st oilcooler;
	uv_solenoid_output_st pump;
	uv_output_st radiator;

	uint16_t total_current;

	uv_delay_st oilc_restart_delay;

	/// @brief: Hour counter multiplexed with
	/// uw_display.
	uint32_t hour_counter;

	uv_sensor_st motor_temp;
	int8_t motor_temp_value;
	uv_sensor_st oil_temp;
	int8_t oil_temp_value;
	uv_sensor_st oil_level;
	uint8_t oil_level_value;

	uv_hysteresis_st oil_temp_hyst;

	uv_delay_st radiator_delay;

	CANOPEN_TYPEOF(ESB_ENGINE_STOP_CAUSE_TYPE) engine_stop_cause;

	uint16_t alt_p_rpm;
	uint8_t alt_l;
	uint8_t motor_water_temp;
	uint8_t motor_oil_press;
	uv_delay_st motor_delay;

	uv_moving_aver_st vdd_avg;
	uv_hysteresis_st vdd_warning;
	uint16_t vdd;
	bool vdd_warn_req;

	struct {
		// stores the current power limit from rpm and pressure.
		// Machine shouldnt draw more power than this
		int32_t limit;
		// last calculated value
		int32_t last_limit;
		// rising value pid controller
		uv_pid_st pid;
		// result value of the controller. 0...1000
		int16_t pump_angle;
	} pwr;


	struct {
		fsb_ignkey_states_e ignkey_state;
		uint8_t emcy;
	} fsb;
	struct {
		int16_t hydr_pressure;
	} hcu;
	struct {
		uint8_t ac_req;
	} csb;

	uint8_t ac_override;

	// non-volatile data start
	uv_data_start_t data_start;

	uint8_t starter_enabled;
	uint8_t glow_enabled;
	uint8_t ac_enabled;
	uint8_t engine_start_enabled;
	uint8_t alt_ig_enabled;
	uint8_t oilcooler_enabled;
	uint8_t pump_enabled;
	uint8_t radiator_enabled;


	uv_solenoid_output_conf_st pump_conf;

	int8_t oilcooler_trigger_temp;

	uint8_t engine_power_usage;
	// Kp factor for increasing the pump angle
	uint16_t pwr_rising_p;

	/// @brief: Proportional solenoid dither frequency
	uint8_t dither_freq;
	/// @brief: Proportional solenoid dither amplitude
	uint16_t dither_ampl;

	uv_data_end_t data_end;

} dev_st;


void adc_callback(void);

void step(void* me);

void init(dev_st* me);





#endif /* MAIN_H_ */
