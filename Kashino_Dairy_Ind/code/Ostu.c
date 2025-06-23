/*
 * scan.c
 *
 *  Created on: 2023/11/16
 *      Author: 17793
 */
#include "Ostu.h"

uint8 image_use[70][188];

uint8 Ostu(uint8 *image, uint16 col, uint16 row)
{

    uint8 LOST=0;
    uint16 width = col;
    uint16 height = row;
    uint8 pixelNum[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height /2;
    uint8* IMGdata = image;  //指向像素数据的指针

    memset(pixelNum,0,GrayScale);
    memset(pixelPro,0,GrayScale);

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++)
        for (j = i%2; j < width; j+=2)
            pixelNum[(int)IMGdata[i * width + j]]++;  //将灰度值作为计数数组的下标

    //计算每个像素在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
        pixelPro[i] = (float)pixelNum[i] / pixelSum;

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u;
    float Var[GrayScale];
    for (i=0;i<GrayScale;i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = 0;
        for (j=0;j<GrayScale;j++)
        {
            if (j <= i)   //背景部分为黑色
            {
                w0+= pixelPro[j];          //背景占全图的比例
                u0tmp+= j*pixelPro[j];   //背景平均灰度值
            }
            else          //前景部分为白色即赛道
            {
                w1+= pixelPro[j];          //赛道占全图的比例
                u1tmp+= j*pixelPro[j];   //赛道平均灰度值
            }
        }
        u0 = u0tmp / w0;                    //Formula of Ostu
        u1 = u1tmp / w1;
        u  = u0tmp + u1tmp;
        Var[i] = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
    }
    //三分法
    uint16 L = 0,R = 2550;
    uint16 lm,rm;
    while(L+5 < R)
    {
        lm = L + ( (R - L) / 3);
        rm = R - ( (R - L) / 3);
        if(Var[lm/10] >= Var[rm/10]) R = rm; //change
        else L=lm;
    }
    if(R >= 2500)     //画面过于一致，u0 u1相差不大，要吗黑，要吗白（滤波）
    {
        R = 700;
        LOST = 1;
    }
        //三分法
    for(i = 0;i < height;++i)
        for(j = 0;j < width;++j)
            image_use[i][j] = (uint8)IMGdata[i * width + j] > (uint8)(R/10) ? 255:0;
    return LOST==1 ? 1 : 0;
}
