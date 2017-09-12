/*
 * pin_mappings.h
 *
 *  Created on: Jan 27, 2015
 *      Author: usenius
 *
 *  Defines keypad pin mappings for different pcb revisions used
 */

#ifndef UW_MB_SRC_PIN_MAPPINGS_H_
#define UW_MB_SRC_PIN_MAPPINGS_H_

#include <uv_gpio.h>
#include <uv_timer.h>
#include <uv_adc.h>


// Analog inputs
#define VDD_SENSE_AIN				ADC0_10
#define GLOW_SENSE_AIN				ADC0_8
#define STARTER_SENSE_AIN			ADC0_9
#define AC_SENSE_AIN				ADC0_6
#define ENGINE_START1_SENSE_AIN		ADC0_4
#define ENGINE_START2_SENSE_AIN		ADC0_5
#define ALT_IG_SENSE_AIN			ADC0_2
#define PUMP_SENSE_AIN				ADC0_3
#define MOTOR_TEMP_AIN				ADC1_11
#define OIL_TEMP_AIN				ADC1_10
#define FUEL_LEVEL_AIN				ADC1_8
#define OIL_LEVEL_AIN				ADC1_9

// Digital inputs
#define ALT_L_I						P0_11
#define ALT_P_RPM_I					P0_13
#define MOTOR_WATER_TEMP_I			P1_3
#define MOTOR_OIL_PRESS_I			P1_2

// Digital outputs
#define PUMP_O						P0_10
#define ALT_IG_O					P0_18
#define AC_O						P0_28
#define ENGINE_START_1_O			P1_1
#define ENGINE_START_2_O			P0_9
#define GLOW_PLUGS_O				P0_27
#define STARTER_O					P0_26



#endif /* UW_MB_SRC_PIN_MAPPINGS_H_ */
