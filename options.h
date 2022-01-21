/*
 * options.h
 *
 *  Created on: 20 янв. 2022 г.
 *      Author: serge78rus
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdbool.h>

/*
 * type definitions
 */

typedef struct {
	bool help_flag;
	bool version_flag;
	bool verbose_flag;
	char *serial_device;
	unsigned long baudrate;
	unsigned int slave;
} Options;

//public functions
Options* options_parse(int argc, char **argv);
void options_print(void);
void options_help(void);
void options_version(void);


#endif /* OPTIONS_H_ */
