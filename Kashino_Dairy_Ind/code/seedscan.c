/*
 * scan.c
 *
 *  Created on: 2023/6/5
 *      Author: 17793
 */
#include "seedscan.h"

uint8 left_line_list[MT9V03X_H+2][50];
uint8 right_line_list[MT9V03X_H+2][50];
uint8 center_line_list[MT9V03X_H];
uint8 Ln[75]={0},Rn[75]={0};                        //How many points per line
uint8 st_change[70]={129, 18, 18, 20, 20, 20, 22, 22, 22, 24, 24, 24, 26, 26, 26, 28, 28, 28, 30, 30, 30, 31, 32, 32, 32, 34, 34, 34, 36, 36, 36, 38, 38, 38, 40, 40, 40, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 48, 49, 50, 50, 50, 51, 52, 52, 53, 54, 54, 55, 56, 56, 56, 57, 58, 58, 59, 94, 94, 0};
uint8 FAR=35;


uint8 left_find_time;
uint8 right_find_time;
uint8 turnl=0;
uint8 turnr=0;
int ways_f[4][2]={{-1,0},{0,1},{1,0},{0,-1}};       //front Vector (U R D L)
int ways_fl[4][2]={{-1,-1},{-1,1},{1,1},{1,-1}};    //front left Vector
int ways_fr[4][2]={{-1,1},{1,1},{1,-1},{-1,-1}};    //front right Vector


uint8 dirl=0,dirr=0;
uint8 sxl,syl,sxr,syr;

uint8 center_line;

void find_lseeds()
{
    sxl=0;
    for(uint8 i=MT9V03X_H-3; i>=FAR;i--)          //*****************************
    {
        if(image_use[i][(MT9V03X_W/2)+13] == 255)
        {
            for(uint8 j = (MT9V03X_W/2)+13; j >= 6; j--)           //Find left seed
            {
                if ((!image_use[i][j-1]) && (!image_use[i][j]) && (image_use[i][j+1])) //bbw
                {
                    sxl=i;
                    syl=j+1;
                    break;
                }
            }
            if(sxl) return;
        }
    }

        for(uint8 i=MT9V03X_H-3; i>=FAR;i--)
        {
            if(!image_use[i][(MT9V03X_W/4)+13]) break;
            for (uint8 j=(MT9V03X_W/4)+13; j>=6; j--)
            {
                if((!image_use[i][j-1]) && (!image_use[i][j]) && (image_use[i][j+1]))
                {
                    sxl=i;
                    syl=j+1;
                    break;
                }
            }
            if(sxl) return;
        }

        for(uint8 i=MT9V03X_H-3; i>=FAR;i--)
        {
            if(!image_use[i][(MT9V03X_W/4)*3+13]) return;
            for (uint8 j=(MT9V03X_W/4)*3+13;j>=(MT9V03X_W/2)+13;j--)
            {
                if((!image_use[i][j-1]) && (!image_use[i][j]) && (image_use[i][j+1]))
                {
                    sxl=i;
                    syl=j+1;
                    break;
                }

            }
            if(sxl) return;
        }
}

void find_rseeds()
{
    sxr=0;
    for(uint8 i=MT9V03X_H-3; i>=FAR;i--)
    {
        if(image_use[i][(MT9V03X_W/2)+13] == 255)
        {
            for (uint8 j = (MT9V03X_W/2)+13; j <= MT9V03X_W - 4; j++)          //Find right seed
            {
                if((image_use[i][j-1]) && (!image_use[i][j]) && (!image_use[i][j+1])) //wbb
                {
                    sxr=i;
                    syr=j-1;
                    break;
                }
            }
            if(sxr) return;
        }
    }

        for(uint8 i=MT9V03X_H-3; i>=FAR;i--)
        {
            if(!image_use[i][(MT9V03X_W/4)*3+13]) return;
            for (uint8 j=(MT9V03X_W/4)*3+13;j<=185;j++)
            {
                if((image_use[i][j-1]) && (!image_use[i][j]) && (!image_use[i][j+1]))
                {
                    sxr=i;
                    syr=j-1;
                    break;
                }
            }
            if(sxr) return;
        }
        for(uint8 i=MT9V03X_H-3; i>=FAR;i--)
        {
            if(!image_use[i][(MT9V03X_W/4)+13]) break;
            for (uint8 j=(MT9V03X_W/4)+13; j<=(MT9V03X_W/2)+13; j++)
            {
                if((image_use[i][j-1]) && (!image_use[i][j]) && (!image_use[i][j+1]))
                {
                    sxr=i;
                    syr=j-1;
                    break;
                }
            }
            if(sxr) return;
        }


}

