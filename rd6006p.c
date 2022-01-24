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

//private functions
static inline bool read(int first_reg, int num_reg);
static inline bool write(int first_reg, int num_reg);

//private variables
static modbus_t *ctx = 0;
static uint16_t reg[NUM_REG];
static rd6006p_Status status;

bool rd6006p_open(const char *device, unsigned long baudrate, unsigned int slave_addr)
{
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

	modbus_set_slave(ctx, slave_addr); //TODO check result

	//TODO
	//modbus_set_response_timeout(); //TODO
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

/*
 * private functions
 */

static inline bool read(int first_reg, int num_reg)
{
	int num = modbus_read_registers(ctx, first_reg, num_reg, reg);
	if (num != num_reg) { // invalid number of read registers
	    ERR_MSG_F("Failed modbus_read_registers(): %s", modbus_strerror(errno));
		return false;
	}
	return true;
}

static inline bool write(int first_reg, int num_reg)
{
	int num = modbus_write_registers(ctx, first_reg, num_reg, reg);
	if (num != num_reg) { // invalid number of write registers
	    ERR_MSG_F("Failed modbus_write_registers(): %s", modbus_strerror(errno));
		return false;
	}
	return true;
}



