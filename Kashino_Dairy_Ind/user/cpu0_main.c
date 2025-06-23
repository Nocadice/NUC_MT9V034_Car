/*********************************************************************************************************************
* TC377 Opensourec Library 即（TC377 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC377 开源库的一部分
*
* TC377 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC377TP
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-11-03       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"

// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 此处编写用户代码 例如外设初始化代码等
//
//uint8 key1_status = 1;    //开关状态变量
//uint8 key1_last_status;   //上一次开关状态变量
//uint8 key1_flag;          //开关标志位
//uint16 aaa=SERVO_CENTER;
//uint8 key2_status = 1;
//uint8 key2_last_status;
//uint8 key2_flag;
////
//void key_set(void)
//{
//    //*************************key1**************************
//    key1_last_status=key1_status;
//    key1_status= key_get_state(KEY_1);       //按下是0 没按是1
//    if(key1_status && !key1_last_status)    //如果上次状态是1 这次是0 说明按了
//    {
//        key1_flag=1;
//    }
//    if(key1_flag)
//    {
//        /*这里写按键处理部分*/                   //按下还不会执行，松开才执行
////        basic_speed=1400;
////        start_flag=1;
//        aaa+=25;
//        /*这里写按键处理部分*/
//        key1_flag=0;
//    }
//    key2_last_status=key2_status;
//    key2_status= key_get_state(KEY_2);
//    if(key2_status && !key2_last_status)
//    {
//        key2_flag=1;
//    }
//    if(key2_flag)
//    {
//        /*这里写按键处理部分*/
//        aaa-=25;
//        /*这里写按键处理部分*/
//        key2_flag=0;
//
//    }
//}

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等


    Read_Flash();
    key_init(10);
    mt9v03x_init();                                             //camera
    ips200_init(IPS200_TYPE_PARALLEL8);                         //screen
    imu660ra_init();

    wireless_uart_init();

    pwm_init(SERVO_PIN,200,SERVO_CENTER);

    pwm_init(ATOM0_CH1_P21_3,50,500);
    pwm_init(ATOM0_CH0_P21_2,50,500);

    Servo_pid_Init(&servo_PID);
    IncPID_Init(&left_PID);
    IncPID_Init(&right_PID);

    motor_init();                               //    motor & encoder

    pit_ms_init(CCU60_CH0, 5);
    pit_ms_init(CCU60_CH1, 5);
    pit_ms_init(CCU61_CH0, 10);
    pit_ms_init(CCU61_CH1, 5);
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

    while (TRUE)
    {
        // 此处编写需要循环执行的代码
//        key_set();
//        ips200_show_int(100,120,aaa,4);
//        pwm_set_duty(SERVO_PIN,aaa);
        if(!start_flag)    Key_Set();
        if(RON)
        {
            pwm_set_duty(ATOM0_CH1_P21_3,MT_Lduty);          //左风扇
            pwm_set_duty(ATOM0_CH0_P21_2,MT_Rduty);          //右风扇

        }
        check_start_fing_line();
//         此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

