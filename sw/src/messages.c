/*
 * uw_messages.c
 *
 *  Created on: Feb 19, 2015
 *      Author: usenius
 */


#include "main.h"
#include "messages.h"
#include "can_esb.h"
#include "can_fsb.h"
#include "pin_mappings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uv_timer.h>
#include <uv_utilities.h>

extern dev_st dev;
#define this (&dev)




canopen_object_st obj_dict[] = {
		{
				.main_index = ESB_TOTAL_CURRENT_INDEX,
				.sub_index = ESB_TOTAL_CURRENT_SUBINDEX,
				.type = ESB_TOTAL_CURRENT_TYPE,
				.permissions = ESB_TOTAL_CURRENT_PERMISSIONS,
				.data_ptr = &this->total_current
		},
		{
				.main_index = ESB_GLOW_STATUS_INDEX,
				.sub_index = ESB_GLOW_STATUS_SUBINDEX,
				.type = ESB_GLOW_STATUS_TYPE,
				.permissions = ESB_GLOW_STATUS_PERMISSIONS,
				.data_ptr = &this->glow.state
		},
		{
				.main_index = ESB_GLOW_CURRENT_INDEX,
				.sub_index = ESB_GLOW_CURRENT_SUBINDEX,
				.type = ESB_GLOW_CURRENT_TYPE,
				.permissions = ESB_GLOW_CURRENT_PERMISSIONS,
				.data_ptr = &this->glow.current
		},
		{
				.main_index = ESB_STARTER_STATUS_INDEX,
				.sub_index = ESB_STARTER_STATUS_SUBINDEX,
				.type = ESB_STARTER_STATUS_TYPE,
				.permissions = ESB_STARTER_STATUS_PERMISSIONS,
				.data_ptr = &this->starter.state
		},
		{
				.main_index = ESB_STARTER_CURRENT_INDEX,
				.sub_index = ESB_STARTER_CURRENT_SUBINDEX,
				.type = ESB_STARTER_CURRENT_TYPE,
				.permissions = ESB_STARTER_CURRENT_PERMISSIONS,
				.data_ptr = &this->starter.current
		},
		{
				.main_index = ESB_AC_STATUS_INDEX,
				.sub_index = ESB_AC_STATUS_SUBINDEX,
				.type = ESB_AC_STATUS_TYPE,
				.permissions = ESB_AC_STATUS_PERMISSIONS,
				.data_ptr = &this->ac.state
		},
		{
				.main_index = ESB_AC_CURRENT_INDEX,
				.sub_index = ESB_AC_CURRENT_SUBINDEX,
				.type = ESB_AC_CURRENT_TYPE,
				.permissions = ESB_AC_CURRENT_PERMISSIONS,
				.data_ptr = &this->ac.current
		},
		{
				.main_index = ESB_ENGINE_START1_STATUS_INDEX,
				.sub_index = ESB_ENGINE_START1_STATUS_SUBINDEX,
				.type = ESB_ENGINE_START1_STATUS_TYPE,
				.permissions = ESB_ENGINE_START1_STATUS_PERMISSIONS,
				.data_ptr = &this->engine_start1.state
		},
		{
				.main_index = ESB_ENGINE_START1_CURRENT_INDEX,
				.sub_index = ESB_ENGINE_START1_CURRENT_SUBINDEX,
				.type = ESB_ENGINE_START1_CURRENT_TYPE,
				.permissions = ESB_ENGINE_START1_CURRENT_PERMISSIONS,
				.data_ptr = &this->engine_start1.current
		},
		{
				.main_index = ESB_ENGINE_START2_STATUS_INDEX,
				.sub_index = ESB_ENGINE_START2_STATUS_SUBINDEX,
				.type = ESB_ENGINE_START2_STATUS_TYPE,
				.permissions = ESB_ENGINE_START2_STATUS_PERMISSIONS,
				.data_ptr = &this->engine_start2.state
		},
		{
				.main_index = ESB_ENGINE_START2_CURRENT_INDEX,
				.sub_index = ESB_ENGINE_START2_CURRENT_SUBINDEX,
				.type = ESB_ENGINE_START2_CURRENT_TYPE,
				.permissions = ESB_ENGINE_START2_CURRENT_PERMISSIONS,
				.data_ptr = &this->engine_start2.current
		},
		{
				.main_index = ESB_PUMP_STATUS_INDEX,
				.sub_index = ESB_PUMP_STATUS_SUBINDEX,
				.type = ESB_PUMP_STATUS_TYPE,
				.permissions = ESB_PUMP_STATUS_PERMISSIONS,
				.data_ptr = &this->pump.state
		},
		{
				.main_index = ESB_PUMP_CURRENT_INDEX,
				.sub_index = ESB_PUMP_CURRENT_SUBINDEX,
				.type = ESB_PUMP_CURRENT_TYPE,
				.permissions = ESB_PUMP_CURRENT_PERMISSIONS,
				.data_ptr = &this->pump.current
		},
		{
				.main_index = ESB_ALT_IG_STATUS_INDEX,
				.sub_index = ESB_ALT_IG_STATUS_SUBINDEX,
				.type = ESB_ALT_IG_STATUS_TYPE,
				.permissions = ESB_ALT_IG_STATUS_PERMISSIONS,
				.data_ptr = &this->alt_ig.state
		},
		{
				.main_index = ESB_ALT_IG_CURRENT_INDEX,
				.sub_index = ESB_ALT_IG_CURRENT_SUBINDEX,
				.type = ESB_ALT_IG_CURRENT_TYPE,
				.permissions = ESB_ALT_IG_CURRENT_PERMISSIONS,
				.data_ptr = &this->alt_ig.current
		},

		{
				.main_index = ESB_RPM_INDEX,
				.sub_index = ESB_RPM_SUBINDEX,
				.type = ESB_RPM_TYPE,
				.permissions = ESB_RPM_PERMISSIONS,
				.data_ptr = &this->alt_p_rpm
		},
		{
				.main_index = ESB_ALT_L_INDEX,
				.sub_index = ESB_ALT_L_SUBINDEX,
				.type = ESB_ALT_L_TYPE,
				.permissions = ESB_ALT_L_PERMISSIONS,
				.data_ptr = &this->alt_l
		},
		{
				.main_index = ESB_MOTOR_WATER_TEMP_INDEX,
				.sub_index = ESB_MOTOR_WATER_TEMP_SUBINDEX,
				.type = ESB_MOTOR_WATER_TEMP_TYPE,
				.permissions = ESB_MOTOR_WATER_TEMP_PERMISSIONS,
				.data_ptr = &this->motor_water_temp
		},
		{
				.main_index = ESB_MOTOR_OIL_PRESS_INDEX,
				.sub_index = ESB_MOTOR_OIL_PRESS_SUBINDEX,
				.type = ESB_MOTOR_OIL_PRESS_TYPE,
				.permissions = ESB_MOTOR_OIL_PRESS_PERMISSIONS,
				.data_ptr = &this->motor_oil_press
		},
		{
				.main_index = ESB_MOTOR_TEMP_INDEX,
				.sub_index = ESB_MOTOR_TEMP_SUBINDEX,
				.type = ESB_MOTOR_TEMP_TYPE,
				.permissions = ESB_MOTOR_TEMP_PERMISSIONS,
				.data_ptr = &this->motor_temp.value
		},
		{
				.main_index = ESB_OIL_TEMP_INDEX,
				.sub_index = ESB_OIL_TEMP_SUBINDEX,
				.type = ESB_OIL_TEMP_TYPE,
				.permissions = ESB_OIL_TEMP_PERMISSIONS,
				.data_ptr = &this->oil_temp.value
		},
		{
				.main_index = ESB_OIL_LEVEL_INDEX,
				.sub_index = ESB_OIL_LEVEL_SUBINDEX,
				.type = ESB_OIL_LEVEL_TYPE,
				.permissions = ESB_OIL_LEVEL_PERMISSIONS,
				.data_ptr = &this->oil_level.value
		},
		{
				.main_index = ESB_FUEL_LEVEL_INDEX,
				.sub_index = ESB_FUEL_LEVEL_SUBINDEX,
				.type = ESB_FUEL_LEVEL_TYPE,
				.permissions = ESB_FUEL_LEVEL_PERMISSIONS,
				.data_ptr = &this->fuel_level.value
		},
		{
				.main_index = ESB_VDD_INDEX,
				.sub_index = ESB_VDD_SUBINDEX,
				.type = ESB_VDD_TYPE,
				.permissions = ESB_VDD_PERMISSIONS,
				.data_ptr = &this->vdd
		},
		{
				.main_index = ESB_FSB_TOTAL_CURRENT_INDEX,
				.sub_index = ESB_FSB_TOTAL_CURRENT_SUBINDEX,
				.type = ESB_FSB_TOTAL_CURRENT_TYPE,
				.permissions = ESB_FSB_TOTAL_CURRENT_PERMISSIONS,
				.data_ptr = &this->fsb.total_current
		},
		{
				.main_index = ESB_IGNKEY_INDEX,
				.sub_index = ESB_IGNKEY_SUBINDEX,
				.type = ESB_IGNKEY_TYPE,
				.permissions = ESB_IGNKEY_PERMISSIONS,
				.data_ptr = &this->fsb.ignkey_state
		},
};

