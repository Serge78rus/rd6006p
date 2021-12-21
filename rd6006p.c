/*
 * rd6006p.c
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#include <modbus/modbus.h>

#include "rd6006p.h"
#include "errmsg.h"

static modbus_t *ctx = 0;

bool rd6006p_open(const char *device, int slave_addr)
{
	ctx = modbus_new_rtu(device, 115200, 'N', 8, 1);
	if (!ctx) {
	    ERR_MSG_F("Failed to create the context: %s\n", modbus_strerror(errno));
	    return false;
	}

	//modbus_set_debug(ctx, TRUE);

	if (modbus_connect(ctx) == -1) {
	    ERR_MSG_F("Unable to connect: %s\n", modbus_strerror(errno));
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
