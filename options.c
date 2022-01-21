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

		{0, 0, 0, 0}
};

static Options options = {
		.help_flag = false,
		.version_flag = false,
		.verbose_flag = false,
		.serial_device = 0,
		.baudrate = 115200,
		.slave = 1
};

/*
 * public functions
 */

Options* options_parse(int argc, char **argv)
{
	for (;;) {
		int c = getopt_long(argc, argv, "hVd:b:s:", long_options, 0/*&option_index*/);

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
			"help:     %s\n"
			"version:  %s\n"
			"verbose:  %s\n"
			"device:   %s\n"
			"baudrate: %lu\n"
			"slave:    %u\n"

			"\n",

			options.help_flag ? "true" : "false",
			options.version_flag ? "true" : "false",
			options.verbose_flag ? "true" : "false",
			options.serial_device,
			options.baudrate,
			options.slave
	);
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
			"\n"
	);
}

void options_version(void)
{
	printf(APP_NAME" version "VERSION" by Serge L. Ryadkow (aka Serge78rus), GPL-3.0 License\n");
}