int obj_dict_len() {
	return sizeof(obj_dict) / sizeof(canopen_object_st);
}


void engine_callb(void *me, unsigned int cmd, unsigned int args, argument_st *argv);
void pump_callb(void *me, unsigned int cmd, unsigned int args, argument_st *argv);



const uv_command_st terminal_commands[] = {
		{
				.id = CMD_STAT,
				.str = "stat",
				.instructions = "Displays the system status information.\n"
						"Usage: stat",
				.callback = &stat_callb
		},
		{
				.id = CMD_ENGINE,
				.str = "engine",
				.instructions = "Simulates the ignition key positions.\n"
						"0: power off\n"
						"1: power on\n"
						"2: Glow Plugs on\n"
						"3: starter on\n"
						"Usage: engine (0/1/2/3)",
				.callback = &engine_callb
		},
		{
				.id = CMD_PUMP,
				.str = "pump",
				.instructions = "Sets the hydraulic pump output state.\n"
						"Usage: pump (milliamps)",
				.callback = &pump_callb
		}
};




unsigned int commands_size(void) {
	return sizeof(terminal_commands) / sizeof(uv_command_st);
}




static void stat_output(uv_output_st *output, const char *output_name) {
	printf("%s state: %u, current: %u mA\n",
			output_name, uv_output_get_state(output), uv_output_get_current(output));
}


