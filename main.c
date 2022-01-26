/*
 * main.c
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
//#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "rd6006p.h"
#include "errmsg.h"
#include "options.h"

//private variables
static volatile bool keep_running = true;

//private functions
static void int_handler(int dummy);
static inline void show_seconds(struct timeval *time);
//static inline void show_time(const char *format, bool milliseconds);

int main(int argc, char **argv)
{
	//printf("begin\n");

	signal(SIGINT, int_handler);

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

	//show status cycle
	unsigned int cycle_us = options->cycle_s * 1000000;
	int cycle_count = 0;
	while (keep_running) {

		static struct timeval start_time;
	    gettimeofday(&start_time , NULL);

	    //if (options->seconds_flag) {
		    show_seconds(&start_time);
	    //}

	    //////
		rd6006p_Status *status = rd6006p_get_status();
		if (status) {
			printf("%s %.3f %.4f\n",
					status->output ? (status->mode ? "CC" : "CV") : "OFF",
					status->voltage,
					status->current
			);
		} else {
			ERR_MSG("rd6006p_get_status()");
		}
		////////////////

		if (options->cycles_number) {
			if (++cycle_count == options->cycles_number) {
				break;
			}
		}

	    static struct timeval end_time;
	    gettimeofday(&end_time , NULL);

	    unsigned int cycle_corrections = (end_time.tv_sec - start_time.tv_sec) * 1000000;
	    cycle_corrections = (end_time.tv_usec + cycle_corrections) - start_time.tv_usec;
	    if (cycle_us > cycle_corrections) {
			usleep(cycle_us - cycle_corrections);
	    }

	}

	rd6006p_close();
	return 0;
}

/*
 * private functions
 */

static void int_handler(int dummy)
{
    keep_running = false;
}

static inline void show_seconds(struct timeval *time)
{
	static bool first_cycle = true;
    static struct timeval now_time;
    static struct timeval start_time;

    now_time.tv_sec = time->tv_sec;
    now_time.tv_usec = time->tv_usec;

    if (first_cycle) {
    	first_cycle = false;
        start_time.tv_sec = time->tv_sec;
        start_time.tv_usec = time->tv_usec;
    }

    if (now_time.tv_usec < start_time.tv_usec) {
    	now_time.tv_sec -= 1;
    	now_time.tv_usec += 1000000;
    }
	fprintf(stdout, "%u.%03u ", (unsigned int)(now_time.tv_sec - start_time.tv_sec),
			(unsigned int)(now_time.tv_usec - start_time.tv_usec) / 1000);
}



