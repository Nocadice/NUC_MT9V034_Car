/*
 * seedscan.h
 *
 *  Created on: 2023年6月8日
 *      Author: 杜相均
 */

#ifndef CODE_SEEDSCAN_H_
#define CODE_SEEDSCAN_H_

#include "zf_common_headfile.h"


extern uint8 center_line;
extern uint8 center_line_list[MT9V03X_H];

extern uint8 left_line_list[MT9V03X_H+2][50];
extern uint8 right_line_list[MT9V03X_H+2][50];
extern uint8 Rn[75];
extern uint8 Ln[75];
extern uint8 center_line_list[MT9V03X_H];
extern uint8 st_change[70];

extern uint8 left_find_time;
extern uint8 right_find_time;
extern uint8 FAR;


#define MID_W 111

void Find_Center_Line_weight();
void Draw_Points(uint8 x,uint8 y, uint16 color);                  //画点函数，一点周围九格内画出
void Draw_Line(void);
void seeds(void);


#endif /* CODE_SEEDSCAN_H_ */
