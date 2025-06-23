/*
 * Circle.c
 *
 *  Created on: 2023年6月7日
 *      Author: 17793
 */

#include "Circle.h"
#include "isr.h"
#include "math.h"

uint8 L_C_F=0,LC=0;
uint8 C_stage_l[10]={0};
uint8 IN_A_L=50,OUT_A_L=230;
uint8 first_f_l=0;
uint8 TJdl=0,TJul=0;
uint8 C_L_O=0;

float solve_k_l(float x1,float y1,float x2,float y2)
{
    return ((y1-y2)/(x1-x2));
}

void circle_left()
{
    uint8 target_v,pt;
    uint8 turnrx,turnry;
    uint8 turnlx=0,turnly;
    if(start_line_flag) return;
    if(L_C_F || start_line_flag)
    {
        FAR=40;
        first_f_l=0;
        memset(C_stage_l,0,sizeof(C_stage_l));
        L_C_F=0;
        LC=0;
    }
    if(Rsr>=0.97 && !L_C_F && !C_stage_l[0] && left_find_time<=45)
    {
        TJdl=0;
        for (int j=65; j>=25; j--)
        {
            if (abs(left_line_list[j+1][1] - left_line_list[j+2][1]) <= 3
                    && abs(left_line_list[j][1] - left_line_list[j+1][1]) <= 3
                    && (left_line_list[j][1] - left_line_list[j-1][1])>= 8
                    && (left_line_list[j][1] - left_line_list[j-2][1])>= 9
                    && (left_line_list[j][1] - left_line_list[j-3][1])>= 10
                    && Ln[j+2] && Ln[j+1] && Ln[j])
            {

                TJdl=1;
                LC=1;
                break;
            }
        }
        if(TJdl)
        {
            C_stage_l[1]=1;
            C_stage_l[0]=1;
            FAR=15;
        }
    }

    if(!C_stage_l[0]) return;
    if(C_stage_l[1])
    {
        TJul=0;
        target_v=0;
        for (int j=6; j<=36; j++)
        {
            if ((left_line_list[j-3][1]-left_line_list[j+3][1]>=60 && left_line_list[j-4][1]-left_line_list[j+4][1]>=65 && left_line_list[j-5][1]-left_line_list[j+5][1]>=70)
                    && left_line_list[j-1][1]-left_line_list[j][1]<=2 && left_line_list[j-2][1]-left_line_list[j-1][1]<=2 && left_line_list[j-3][1]-left_line_list[j-2][1]<=2
                    && (Ln[j-2] && Ln[j - 3] && Ln[j - 4]))
            {
                TJul=1;
                break;
            }
        }
        if(TJul)
        {
            C_stage_l[1]=0;
            C_stage_l[2]=1;
        }
        for(int i=66;i>=6;--i)
            center_line_list[i]=right_line_list[i][1]-st_change[i];
    }
    if(C_stage_l[2])
    {
        float tempk;
        if(!first_f_l)
        {
            gyro_angle_z=0;
            first_f_l=1;
        }
        turnrx=67;
        turnry=180;
        tempk=solve_k_l(20,8,turnrx,turnry);
        memset(left_line_list,18,sizeof(left_line_list));
        fix_right(tempk,turnrx,turnry,1,69);
        if(gyro_angle_z>=IN_A_L)  //陀螺仪打个40度角，也可以找拐点
        {
            C_stage_l[2]=0;
            C_stage_l[3]=1;
            first_f_l=0;
            FAR=40;
        }
    }
    if(C_stage_l[3])
    {
        float tempk=0;
        target_v=188;
        if(!first_f_l)
        {
            gyro_angle_z=0;
            first_f_l=1;
        }


        if(gyro_angle_z>=155)              //转到角度出圆环，也可拐以找点补线出
        {
            for (uint8 i=68; i>=35; --i)
            {
                if(right_line_list[i][1]>=180 && right_line_list[i-1][1]>=180 && right_line_list[i+1][1]>=180)
                    break;
                if(right_line_list[i][1]<target_v)      //
                {
                    target_v=right_line_list[i][1];
                    pt=i;
                }
            }
            if(target_v)
            {
                turnrx=pt;
                turnry=target_v;
                tempk=solve_k_l(turnrx,turnry,20,15);
            }
            fix_right(tempk,turnrx,turnry,5,pt);
        }
        if(Rsr>=0.95 && gyro_angle_z>=OUT_A_L)                //左边35行以上有线，角度转够了
        {
            C_stage_l[3]=0;
            C_stage_l[4]=1;
        }
    }
    if(C_stage_l[4])
    {
        FAR=40;
        for(uint8 i=3;i<=65;++i)
            center_line_list[i]=right_line_list[i][1]-st_change[i]*1.12;
        if(left_find_time>=50 && Rsl>=0.95) L_C_F=1;        //右边有50行有线，出圆环
    }
}
;
