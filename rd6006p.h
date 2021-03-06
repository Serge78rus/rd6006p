/*
 * rd6006p.h
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#ifndef RD6006P_H_
#define RD6006P_H_

#include <stdbool.h>
#include <stdint.h>

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

typedef enum {
	rd6006p_TYPE_UNKNOWN	= 0,
	rd6006p_TYPE_RD6006P	= 60065
} rd6006p_Type;

typedef struct {
	rd6006p_Type type;
	uint32_t serial;
	unsigned int fw_version_hi;
	unsigned int fw_version_lo;
	int temperature;
	//TODO ...
} rd6006p_Info;

bool rd6006p_open(const char *device, unsigned long baudrate, unsigned int slave_addr, bool verbose_flag);
void rd6006p_close(void);
rd6006p_Status* rd6006p_get_status(void);
bool rd6006p_set_voltage(double voltage);
bool rd6006p_set_current(double current);
bool rd6006p_set_voltage_current(double voltage, double current);
bool rd6006p_set_output(bool on);
rd6006p_Info* rd6006p_get_info(void);

#endif /* RD6006P_H_ */
