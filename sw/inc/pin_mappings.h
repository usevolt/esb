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
#define VBAT_SENSE_AIN				ADC0_11
#define VDD_SENSE_AIN				ADC0_10
#define GLOW_PLUGS1_SENSE_AIN		ADC0_9
#define STARTE_SENSE_AIN			ADC0_8
#define CRANE_LIGHT_SENSE_AIN		ADC0_7
#define GLOW_PLUGS2_SENSE_AIN		ADC0_6
#define MOTOR_ON_SENSE_AIN			ADC0_5
#define MOTOR_START_SENSE_AIN		ADC0_4
#define ALT_IG_SENSE_AIN			ADC0_3
#define PUMP_SENSE_AIN				ADC0_2
#define MOTOR_TEMP_AIN				ADC1_6
#define HYDR_OIL_TEMP_AIN			ADC1_7
#define FUEL_LEVEL_AIN				ADC1_9

// Digital inputs
#define ALT_L_I						P1_4
#define ALT_P_RPM_I					P0_11
#define MOTOR_WATER_TEMP_I			P1_3
#define MOTOR_OIL_PRESS_I			P1_2
#define EBER_FAN_I					P1_5

// Digital outputs
#define PUMP_O						P0_7
#define ALT_IG_O					P0_8
#define MOTOR_ON_O					P1_1
#define MOTOR_START_O				P0_9
#define GLOW_PLUGS1_O				P0_24
#define GLOW_PLUGS2_O				P0_25
#define STARTER_O					P1_10
#define CRANE_LIGHT_O				P0_26



#endif /* UW_MB_SRC_PIN_MAPPINGS_H_ */