void growl(uint8 tx,uint8 ty)
{
    dirl=0;
    turnl=0;
    for(int i=1;i<=250;++i)
    {
        if(tx<1 || tx>68 || turnl>=4 || abs(left_line_list[tx][Ln[tx]]-right_line_list[tx][1])<=5) break; //|| ty<2 || ty>185
        if(image_use[tx+ways_f[dirl][0]][ty+ways_f[dirl][1]]==0) // 前进方向像素为黑色
        {
            dirl = (dirl + 1) % 4;   // 遇到前方为黑色需要右转一次
            turnl++;
        }
        else
        {
            if(image_use[tx+ways_f[dirl][0]][ty+ways_f[dirl][1]]==255 && image_use[tx+ways_fl[dirl][0]][ty+ways_fl[dirl][1]]==0) // 前方像素为白色，且左前方像素为黑色
            {
                tx += ways_f[dirl][0];
                ty += ways_f[dirl][1];
                if(ty>=5)
                {
                    if(!Ln[tx]) left_find_time++;
                    left_line_list[tx][++Ln[tx]]=ty;
                }
                turnl=0;
            }
            else // 前方为白色，左前方为白色（墙角）
            {
                tx += ways_fl[dirl][0];   // 遇到墙角要斜着走
                ty += ways_fl[dirl][1];
                dirl = (dirl + 3) % 4;   // 遇到墙角要左转一次
                if(ty>=5)
                {
                    if(!Ln[tx]) left_find_time++;
                    left_line_list[tx][++Ln[tx]]=ty;
                }
                turnl=0;
            }
        }
    }
}

void growr(uint8 tx,uint8 ty)
{
    dirr=0;
    turnr=0;
    for(int i=1;i<=250;++i)
    {
       if(tx<1 || tx>68 || turnr>=4 || abs(right_line_list[tx][Rn[tx]]-left_line_list[tx][1])<=5) break; //|| ty<2 || ty>185

       if(image_use[tx+ways_f[dirr][0]][ty+ways_f[dirr][1]]==0) // 前进方向像素为黑色
       {
           dirr = (dirr + 3) % 4;   // 遇到前方为黑色需要右转一次
           turnr++;
       }
       else
       {
           if(image_use[tx+ways_f[dirr][0]][ty+ways_f[dirr][1]]==255 && image_use[tx+ways_fr[dirr][0]][ty+ways_fr[dirr][1]]==0) // 前方像素为白色，且左前方像素为黑色
           {
                tx += ways_f[dirr][0];
                ty += ways_f[dirr][1];
                if(ty<=185)
                {
                    if(!Rn[tx]) right_find_time++;
                    right_line_list[tx][++Rn[tx]]=ty;
                }
                turnr=0;
            }
            else // 前方为白色，右前方为白色（墙角）
            {
                tx += ways_fr[dirr][0];   // 遇到墙角要斜着走
                ty += ways_fr[dirr][1];
                dirr = (dirr + 1) % 4;   // 遇到墙角要右转一次
                if(ty<=185)
                {
                    if(!Rn[tx]) right_find_time++;
                    right_line_list[tx][++Rn[tx]]=ty;
                }
                turnr=0;
            }
        }
     }
}

