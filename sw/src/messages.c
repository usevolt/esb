/*
 * uw_messages.c
 *
 *  Created on: Feb 19, 2015
 *      Author: usenius
 */


#include "main.h"
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uv_timer.h>
#include <uv_utilities.h>

extern dev_st dev;





canopen_object_st obj_dict[] = {
};

int obj_dict_len() {
	return sizeof(obj_dict) / sizeof(canopen_object_st);
}





const uv_command_st terminal_commands[] = {
		{
				.id = CMD_STAT,
				.str = "stat",
				.instructions = "Displays the system status information.\n"
						"Usage: stat",
				.callback = &stat_callb
		},
		{
				.id = CMD_ECHO,
				.str = "echo",
				.instructions = "Continuously echoes status to stdout.\n"
						"Usage: echo <1/0>",
				.callback = &echo_callb
		}
};




unsigned int commands_size(void) {
	return sizeof(terminal_commands) / sizeof(uv_command_st);
}




#define this ((dev_st*)me)






void stat_callb(void* me, unsigned int cmd, unsigned int args, argument_st *argv) {
	printf("SYSTEM STATUS:\nMotor temperature: %i C (%u %u)\nHydraulic oil temperature: %i C (%u %u)\n"
			"Fuel level: %u %% (%u %u)\n",
			(int) uv_moving_aver_get_val(&this->motor_temp),
			uv_hysteresis_get_output(&this->motor_temp_warn),
			uv_hysteresis_get_output(&this->motor_temp_err),
			(int) uv_moving_aver_get_val(&this->hydroil_temp),
			uv_hysteresis_get_output(&this->hydroil_temp_warn),
			uv_hysteresis_get_output(&this->hydroil_temp_err),
			(int) uv_moving_aver_get_val(&this->fuel_level),
			uv_hysteresis_get_output(&this->fuel_level_warn),
			uv_hysteresis_get_output(&this->fuel_level_err));
}

void echo_callb(void* me, unsigned int cmd, unsigned int args, argument_st *argv) {
	if (args && argv[0].type == ARG_INTEGER) {
		this->echo = argv[0].number;
	}
	printf("echo: %u\n", this->echo);
}





