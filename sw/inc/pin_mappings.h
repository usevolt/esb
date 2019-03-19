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


// Analog inputs
#define VDD_SENSE_AIN				ADC1_11
#define GLOW_SENSE_AIN				ADC0_3
#define STARTER_SENSE_AIN			ADC0_5
#define AC_SENSE_AIN				ADC0_7
#define ENGINE_START1_SENSE_AIN		ADC0_11
#define ENGINE_START2_SENSE_AIN		ADC0_10
#define PUMP_SENSE_AIN				ADC1_9
#define ALT_IG_SENSE_AIN			ADC1_8
#define MOTOR_TEMP_AIN				ADC1_2
#define OIL_TEMP_AIN				ADC1_1
#define OIL_LEVEL_AIN				ADC1_0
#define OILCOOLER_AIN				ADC1_10
#define RADIATOR_AIN				ADC1_4

// Digital inputs
#define ALT_L_I						P0_18
#define ALT_P_RPM_I					P0_6
#define MOTOR_WATER_TEMP_I			P0_8
#define MOTOR_OIL_PRESS_I			P0_7

// Digital outputs
#define PUMP_PWM					PWM0_0
#define PUMP_PWM_IO					P0_13
#define ALT_IG_O					P0_14
#define AC_O						P1_0
#define ENGINE_START_1_O			P0_28
#define ENGINE_START_2_O			P0_29
#define GLOW_PLUGS_O				P0_4
#define STARTER_O					P0_2
#define OILCOOLER_O					P1_7
#define RADIATOR_O					P0_12

// RPM timer
#define RPM_TIMER					TIMER1

#endif /* UW_MB_SRC_PIN_MAPPINGS_H_ */
