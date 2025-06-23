/*
 * Circle.h
 *
 *  Created on: 2023年5月15日
 *      Author: 17793
 */

#ifndef CODE_CIRCLE_H_
#define CODE_CIRCLE_H_


#include "zf_common_headfile.h"

void fix_right(float k,float x,float y,uint8 S,uint8 E);
void fix_left(float k,float x,float y,uint8 S,uint8 E);
void circle(void);

extern boolean RC;
extern int16 IN_A_R;
extern int16 OUT_A_R;           //出入角度


#endif /* CODE_CIRCLE_H_ */
