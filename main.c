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

int main(int argc, char **argv)
{
	//printf("begin\n");

	if (!rd6006p_open("/dev/ttyUSB0", 1)) { //TODO device & addr from options
		ERR_MSG("rd6006_open()");
		rd6006p_close();
		return 1;
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
