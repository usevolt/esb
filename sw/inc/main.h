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
#define HYDROIL_TEMP_WARN_VALUE		80
/// @brief: Motor temperature max limit in celsius.
/// Error is generated after this has been exceeded
#define HYDROIL_TEMP_ERR_VALUE		90
#define HYDROIL_TEMP_HYSTERESIS		4
#define HYDROIL_TEMP_AVG_COUNT		100

/// @brief: Fuel level min limit in percents.
/// Warning is generated after this has been exceeded
#define FUEL_LEVEL_WARN_VALUE		25
/// @brief: fuel level min limit in percents.
/// Error is generated after this has been exceeded
#define FUEL_LEVEL_ERR_VALUE		10
#define FUEL_LEVEL_HYSTERESIS		5
#define FUEL_LEVEL_AVG_COUNT		100




/// @brief: main data structure.
/// This struct can be save to non-volatile flash memory with
/// a terminal command 'save'.
typedef struct _dev_st {

	uv_moving_aver_st motor_temp;
	uv_hysteresis_st motor_temp_warn;
	uv_hysteresis_st motor_temp_err;

	uv_moving_aver_st hydroil_temp;
	uv_hysteresis_st hydroil_temp_warn;
	uv_hysteresis_st hydroil_temp_err;

	uv_moving_aver_st fuel_level;
	uv_hysteresis_st fuel_level_warn;
	uv_hysteresis_st fuel_level_err;

	uv_data_start_t data_start;

	/// @brief: CANopen object dictionary structure
	struct objdict {
	};

	uv_data_end_t data_end;

} dev_st;




void step(void* me);

void init(dev_st* me);


#endif /* MAIN_H_ */
