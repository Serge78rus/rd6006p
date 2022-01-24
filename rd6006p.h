/*
 * rd6006p.h
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#ifndef RD6006P_H_
#define RD6006P_H_

#include <stdbool.h>

typedef enum {
	rd6006p_MODE_CV		= 0,
	rd6006p_MODE_CC		= 1
} rd6006p_Mode;

typedef enum {
	rd6006p_OUTPUT_OFF	= 0,
	rd6006p_OUTPUT_ON	= 1
} rd6006p_Output;

typedef struct {
	rd6006p_Mode		mode;
	rd6006p_Output		output;
	double				voltage;
	double				current;
} rd6006p_Status;

bool rd6006p_open(const char *device, unsigned long baudrate, unsigned int slave_addr);
void rd6006p_close(void);
rd6006p_Status* rd6006p_get_status(void);
bool rd6006p_set_voltage(double voltage);
bool rd6006p_set_current(double current);
bool rd6006p_set_voltage_current(double voltage, double current);
bool rd6006p_set_output(bool on);

#endif /* RD6006P_H_ */
