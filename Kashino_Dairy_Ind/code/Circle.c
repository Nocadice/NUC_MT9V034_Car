/*
 * Circle.c
 *
 *  Created on: 2023年6月7日
 *      Author: 17793
 */

#include "Circle.h"
#include "isr.h"
#include "math.h"

boolean R_C_F=0,RC=0;         //右圆环结束的标志，右圆环开始的标志
int16 IN_A_R=-45,OUT_A_R=-240;           //出入角度
uint8 C_stage[10]={0};      //处于圆环的哪个状态
uint8 first_f=0;            //陀螺仪清零标志位
boolean TJu=0,TJd=0;

float solve_k(float x1,float y1,float x2,float y2)      //两点斜率
{
    return ((y1-y2)/(x1-x2));
}

void fix_right(float k,float x,float y,uint8 S,uint8 E)     //补右线
{
    float b=y-k*x;
    for(int i=S;i<=E;++i)
        right_line_list[i][1]=k*i+b;
    for(int i=1;i<=68;++i) center_line_list[i]=right_line_list[i][1]/2+left_line_list[i][1]/2;
}

void fix_left(float k,float x,float y,uint8 S,uint8 E)      //补左线
{
    float b=y-k*x;
    for(int i=S;i<=E;++i)
        left_line_list[i][1]=k*i+b;
    for(int i=1;i<=68;++i) center_line_list[i]=right_line_list[i][1]/2+left_line_list[i][1]/2;
}

void circle()
{
    uint8 target_v,pt;
    uint8 turnrx,turnry;
    uint8 turnlx=0,turnly;
    if(start_line_flag) return;
    if(R_C_F || start_line_flag)
    {
        FAR=40;                             //前瞻距离
        first_f=0;
        memset(C_stage,0,sizeof(C_stage));
        R_C_F=0;
        RC=0;
    }
    if(Rsl>=0.97 && R_C_F==0 && !C_stage[0] && right_find_time<=45)        //Rsl————左边为直线拟合的R方，
    {
        TJd=0;
        for (uint8 j=65;j>=25;j--)                    //右下拐点 从下往上找
        {
            if(abs(right_line_list[j+1][1] - right_line_list[j+2][1]) <= 3
                && abs(right_line_list[j][1] - right_line_list[j+1][1]) <= 3
                && (right_line_list[j-1][1] - right_line_list[j][1])>= 8
                && (right_line_list[j-2][1] - right_line_list[j][1])>= 9
                && (right_line_list[j-3][1] - right_line_list[j][1])>= 10
                && Rn[j+2] && Rn[j+1] && Rn[j])
            {
                 TJd=1;
                 RC=1;
                 break;
            }
        }
        if(TJd)
        {
            C_stage[1]=1;
            C_stage[0]=1;
            FAR=10;
            //gpio_set_level(BEEP_PIN,1);
        }
    }
    if(!C_stage[0]) return;

    if(C_stage[1])
    {
        TJu=0;
        for (uint8 j=5; j<=30; j++)
        {
             if ((right_line_list[j+3][1]-right_line_list[j-3][1]>=60 && right_line_list[j+4][1]-right_line_list[j-4][1]>=65 && right_line_list[j+5][1]-right_line_list[j-5][1]>=70)
                 && right_line_list[j][1]-right_line_list[j-1][1]<=2 && right_line_list[j-1][1]-right_line_list[j-2][1]<=2 && right_line_list[j-2][1]-right_line_list[j-3][1]<=2
                 && (Rn[j-2] && Rn[j - 3] && Rn[j - 4]))
             {
                 TJu=1;
                 break;
             }
        }
        if(TJu)
        {
            C_stage[1]=0;
            C_stage[2]=1;
        }
        for(int i=66;i>=6;--i)
            center_line_list[i]=left_line_list[i][1]+st_change[i];
    }
    if(C_stage[2])
    {
        float tempk;
        if(!first_f)
        {
            gyro_angle_z=0;
            first_f=1;
        }
        turnrx=20;
        turnry=170;
        tempk=solve_k(67,7,turnrx,turnry);
        memset(right_line_list,206,sizeof(right_line_list));
        fix_left(tempk,turnrx,turnry,1,69);
        if(gyro_angle_z<=IN_A_R)  //陀螺仪打个40度角，也可以找拐点
        {
            C_stage[2]=0;
            C_stage[3]=1;
            first_f=0;
            FAR=40;
        }
    }
    if(C_stage[3])
    {
        float tempk=0;
        target_v=0;
        if(!first_f)
        {
            gyro_angle_z=0;
            first_f=1;
        }
        for (uint8 i=68; i>=35; --i)
        {
            if(left_line_list[i][1]<=5 && left_line_list[i-1][1]<=5 && left_line_list[i+1][1]<=5)
                break;
            if(left_line_list[i][1]>target_v)      //
            {
                target_v=left_line_list[i][1];
                pt=i;
            }
        }

        if(gyro_angle_z<=-155)              //转到角度出圆环，也可拐以找点补线出
        {
            if(target_v)
            {
                turnlx=pt;
                turnly=target_v;
                tempk=solve_k(turnlx,turnly,20,170);
            }
            fix_left(tempk,turnlx,turnly,5,pt);
        }
        if(Rsl>=0.95 && gyro_angle_z<=OUT_A_R)                //左边35行以上有线，角度转够了
        {
            C_stage[3]=0;
            C_stage[4]=1;
        }
    }
    if(C_stage[4])          //left
    {
        FAR=40;
        for(uint8 i=3;i<=65;++i)
            center_line_list[i]=left_line_list[i][1]+st_change[i]*1.12;
        if(right_find_time>=50 && Rsr>=0.95) R_C_F=1;        //右边有50行有线，出圆环
    }
}
