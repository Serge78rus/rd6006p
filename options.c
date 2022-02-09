/*
 * options.c
 *
 *  Created on: 20 янв. 2022 г.
 *      Author: serge78rus
 */

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "options.h"
#include "errmsg.h"
#include "def.h"

//const for long options
#define VERBOSE		1

/*
 * local variables
 */

static struct option long_options[] = {
	    {.name = "help", 			.has_arg = no_argument, 		.flag = 0, .val = 'h'},
	    {.name = "version",			.has_arg = no_argument, 		.flag = 0, .val = 'V'},
	    {.name = "verbose", 		.has_arg = no_argument, 		.flag = 0, .val = VERBOSE},
	    {.name = "device", 			.has_arg = required_argument,	.flag = 0, .val = 'd'},
	    {.name = "baudrate",		.has_arg = required_argument,	.flag = 0, .val = 'b'},
	    {.name = "slave",		    .has_arg = required_argument,	.flag = 0, .val = 's'},
	    {.name = "voltage",		    .has_arg = required_argument,	.flag = 0, .val = 'v'},
	    {.name = "current",		    .has_arg = required_argument,	.flag = 0, .val = 'c'},
	    {.name = "output",		    .has_arg = required_argument,	.flag = 0, .val = 'o'},
	    {.name = "cycle", 			.has_arg = required_argument,	.flag = 0, .val = 'C'},
	    {.name = "number", 			.has_arg = required_argument,	.flag = 0, .val = 'n'},

		{0, 0, 0, 0}
};

static Options options = {
		.help_flag = false,
		.version_flag = false,
		.verbose_flag = false,
		.serial_device = 0,
		.baudrate = 115200,
		.slave = 1,
		.voltage_set_flag = false,
		.voltage_set_value = 0,
		.current_set_flag = false,
		.current_set_value = 0,
		.output_on_off_flag = false,
		.output_on_off_value = false,
		.cycle_s = 1.0,
		.cycles_number = 0
};

/*
 * public functions
 */

Options* options_parse(int argc, char **argv)
{
	for (;;) {
		int c = getopt_long(argc, argv, "hVd:b:s:v:c:o:C:n:", long_options, 0/*&option_index*/);

		if (c == -1) {
			break;
		}

		switch (c) {
		case 0:
			ERR_MSG("getopt_long() return 0");
			return 0;

		case 'h':
			options.help_flag = true;
			break;
		case 'V':
			options.version_flag = true;
			break;
		case VERBOSE:
			options.verbose_flag = true;
			break;
		case 'd':
			options.serial_device = optarg;
			break;
		case 'b': {
			char *end_ptr = optarg;
			options.baudrate = strtoul(optarg, &end_ptr, 10);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid unsigned integer value \"%s\" in command line", optarg);
				return 0;
			}
			break;
		}
		case 's': {
			char *end_ptr = optarg;
			options.slave = strtoul(optarg, &end_ptr, 10);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid unsigned integer value \"%s\" in command line", optarg);
				return 0;
			}
			break;
		}
		case 'v': {
			char *end_ptr = optarg;
			options.voltage_set_value = strtod(optarg, &end_ptr);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid floating point value \"%s\" in command line", optarg);
				return 0;
			}
			options.voltage_set_flag = true;
			break;
		}
		case 'c': {
			char *end_ptr = optarg;
			options.current_set_value = strtod(optarg, &end_ptr);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid floating point value \"%s\" in command line", optarg);
				return 0;
			}
			options.current_set_flag = true;
			break;
		}
		case 'o': {
			if (!strcmp("off", optarg) || !strcmp("0", optarg)) {
				options.output_on_off_value = false;
			} else if (!strcmp("on", optarg) || !strcmp("1", optarg)) {
				options.output_on_off_value = true;
			} else {
				ERR_MSG_F("Invalid output on/off value \"%s\" in command line", optarg);
				return 0;
			}
			options.output_on_off_flag = true;
			break;
		}
		case 'C': {
			char *end_ptr = optarg;
			options.cycle_s = strtod(optarg, &end_ptr);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid floating point value \"%s\" in command line", optarg);
				return 0;
			}
			break;
		}
		case 'n': {
			char *end_ptr = optarg;
			options.cycles_number = strtoul(optarg, &end_ptr, 10);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid unsigned integer value \"%s\" in command line", optarg);
				return 0;
			}
			break;
		}

		default:
			ERR_MSG("Unknown getopt_long() error");
			return 0;
		}
	}

	if (optind < argc) {
		ERR_MSG_F("Unknown command line argument%s", argc - optind > 1 ? "s" : "");
		while (optind < argc) {
			ERR_MSG_F(" %s", argv[optind++]);
		}
		return 0;
	}

	return &options;
}

void options_print(void)
{
	printf("Options:\n"
			"help:          %s\n"
			"version:       %s\n"
			"verbose:       %s\n"
			"device:        %s\n"
			"baudrate:      %lu\n"
			"slave:         %u\n",
			options.help_flag ? "true" : "false",
			options.version_flag ? "true" : "false",
			options.verbose_flag ? "true" : "false",
			options.serial_device,
			options.baudrate,
			options.slave
	);

	if (options.voltage_set_flag) {
		printf("voltage set:   %.3f\n", options.voltage_set_value);
	} else {
		printf("voltage set:   no\n");
	}

	if (options.current_set_flag) {
		printf("current set:   %.4f\n", options.current_set_value);
	} else {
		printf("current set:   no\n");
	}

	printf("output on/off: %s\n"
			"cycle:         %f s\n"
			"cycles_number: %i\n",
			options.output_on_off_flag ? (options.output_on_off_value ? "on" : "off") : "no",
			options.cycle_s,
			options.cycles_number
	);

	printf("\n");
}

void options_help(void)
{
	printf( "\n"
			"Usage: "APP_NAME" options\n"
			"\n"
			"options:\n"
			"\n"
			"-h, --help                       show this help screen and exit\n"
			"-V, --version                    show version information and exit\n"
			"    --verbose          optional  show verbose information\n"
			"                                 (default: false)\n"
			"-d, --device=NAME      required  serial communication device\n"
			"-b, --baudrate=VALUE   optional  communication baudrate\n"
			"                                 (default: 115200)\n"
			"-s, --slave=VALUE      optional  slave address\n"
			"                                 (default: 1)\n"
			"-v, --voltage=VALUE    optional  set output voltage value\n"
			"                                 (default: no set)\n"
			"-c, --current=VALUE    optional  set output current value\n"
			"                                 (default: no set)\n"
			"-o, --output=STATE     optional  set output state\n"
			"                                 valid STATE: 0, off, 1, on\n"
			"                                 (default: no set)\n"
			"-C, --cycle=VALUE      optional  display status cycle in seconds\n"
			"                                 (default: 1.0)\n"
			"-n, --number=VALUE     optional  display status cycles count\n"
			"                                 (default: 0 - no limit)\n"
			"\n"
	);
}

void options_version(void)
{
	printf(APP_NAME" version "VERSION" by Serge L. Ryadkow (aka Serge78rus), GPL-3.0 License\n");
}




