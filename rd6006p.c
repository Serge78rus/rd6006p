/*
 * rd6006p.c
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#include <stdint.h>
#include <modbus/modbus.h>

#include "rd6006p.h"
#include "errmsg.h"

#define NUM_REG 100 /*TODO set real value*/
#define MODBUS_ATTEMPTS 3

//private functions
static inline bool read(int first_reg, int num_reg);
static inline bool write(int first_reg, int num_reg);

//private variables
static bool verbose;
static modbus_t *ctx = 0;
static uint16_t reg[NUM_REG];
static rd6006p_Status status;
static rd6006p_Info info;

bool rd6006p_open(const char *device, unsigned long baudrate, unsigned int slave_addr, bool verbose_flag)
{
	verbose = verbose_flag;

	ctx = modbus_new_rtu(device, baudrate, 'N', 8, 1);
	if (!ctx) {
	    ERR_MSG_F("Failed to create MODBUS RTU context: %s", modbus_strerror(errno));
	    return false;
	}

	//modbus_set_debug(ctx, TRUE);

	if (modbus_connect(ctx) == -1) {
	    ERR_MSG_F("Failed connect to device %s: %s", device, modbus_strerror(errno));
	    return false;
	}

	if (modbus_set_slave(ctx, slave_addr) == -1) {
	    ERR_MSG_F("Failed set slave %u: %s", slave_addr, modbus_strerror(errno));
	    return false;
	}

	////////////////////////
	/*
	uint32_t rto_sec;
	uint32_t rto_usec;
	uint32_t bto_sec;
	uint32_t bto_usec;
	modbus_get_response_timeout(ctx, &rto_sec, &rto_usec);
	modbus_get_byte_timeout(ctx, &bto_sec, &bto_usec);
	printf("rto_sec: %u rto_usec: %u bto_sec: %u bto_usec: %u\n", rto_sec, rto_usec, bto_sec, bto_usec);
	modbus_set_response_timeout(ctx, 1, 0);
	modbus_get_response_timeout(ctx, &rto_sec, &rto_usec);
	printf("rto_sec: %u rto_usec: %u\n", rto_sec, rto_usec);
	*/

	//modbus_set_response_timeout(ctx, 1, 0);
	//modbus_set_response_timeout(ctx, 0, 400000);
	//modbus_set_byte_timeout(ctx, 0, 10000);
	//TODO
	//modbus_set_byte_timeout(); //TODO

	return true;
}

void rd6006p_close(void)
{
	if (ctx) {
		//modbus_close(); //TODO
	    modbus_free(ctx);
	}
}

rd6006p_Status* rd6006p_get_status(void)
{
	if (!read(10, 9)) {
		ERR_MSG("Failed read()");
		return 0;
	}

	status.voltage = (double)(reg[0]) / 1000.0;
	status.current = (double)(reg[1]) / 10000.0;
	status.mode = reg[7] ? rd6006p_MODE_CC : rd6006p_MODE_CV;
	status.output = reg[8] ? rd6006p_OUTPUT_ON : rd6006p_OUTPUT_OFF;

	return &status;
}

bool rd6006p_set_voltage(double voltage)
{
	reg[0] = (uint16_t)(voltage * 1000.0);
	return write(8, 1);
}

bool rd6006p_set_current(double current)
{
	reg[0] = (uint16_t)(current * 10000.0);
	return write(9, 1);
}

bool rd6006p_set_voltage_current(double voltage, double current)
{
	reg[0] = (uint16_t)(voltage * 1000.0);
	reg[1] = (uint16_t)(current * 10000.0);
	return write(8, 2);
}

bool rd6006p_set_output(bool on)
{
	reg[0] = on ? 1 : 0;
	return write(18, 1);
}

rd6006p_Info* rd6006p_get_info(void)
{
	if (!read(0, 6)) {
		ERR_MSG("Failed read()");
		return 0;
	}

	//printf("reg[0]: %u", reg[0]);
	switch ((rd6006p_Type)reg[0]) {
	case rd6006p_TYPE_RD6006P:
		info.type = (rd6006p_Type)reg[0];
		break;
	default:
		info.type = rd6006p_TYPE_UNKNOWN;
	}

	//printf("reg[1]: %u", reg[1]);
	//printf("reg[2]: %u", reg[2]);
	info.serial = ((unsigned)reg[1] << 16) | (unsigned)reg[2];

	//printf("reg[3]: %u", reg[3]);
	info.fw_version_hi = reg[3] / 100;
	info.fw_version_lo = reg[3] % 100;

	//printf("reg[4]: %u", reg[4]);
	//printf("reg[5]: %u", reg[5]);
	info.temperature = reg[4] ? -(int)reg[5] : (int)reg[5];

	//TODO

	return &info;
}

/*
 * private functions
 */

static inline bool read(int first_reg, int num_reg)
{
	for (int i = 0; i < MODBUS_ATTEMPTS; ++i) {
		int num = modbus_read_registers(ctx, first_reg, num_reg, reg);
		if (num == num_reg) {
			return true;
		} else if (verbose) { // invalid number of read registers
		    ERR_MSG_F("modbus_read_registers() returned %i expected %i attempt %i : %s",
		    		num, num_reg, i + 1, modbus_strerror(errno));
		}
	}
	return false;
}

static inline bool write(int first_reg, int num_reg)
{
	for (int i = 0; i < MODBUS_ATTEMPTS; ++i) {
		int num = modbus_write_registers(ctx, first_reg, num_reg, reg);
		if (num == num_reg) {
			return true;
		} else if (verbose) { // invalid number of write registers
		    ERR_MSG_F("modbus_write_registers() returned %i expected %i attempt %i : %s",
		    		num, num_reg, i + 1, modbus_strerror(errno));
		}
	}
	return false;
}


