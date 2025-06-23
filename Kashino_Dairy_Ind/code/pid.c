 /*
 * pid.c
 *
 *  Created on: 2024年3月10日
 *      Author: 17793
 */
#include "pid.h"
//
//1.采样周期10ms、20ms  （编码器）
//2.测试    2000     ---80/90
//3.内轮减速


//后轮15.5cm，车长20cm左右

/*
   增量式pid
   先定KI，它反映了响应速度
   再整定KP，它反映了对超调量的限制，也就是缓增量式KI参数过大的时候的抖动
   先加大KI，这时候会越来越接近实际速度
   当KI过大的时候，在切换目标速度的时候，就会抖动
   这时候就是KI大了，响应速度高了，但导致超调量增加
   这时候就加大增量式的KP，来缓减抖动，减小超调量

   kd减少震荡
   位置式pid
   P： 响应速度，过小响应速度太慢，基本不震荡，过大响应速度快，高频震荡
   I： 静态 当有静差的时候把I加大点即可清除
   D： 稳态，减小最大超调量，该参数过大会低频震荡

  总结：P用于提高响应速度，I用于减小静差，D用于抑制震荡
  具体参数的整定原则
  一开始先加大比例KP，P小了会达不到目标速度，会差很多
  加大P时，电机出现塔塔的抖动或者上位机的波形剧烈抖动的时候，p就过大了，其实p可以不是特别大

*/

#define MAX_I 60
#define TN 360

PID center_PID, left_PID, right_PID, servo_PID;    //定义舵机和电机的PID参数结构体//   11, 1, 50,135 回环

float DK_IN, DK_OUT, R;                                           //差速比例 & 转弯曲率R

float MOTOR[5]  = {5.4,0.7,0.7,0,170};                      //电机PI     kp   ki   kd XXX kt
float TURN[5] = {6.54,0,-4.5,-0.045,450};                  //舵机PD     kp   kI   kd   kd1  kt

int16 basic_speed = 0;    //pwm
int32 Increase_left =0, Increase_right =0;
int32 R_I,R_D,L_I,L_D;

//增量式PID参数初始化


void IncPID_Init(PID *sptr)
{
    sptr->LastError = 0;    //Error[-1]
    sptr->PrevError = 0;    //Error[-2]
}

void Servo_pid_Init(PID *sptr)
{
    sptr->LastError = 0;    //Error[-1]
    sptr->PrevError = 0;    //Error[-2]
}

int32 integrator(int32 err,int32 Act_S)
{
    register int32 F_I;
    if(abs(err) > 350) return 0;                              //分离
    float D_KI;

    D_KI = (float)(0.003)*abs(err);                     //linear变速
    D_KI = (D_KI>(float)(MOTOR[KI]) ? MOTOR[KI] : D_KI);     //KI限幅
    F_I  = D_KI * err;

    if(F_I >  MAX_I) F_I =  MAX_I;                            //I限幅
    if(F_I < -MAX_I) F_I = -MAX_I;

    if(Act_S > MAX_ECD) return (F_I < 0 ? F_I : 0);             //遇限削弱
    if(Act_S < MIN_ECD) return (F_I > 0 ? F_I : 0);
    return F_I;
}

