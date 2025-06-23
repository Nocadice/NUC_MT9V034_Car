/*
 * Circle.c
 *
 *  Created on: 2023年7月4日
 *      Author: 17793
 */

#include "Determination.h"

boolean Brake=0;
float b,ax,ay,axs,Rsl,Rsr,k,temp1,temp2;
float Sxy=0,Sxs=0,Sx=0,Sy=0;

uint8 numb=0;
boolean straight_flag=0,curve_flag=0,crossing_flag=0,start_line_flag=0;

inline void reset_D()
{
    numb=0;
    Sxy=0;
    Sxs=0;
    Sx=0;
    Sy=0;
    temp1=0;
    temp2=0;
}
void D_road()
{
    reset_D();
    if(left_find_time>=45)
    {
        uint8 tag=0;
        for(uint8 i=55;i>=40;--i)
        {
            if(Ln[i] && Ln[i+10])
            {
                tag=i;
                break;
            }
        }
        if(tag)
        {
            for(uint8 i=tag;i<=tag+10;++i)
            {
                if(Ln[i])
                {
                    for(uint8 j=1;j<=Ln[i];++j)
                    {
                        numb++;
                        Sxy+=i*left_line_list[i][j];
                        Sxs+=i*i;
                        Sx+=i;
                        Sy+=left_line_list[i][j];
                    }
                }
            }
            ax=Sx/(float)(numb);
            ay=Sy/(float)(numb);
            k=(Sxy-(float)(numb)*ax*ay)/(Sxs-(float)(numb)*ax*ax);
            b=ay-k*ax;
            for(uint8 i=5;i<=15;++i)
            {
                if(Ln[i])
                {
                    for(uint8 j=1;j<=Ln[i];++j)
                    {
                       temp1+=((k*(float)(i)+b)-left_line_list[i][j])*((k*(float)(i)+b)-left_line_list[i][j]);
                       temp2+=((k*(float)(i)+b)-ay)*((k*(float)(i)+b)-ay);
                    }
                }
            }
            Rsl=(float)(1.0)-(temp1/temp2);
        }
        else Rsl=0;
    }
    else Rsl=0;

    reset_D();
    if(right_find_time>=45)
    {
        uint8 tag=0;
        for(uint8 i=55;i>=40;--i)
        {
            if(Rn[i] && Rn[i+10])
            {
                 tag=i;
                 break;
            }
        }
        if(tag)
        {
            for(uint8 i=tag;i<=tag+10;++i)
            {
                if(Rn[i])
                {
                    for(uint8 j=1;j<=Rn[i];++j)
                    {
                        numb++;
                        Sxy+=i*right_line_list[i][j];
                        Sxs+=i*i;
                        Sx+=i;
                        Sy+=right_line_list[i][j];
                    }
                }
            }
            ax=Sx/(float)(numb);
            ay=Sy/(float)(numb);
            k=(Sxy-(float)(numb)*ax*ay)/(Sxs-(float)(numb)*ax*ax);
            b=ay-k*ax;
            for(uint8 i=5;i<=15;++i)
            {
                if(Rn[i])
                {
                    for(uint8 j=1;j<=Rn[i];++j)
                    {
                       temp1+=((k*(float)(i)+b)-right_line_list[i][j])*((k*(float)(i)+b)-right_line_list[i][j]);
                       temp2+=((k*(float)(i)+b)-ay)*((k*(float)(i)+b)-ay);
                    }
                }
            }
            Rsr=(float)(1.0)-(temp1/temp2);
        }
        else Rsr=0;
    }
    else Rsr=0;
}

inline void judge_road()
{
    if(Rsl >= 0.95 && Rsr >= 0.95)  {straight_flag=1;curve_flag=0;crossing_flag=0;FAR=35; return;}
    if(Rsl == 0    && Rsr == 0 && left_find_time<=35 && right_find_time<=35)     {straight_flag=0;curve_flag=0;crossing_flag=1;FAR=10; return;}
    if(Rsl < 0.9  || Rsr < 0.9 )
    {
        if(straight_flag) Brake=1;
        {straight_flag=0;curve_flag=1;crossing_flag=0;FAR=35; return;}
    }
}

void check_start_fing_line(void)
{
        if(start_line_flag) return;
        uint8 times = 0;
        uint8 black_blocks = 0;
        uint8 cursor = 0;
        for (uint8 i = 35; i <= 50; i++)    //30到55行
        {
            black_blocks = 0;
            cursor = 0;    //指向栈顶
            for(uint8 j = 50; j <= 130; j++)
            {
                if (image_use[i][j] < 160)    //遇到黑色元素plus
                {
                    if (cursor < 20) cursor++;
                }
                else                           //没有遇到
                {
                    if (cursor >= 2 && cursor <= 8)
                     {
                        black_blocks++;
                        cursor = 0;
                    }
                    else cursor = 0;
                }
            }
            if (black_blocks >= 4 && black_blocks <= 12)  times++;
            if(times >= 2 && times <= 8)
            {
                start_line_flag = 1;
                return;
            }
        }
        start_line_flag = 0;
}
