/*
 * control.c
 *
 *  Created on: Dec 27, 2023
 *      Author: 17793
 */

#include "control.h"

//**************小车偏航角*****************
int16 gyro_z = 0;
float gyro_angle_z = 0;

////**************编码器预期值*****************
int32 right_encoder_exp_dir = 0;
int32 left_encoder_exp_dir = 0;
//

//**************编码器实际值*****************
int32 right_encoder_dir = 0;
int32 left_encoder_dir = 0;

void motor_init(void)
{
    right_encoder_exp_dir = basic_speed;
    left_encoder_exp_dir = basic_speed;

    //电机PWM引脚初始化
    pwm_init(MOTORR_PWM, 14000,  basic_speed);         //这里的duty占空比推荐使用变量uint16 dianji_duty;
    pwm_init(MOTORL_PWM, 14000,  basic_speed);

    gpio_set_level(MOTORR_DIR, 0);
    gpio_set_level(MOTORL_DIR, 0);
    //电机方向控制引脚初始化
    gpio_init(MOTORR_DIR, GPO, 0, GPO_PUSH_PULL);
    gpio_init(MOTORL_DIR, GPO, 0, GPO_PUSH_PULL);

    encoder_dir_init(RIGHT_ENCODER_DIR, RIGHT_ENCODER_DIR_PULSE, RIGHT_ENCODER_DIR_DIR);         // 初始化右编码器模块与引脚 带方向增量编码器模式
    encoder_dir_init(LEFT_ENCODER_DIR,  LEFT_ENCODER_DIR_PULSE,  LEFT_ENCODER_DIR_DIR);          // 初始化左编码器模块与引脚 带方向增量编码器模式

}

void motor_driver(int32 right,int32 left)  //电机驱动函数
{
    if(straight_flag) basic_speed=Straight_s;
    else basic_speed=Set_speed;
    if(right >= 0)
    {
        pwm_set_duty(MOTORR_PWM,right);    //用的是pwm占空比设置引脚，而不是那个初始化pwm引脚函数
        gpio_set_level(MOTORR_DIR, 0);
    }
    else
    {
        pwm_set_duty(MOTORR_PWM,-right);
        gpio_set_level(MOTORR_DIR, 1);
    }

    if(left >= 0)
    {
        pwm_set_duty(MOTORL_PWM,left);
        gpio_set_level(MOTORL_DIR, 0);
    }
    else
    {
        pwm_set_duty(MOTORL_PWM,-left);
        gpio_set_level(MOTORL_DIR, 1);
    }
}