void seeds(void)        //5~65
{
    left_find_time=0;
    right_find_time=0;
    memset(Ln,0,sizeof(Ln));
    memset(Rn,0,sizeof(Rn));
    find_lseeds();
    find_rseeds();                      // put in seeds
    if(sxl!=0) growl(sxl,syl);
    if(sxr!=0) growr(sxr,syr);
    for(int i=1;i<=68;++i)                              //Find a random point
    {
        if(!Rn[i])   // fix crossing
            right_line_list[i][1]=206;
        if(!Ln[i])
            left_line_list[i][1]=18;
        center_line_list[i]=right_line_list[i][1]/2+left_line_list[i][1]/2;
    }
}


uint8 center_weight_list[70]=
{
     1,1,1,1,1,1,1,1,1,1,
     1,1,1,1,1,1,1,1,1,1,
     1,1,3,5,8,11,15,18,15,11,
     8,5,3,1,1,1,1,1,1,1,
     1,1,1,1,1,1,1,1,1,1,
     1,1,1,3,3,3,3,1,1,1,
     1,1,1,1,1,1,1,1,1,1,
};
//uint8 center_weight_list[70]=
//{
//     1,1,1,1,1,1,1,1,1,1,
//     1,1,1,1,1,1,1,1,1,3,
//     3,5,11,17,11,6,3,1,1,1,
//     1,1,1,1,1,1,1,1,1,1,
//     1,1,1,3,3,3,3,1,1,1,
//     1,1,1,1,1,1,1,1,1,1,
//     1,1,1,1,1,1,1,1,1,1,
//};

void Find_Center_Line_weight(void)
{
    uint32 weight_sum =0;
    int16 center_line_error=0;   //      0~188
    uint32 weight_centerline_sum=0;

    for(uint8 i=MT9V03X_H - 2;i>1;i--)
    {
        if(!Ln[i] && !Rn[i]) continue;
        center_line_error =abs(center_line_list[i]-center_line_list[i+1]);
        weight_centerline_sum += center_weight_list[i]*center_line_list[i];
        weight_sum += center_weight_list[i];
    }
    if(weight_sum) center_line = weight_centerline_sum / weight_sum;
}

//-------------------------------paint-----------------------------------------
void Draw_Line(void)
{
          for(uint8 i=MT9V03X_H-1;i>0;i--)
                 Draw_Points(center_line_list[i],i,RGB565_BROWN);
          for(int i=1;i<=68;++i)
          {
              if(right_line_list[i][1]<185 && right_line_list[i][1]>2)
                      Draw_Points(right_line_list[i][1],i,RGB565_RED);
              else Draw_Points(186,i,RGB565_RED);
          }

          for(int i=1;i<=68;++i)
          {
              if(left_line_list[i][1]<185 && left_line_list[i][1]>2)
                      Draw_Points(left_line_list[i][1],i,RGB565_GREEN);
              else Draw_Points(1,i,RGB565_GREEN);
          }
  }

inline void Draw_Points(uint8 x,uint8 y, uint16 color)
  {
      if(x != MT9V03X_W - 1)
      {
          ips200_draw_point(x+1,y,color);
          if(y !=MT9V03X_H-1)
          {
              ips200_draw_point(x,y+1,color);
              ips200_draw_point(x+1,y+1,color);
          }
          if(y !=0)
          {
              ips200_draw_point(x,y+1,color);
              ips200_draw_point(x+1,y-1,color);
          }
      }

      if(x !=0)              //    if((x !=1)&&(x !=0))
      {
          ips200_draw_point(x-1,y,color);
          if(y !=MT9V03X_H-1)
          {
              ips200_draw_point(x,y+1,color);
              ips200_draw_point(x-1,y+1,color);
          }
          if(y !=0)
          {
              ips200_draw_point(x,y+1,color);
              ips200_draw_point(x-1,y-1,color);
          }
      }
      ips200_draw_point(x,y,color);
}
