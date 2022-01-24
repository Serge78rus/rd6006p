/*
 * main.c
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#include <stdlib.h>
#include <stdio.h>

#include "rd6006p.h"
#include "errmsg.h"
#include "options.h"

int main(int argc, char **argv)
{
	//printf("begin\n");

	Options *options = options_parse(argc, argv);
	if (!options) {
		ERR_MSG("Parsing command line error");
		options_help();
		return 1;
	}
	if (options->help_flag) {
		options_help();
		return 0;
	}
	if (options->version_flag) {
		options_version();
		return 0;
	}
	if (options->verbose_flag) {
		options_print();
	}

	// check options
	if (!options->serial_device) {
		ERR_MSG("No communication device specified");
		options_help();
		return 1;
	}

	if (!rd6006p_open(options->serial_device, options->baudrate, options->slave)) {
		ERR_MSG("rd6006_open()");
		rd6006p_close();
		return 1;
	}

	//set voltage and current
	if (options->voltage_set_flag && !options->current_set_flag) {
		if (!rd6006p_set_voltage(options->voltage_set_value)) {
			ERR_MSG("rd6006p_set_voltage()");
			rd6006p_close();
			return 1;
		}
	} else if (!options->voltage_set_flag && options->current_set_flag) {
		if (!rd6006p_set_current(options->current_set_value)) {
			ERR_MSG("rd6006p_set_current()");
			rd6006p_close();
			return 1;
		}
	} else if (options->voltage_set_flag && options->current_set_flag) {
		if (!rd6006p_set_voltage_current(options->voltage_set_value, options->current_set_value)) {
			ERR_MSG("rd6006p_set_voltage_current()");
			rd6006p_close();
			return 1;
		}
	}

	//set output on/off
	if (options->output_on_off_flag) {
		if (!rd6006p_set_output(options->output_on_off_value)) {
			ERR_MSG("rd6006p_set_output()");
			rd6006p_close();
			return 1;
		}
	}

	//
	//
	//


	rd6006p_Status *status = rd6006p_get_status();
	if (!status) {
		ERR_MSG("rd6006p_get_status()");
		rd6006p_close();
		return 1;
	}
	printf("M:%s O:%s V:%.3fV C:%.4fA\n",
			status->mode ? "CC" : "CV",
			status->output ? "ON" : "OFF",
			status->voltage,
			status->current
	);

	//printf("end\n");

	rd6006p_close();
	return 0;
}
