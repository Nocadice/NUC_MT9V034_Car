/*
 * Determination.h
 *
 *  Created on: 2023Äê7ÔÂ4ÈÕ
 *      Author: 17793
 */

#ifndef CODE_DETERMINATION_H_
#define CODE_DETERMINATION_H_

#include "zf_common_headfile.h"

extern float Rsl;
extern float Rsr;
extern boolean straight_flag;
extern boolean curve_flag;
extern boolean crossing_flag;
extern boolean start_line_flag;
extern boolean Brake;

void D_road();
void judge_road();
void check_start_fing_line(void);
#endif /* CODE_DETERMINATION_H_ */