void stat_callb(void* me, unsigned int cmd, unsigned int args, argument_st *argv) {
	printf("SYSTEM STATUS:\n");
	printf("Total current: %u mA\n", (unsigned int) this->total_current);
	printf("Motor temperature: %i C, state: %u\n"
			"Hydraulic oil temperature: %i C, state: %u\n"
			"Fuel level: %u %%, state: %u\n"
			"Oil level: %u %%, state: %u\n",
			sensor_get_value(&this->motor_temp), sensor_get_state(&this->motor_temp),
			sensor_get_value(&this->oil_temp), sensor_get_state(&this->oil_temp),
			sensor_get_value(&this->fuel_level), sensor_get_state(&this->fuel_level),
			sensor_get_value(&this->oil_level), sensor_get_state(&this->oil_level));
	printf("Rpm: %u\nAlt L: %u\n, Motor Water Temp: %u\nMotor Oil Pressure: %u\n",
			this->alt_p_rpm,
			this->alt_l,
			this->motor_water_temp,
			this->motor_oil_press);
	stat_output(&this->glow, "Glow Plugs");
	stat_output(&this->starter, "Starter");
	stat_output(&this->ac, "AC compressor");
	stat_output(&this->engine_start1, "Engine start 1");
	stat_output(&this->engine_start2, "Engine start 2");
	stat_output(&this->pump, "Hydr Pump");
	stat_output(&this->alt_ig, "Alt IG");
	printf("Vdd: %u mV\n", this->vdd);

	printf("motor temp adc: 0x%x / 0x%x\n", uv_adc_read(MOTOR_TEMP_AIN), ADC_MAX_VALUE);
}



void engine_callb(void *me, unsigned int cmd, unsigned int args, argument_st *argv) {
	if ((args == 1) &&
			(argv[0].type == ARG_INTEGER)) {
		printf("engine state: ");
		if (argv[0].number == 0) {
			this->fsb.ignkey_state = FSB_IGNKEY_STATE_OFF;
			printf("OFF\n");
		}
		else if (argv[0].number == 1) {
			this->fsb.ignkey_state = FSB_IGNKEY_STATE_ON;
			printf("ON\n");
		}
		else if (argv[0].number == 2) {
			this->fsb.ignkey_state = FSB_IGNKEY_STATE_PREHEAT;
			printf("PREHEAT\n");
		}
		else if (argv[0].number == 3) {
			this->fsb.ignkey_state = FSB_IGNKEY_STATE_START;
			printf("START\n");
		}
		else {

		}
	}
}



void pump_callb(void *me, unsigned int cmd, unsigned int args, argument_st *argv) {
	if (args && argv[0].type == ARG_INTEGER) {
		uv_output_set_state(&this->pump, argv[0].number);
	}
}




