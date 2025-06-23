/*
 * pid.h
 *
 *  Created on: 2024年3月10日
 *      Author: 17793
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_


#include "zf_common_headfile.h"

#define KP 0
#define KI 1
#define KD 2
#define KD1 3
#define KT 4
#define Max_Speed 8000             //PWM MAX
#define MAX_ECD 1500               //theory ECD_MAX
#define MIN_ECD 750               //theory ECD_MIN
#define SERVO_CENTER  2940



typedef struct PID
{
    int32 LastError;
    int32 PrevError;
} PID;


extern PID center_PID, left_PID, right_PID, servo_PID;    //定义差速和电机的PID参数结构体

extern float MOTOR[5];   //声明存放pid参数数组
//extern float CENTER[5];
extern float TURN[5];
extern int16 basic_speed;
extern float Kp1;
extern int32 Increase_left;
extern int32 Increase_right;



void IncPID_Init(PID *sptr);
void Servo_pid_Init(PID *sptr);      //舵机pid初始化
void Servo_Pid(PID *sptr);      //舵机pid，值返回到结构体里的All_Out
extern int32 R_I,R_D,L_I,L_D;
int32 right_motor_PI(PID *sptr, int32 ActualSpeed, int32 SetSpeed);
int32 left_motor_PI(PID *sptr, int32 ActualSpeed, int32 SetSpeed);

#endif /* CODE_PID_H_ */
