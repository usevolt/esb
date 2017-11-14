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
#include "sensor.h"
#include "can_fsb.h"

/// brief: Delay for motor_oil_press and motor_water_temp
/// warning signals
#define MOTOR_DELAY_MS			2000


/// @brief: Motor temperature max limit in celsius.
/// Warning is generated after this has been exceeded
#define MOTOR_TEMP_WARN_VALUE		90
/// @brief: Motor temperature max limit in celsius.
/// Error is generated after this has been exceeded
#define MOTOR_TEMP_ERR_VALUE		95
#define MOTOR_TEMP_HYSTERESIS		2
#define MOTOR_TEMP_AVG_COUNT		100


/// @brief: Hydraulic oil temperature max limit in celsius.
/// Warning is generated after this has been exceeded
#define OIL_TEMP_WARN_VALUE			80
/// @brief: Motor temperature max limit in celsius.
/// Error is generated after this has been exceeded
#define OIL_TEMP_ERR_VALUE			90
#define OIL_TEMP_HYSTERESIS			4
#define OIL_TEMP_AVG_COUNT			100

/// @brief: Fuel level min limit in percents.
/// Warning is generated after this has been exceeded
#define FUEL_LEVEL_WARN_VALUE		30
/// @brief: fuel level min limit in percents.
/// Error is generated after this has been exceeded
#define FUEL_LEVEL_ERR_VALUE		5
#define FUEL_LEVEL_HYSTERESIS		10
#define FUEL_LEVEL_AVG_COUNT		100

/// @brief: Oil level min limit in percents.
/// Warning is generated after this has been exceeded
#define OIL_LEVEL_WARN_VALUE		70
/// @brief: oil level min limit in percents.
/// Error is generated after this has been exceeded
#define OIL_LEVEL_ERR_VALUE			50
#define OIL_LEVEL_HYSTERESIS		10
#define OIL_LEVEL_AVG_COUNT			100

#define VDD_AVG_COUNT				10
#define VDD_WARN_VALUE_MV			11000
#define VDD_WARN_HYSTERESIS_MV		500

#define OUTPUT_MOVING_AVG_COUNT		100





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
	uv_solenoid_output_st pump;

	uint16_t total_current;

	sensor_st motor_temp;
	sensor_st oil_temp;
	sensor_st fuel_level;
	sensor_st oil_level;

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
		fsb_ignkey_states_e ignkey_state;
		uint8_t emcy;
	} fsb;

	bool network_override;

	uv_data_start_t data_start;

	uv_data_end_t data_end;

} dev_st;


void adc_callback(void);

void step(void* me);

void init(dev_st* me);





#endif /* MAIN_H_ */
