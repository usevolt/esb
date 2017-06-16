/*
 * mosfet_driver.h
 *
 *  Created on: Jun 13, 2017
 *      Author: usevolt
 */

#ifndef MOSFET_DRIVER_H_
#define MOSFET_DRIVER_H_


/// @file: Power mosfet driver module. Monitors the mosfet overcurrent state


#include <uv_utilities.h>
#include <uv_filters.h>
#include <uv_adc.h>


typedef enum {
	MOSFET_STATE_OFF = 0,
	MOSFET_STATE_ON,
	MOSFET_STATE_OVERCURRENT,
	MOSFET_STATE_UNDERCURRENT
} mosfet_state_e;


typedef struct {
	/// @brief: ADC channel of the sense resistor
	uv_adc_channels_e adc_chn;
	/// @brief: Current sense resistor value in microohms
	uint16_t sense_uohm;
	/// @brief: Current sense resistor amplification (defaults to 50)
	uint16_t sense_ampl;
	uv_hysteresis_st min_hyst;
	uv_hysteresis_st max_hyst;
	uv_moving_aver_st moving_avg;
	/// @brief: Mosfet module state
	mosfet_state_e state;
	/// @brief: gpio pin for the gate driving
	uv_gpio_e gate_io;
} mosfet_st;



/// @brief: Initializes the mosfet driver module.
void mosfet_init(mosfet_st *this, const uv_adc_channels_e adc_chn, const uv_gpio_e gate_io,
		const uint16_t sense_uohm, const int32_t min_val, const int32_t max_val,
		const uint32_t hysteresis, const uint16_t moving_avg_count);



/// @brief: Sets the current sense amplification value. Defaults to 50
static inline void mosfet_set_ampl(mosfet_st *this, const uint16_t value) {
	this->sense_ampl = value;
}



static inline mosfet_state_e mosfet_get_state(const mosfet_st *this) {
	return this->state;
}


/// @brief: Step function should be called every step cycle.
void mosfet_step(mosfet_st *this, uint16_t step_ms);












#endif /* MOSFET_DRIVER_H_ */
