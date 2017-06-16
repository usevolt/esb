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

#include <uv_utilities.h>
#include <uv_rtos.h>

dev_st dev;
#define this ((dev_st*) me)



void init(dev_st* me) {

	// initialize outputs

	// initialize inputs
	UV_GPIO_INIT_INPUT(ALT_L_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(ALT_P_RPM_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(MOTOR_WATER_TEMP_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(MOTOR_OIL_PRESS_I, PULL_UP_ENABLED);
	UV_GPIO_INIT_INPUT(EBER_FAN_I, PULL_UP_DISABLED);

	uv_moving_aver_init(&this->motor_temp, MOTOR_TEMP_AVG_COUNT);
	uv_hysteresis_init(&this->motor_temp_warn, MOTOR_TEMP_WARN_VALUE,
			MOTOR_TEMP_HYSTERESIS, false);
	uv_hysteresis_init(&this->motor_temp_err, MOTOR_TEMP_ERR_VALUE,
			MOTOR_TEMP_HYSTERESIS, false);

	uv_moving_aver_init(&this->hydroil_temp, HYDROIL_TEMP_AVG_COUNT);
	uv_hysteresis_init(&this->hydroil_temp_warn, HYDROIL_TEMP_WARN_VALUE,
			HYDROIL_TEMP_HYSTERESIS, false);
	uv_hysteresis_init(&this->hydroil_temp_err, HYDROIL_TEMP_ERR_VALUE,
			HYDROIL_TEMP_HYSTERESIS, false);

	uv_moving_aver_init(&this->fuel_level, FUEL_LEVEL_AVG_COUNT);
	uv_hysteresis_init(&this->fuel_level_warn, FUEL_LEVEL_WARN_VALUE,
			FUEL_LEVEL_HYSTERESIS, true);
	uv_hysteresis_init(&this->fuel_level_err, FUEL_LEVEL_ERR_VALUE,
			FUEL_LEVEL_HYSTERESIS, true);


	//init terminal and pass application terminal commands array as a parameter
	uv_terminal_init(terminal_commands, commands_size());

	// load non-volatile data
	if (uv_memory_load()) {
		// initialize default settings

		uv_memory_save();
	}

}


/// @brief: Returns the ADC's read value as celsius degrees
int8_t adc_get_temp(const uv_adc_channels_e adc_chn) {
	int32_t adc = uv_adc_read(adc_chn);
	return adc;
}

/// @brief: Reads the adc channel and converts it to liquid level percentage
uint8_t adc_get_level(const uv_adc_channels_e adc_chn) {
	int32_t adc = uv_adc_read(adc_chn);
	return adc;
}

/// @brief: Reads the adc channel and returns the current in milliamps
///
/// @param sense_mohm: The resistance of current sense resistor in milliohms
uint16_t adc_get_current(const uv_adc_channels_e adc_chn, const uint16_t sense_mohm) {
	int32_t adc = uv_adc_read(adc_chn);
	return adc;
}


void step(void* me) {

	init(this);

	while (true) {
		unsigned int step_ms = 20;
		// update watchdog timer value to prevent a hard reset
		// uw_wdt_update();

		// motor temperature
		uv_moving_aver_step(&this->motor_temp, adc_get_temp(MOTOR_TEMP_AIN));
		uv_hysteresis_step(&this->motor_temp_warn, uv_moving_aver_get_val(&this->motor_temp));
		uv_hysteresis_step(&this->motor_temp_err, uv_moving_aver_get_val(&this->motor_temp));

		// hydraulic oil temperature
		uv_moving_aver_step(&this->hydroil_temp, adc_get_temp(HYDR_OIL_TEMP_AIN));
		uv_hysteresis_step(&this->hydroil_temp_warn, uv_moving_aver_get_val(&this->hydroil_temp));
		uv_hysteresis_step(&this->hydroil_temp_err, uv_moving_aver_get_val(&this->hydroil_temp));

		// fuel level
		uv_moving_aver_step(&this->fuel_level, adc_get_level(FUEL_LEVEL_AIN));
		uv_hysteresis_step(&this->fuel_level_warn, uv_moving_aver_get_val(&this->fuel_level));
		uv_hysteresis_step(&this->fuel_level_err, uv_moving_aver_get_val(&this->fuel_level));

		// terminal step function
		uv_terminal_step();

		uv_rtos_task_delay(step_ms);
	}
}






int main(void) {

	// init the watchdog timer
//	uw_wdt_init(5);

	uv_init(&dev);

	uv_rtos_task_create(step, "step", UV_RTOS_MIN_STACK_SIZE * 5,
			&dev, UV_RTOS_IDLE_PRIORITY + 3, NULL);


	uv_rtos_start_scheduler();


    // Enter an infinite loop
	// never should end up here
    while(1) {
    }
    return 0;
}
