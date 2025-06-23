/*
 * scan.c
 *
 *  Created on: 2023/11/16
 *      Author: 17793
 */
#ifndef _Ostu_H
#define _Ostu_H

#include "zf_common_headfile.h"

#define GrayScale 256

extern uint8 image_use[70][188];

uint8 Ostu(uint8 *image, uint16 col, uint16 row);


/********************************************************************/

#endif