void Servo_PD(PID *sptr)
{
        register int8 Error;      //当前误差
        register int32 Diff;      // 风扇差速
        register int16 turning;     //最后得出的实际输出值
        float Kp1;                                           //动态P PS:float x int32==float x float

        Error = center_line - MID_W;
        Kp1 = -((Error * Error) / (2 * TURN[KT]) +  TURN[KP]); //P值与差值成二次函数关系
        turning = Kp1 * Error + TURN[KD] * (Error - sptr->LastError) + TURN[KD1] * (imu660ra_gyro_z + 1 );
//   轻微偏右时  error < 0               Kp1 * Error >  0 此时抑制抖动时 imu660ra_gyro_z > 0  需为负值 LPRN
//   轻微偏左时  kp1   < 0   error 大于0  kp1 * error <  0 此时抑制抖动时 imu660ra_gyro_z < 0  需为正值
// TURN[KD1]  这里应该是关于kp1 和 kd 的一个函数 ， 当 kp1 和 kd 都很大时 ，TURN[KD1] 为 接近0

        if(turning<=0)    //左偏右转
        {
            turning=(turning <=-280 ? -280 : turning);
            pwm_set_duty(SERVO_PIN,turning+SERVO_CENTER);
        }
        else   //右偏左转
        {
            turning=(turning >= 280 ? 280 : turning);
            pwm_set_duty(SERVO_PIN,turning+SERVO_CENTER);
        }
        sptr->LastError = Error;
// -------------------------------------Different Motor Speed-----------------------------------
        if(curve_flag)
        {
              R=(float)(84)*powf((float)(abs(turning)),(float)(-0.966));
              DK_IN  = (R - (float)(0.077)) / R;
              DK_OUT = (R + (float)(0.077)) / R;
              if(turning<0)            //R
              {
                   left_encoder_exp_dir = (int32)((float)(basic_speed) * DK_OUT);
                   right_encoder_exp_dir = (int32)((float)(basic_speed) * DK_IN);
                   Diff=(850-MT_duty)*(-turning)/280;
                   MT_Rduty = MT_duty + Diff;
                   MT_Lduty = MT_duty - Diff/2;
              }
              else if(turning>0)      //L
              {
                   left_encoder_exp_dir = (int32)((float)(basic_speed)*DK_IN);
                   right_encoder_exp_dir = (int32)((float)(basic_speed)*DK_OUT);
                   Diff=(850-MT_duty)*(turning)/280;
                   MT_Lduty = MT_duty + Diff;
                   MT_Rduty = MT_duty - Diff/2;
              }
        }
        else
        {
              left_encoder_exp_dir = (basic_speed);
              right_encoder_exp_dir = (basic_speed);
        }
}
// -------------------------------------Different Motor Speed-----------------------------------


int32 left_motor_PI(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
    if(start_line_flag) return -1000;

    //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
    register int32 left_error,out,I,Y;   //当前误差  最后得出的实际增量  积分
    float Kp1;                                           //动态P PS:float x int32==float x float

    left_error = SetSpeed - ActualSpeed;         //计算实际编码器误差
    Kp1 = (((left_error/10) * (left_error/10)) / (100 * MOTOR[KT]) +  MOTOR[KP]); //P值与差值成二次函数关系
    if(Kp1>=9.7) Kp1=9.7;
    L_D = MOTOR[KD] * (left_error + sptr->PrevError - 2 * sptr->LastError);
    I = integrator(left_error,ActualSpeed);
    L_I = I;

    Y =  (I + Kp1 * (left_error - sptr->LastError) + L_D);

    if(curve_flag)
    {
        if(Y > 0)       Y = (Y >  TN ?  TN : Y);
        else            Y = (Y < -TN ? -TN : Y);
        if(ActualSpeed > MAX_ECD)        Y =  (Y < 0 ? Y : 0);
        else if(ActualSpeed < MIN_ECD)   Y =  (Y > 0 ? Y : 0);
    }


    Increase_left += Y;
    out = Increase_left;

    sptr->PrevError = sptr->LastError;       //更新上上次误差
    sptr->LastError = left_error;            //更新上次误差

    return out;
}

int32 right_motor_PI(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
    if(start_line_flag) return -1000;

    //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
    register int32 right_error,out,I,Y;   //当前误差  最后得出的实际增量  积分
    float Kp1;                                           //动态P PS:float x int32==float x float

    right_error = SetSpeed - ActualSpeed;       //计算实际编码器误差
    Kp1 = (((right_error/10) * (right_error/10)) / (100 * MOTOR[KT]) +  MOTOR[KP]); //P值与差值成二次函数关系
    if(Kp1>=9.7) Kp1=9.7;
    I = integrator(right_error,ActualSpeed);
    R_I = I;
    R_D = MOTOR[KD] * (right_error + sptr->PrevError - 2 * sptr->LastError);

    Y = (I + Kp1 * (right_error - sptr->LastError) + R_D);

    if(curve_flag)              //第三种
    {
        if(Y > 0)   Y = (Y >  TN ?  TN : Y);
        else        Y = (Y < -TN ? -TN : Y);
        if(ActualSpeed > MAX_ECD)        Y =  (Y < 0 ? Y : 0);
        else if(ActualSpeed < MIN_ECD)   Y =  (Y > 0 ? Y : 0);
    }

    Increase_right += Y;
    out = Increase_right;

    sptr->PrevError = sptr->LastError;       //更新上上次误差
    sptr->LastError = right_error;            //更新上次误差

    return out;
}
