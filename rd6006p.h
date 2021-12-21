/*
 * rd6006p.h
 *
 *  Created on: 21 дек. 2021 г.
 *      Author: serge78rus
 */

#ifndef RD6006P_H_
#define RD6006P_H_

#include <stdbool.h>

bool rd6006p_open(const char *device, int slave_addr);
void rd6006p_close(void);

#endif /* RD6006P_H_ */
