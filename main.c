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
	printf("begin\n");

	if (!rd6006p_open("/dev/ttyUSB0", 1)) { //TODO device & addr from options
		ERR_MSG("rd6006_open()");
		rd6006p_close();
		return 1;
	}

	//
	//
	//



	printf("end\n");

	rd6006p_close();
	return 0;
}
