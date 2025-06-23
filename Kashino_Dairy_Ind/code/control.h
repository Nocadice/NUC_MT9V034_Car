/*
 * control.h
 *
 *  Created on: Dec 27, 2023
 *      Author: 17793
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

#include "zf_common_headfile.h"

#define MOTORR_DIR   P02_4                  //定义右电机方向控制引脚P02_4
#define MOTORR_PWM   ATOM0_CH5_P02_5        //定义右电机PWM引脚ATOM0_CH5_P02_5
#define MOTORL_DIR   P02_6                  //定义左电机方向控制引脚P02_6
#define MOTORL_PWM   ATOM0_CH7_P02_7        //定义左电机PWM引脚ATOM0_CH7_P02_7

#define RIGHT_ENCODER_DIR                    (TIM6_ENCODER)                         // 4右带方向编码器对应使用的编码器接口
#define RIGHT_ENCODER_DIR_PULSE              (TIM6_ENCODER_CH1_P20_3)               // PULSE 对应的引脚
#define RIGHT_ENCODER_DIR_DIR                (TIM6_ENCODER_CH2_P20_0)               // DIR 对应的引脚

#define LEFT_ENCODER_DIR                     (TIM5_ENCODER)                         // 2左带方向编码器对应使用的编码器接口
#define LEFT_ENCODER_DIR_PULSE               (TIM5_ENCODER_CH1_P10_3)               // PULSE 对应的引脚
#define LEFT_ENCODER_DIR_DIR                 (TIM5_ENCODER_CH2_P10_1)               // DIR 对应的引脚

#define SERVO_PIN                                (ATOM1_CH1_P33_9)

extern int32 right_encoder_exp_dir;
extern int32 left_encoder_exp_dir;

extern int32 right_encoder_dir;
extern int32 left_encoder_dir;

extern int16 gyro_z;
extern float gyro_angle_z;   //小车偏航角

void motor_driver(int32 right,int32 left); //电机驱动函数
void motor_init();

#endif /* CODE_CONTROL_H_ */
