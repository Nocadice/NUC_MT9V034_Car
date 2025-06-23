#include "GUI.h"

uint32 MT_duty=500;
uint32 MT_Lduty = 500;
uint32 MT_Rduty = 500;

boolean start_flag=0,RON=0;
uint8 Page_channel;
uint16 KDI[164][240]={KDI_IMG};

uint16_t Set_speed = 1500;
uint16_t Straight_s=1200;
uint16_t Set_duty = 750;

Key_s Key_num[5]={
    {KEY_RELEASE, KEY_RELEASE, FALSE,FALSE},
    {KEY_RELEASE, KEY_RELEASE, FALSE,FALSE}, // 第一个元素的初始值
    {KEY_RELEASE, KEY_RELEASE, FALSE,FALSE}, // 第二个元素的初始值
    {KEY_RELEASE, KEY_RELEASE, FALSE,FALSE}, // 第三个元素的初始值
    {KEY_RELEASE, KEY_RELEASE, FALSE,FALSE}  // 第四个元素的初始值
};

pin pointer={230,170,1,4,0,0,1};
UI Page = {0,0,Null};

void _Start_()
{
    ips200_show_gray_image(0, 0, image_use[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
    Draw_Line();

    ips200_show_string(0,Second_Line,"   Center:");
    ips200_show_uint(100,Second_Line,center_line,3);

    ips200_show_string(0,Third_Line, "   Gory_z:");
    ips200_show_int(100,Third_Line,imu660ra_gyro_z,4);

    ips200_show_string(0,Fourth_Line,">> SAIL <<");


}

void Speed_Set()
{
    pointer.y = First_Line - 6 * GAP + pointer.line *GAP;
    ips200_show_string(0,First_Line - 5 * GAP," Basic_Speed:");
    ips200_show_uint(120,First_Line - 5 * GAP,Set_speed,4);

    ips200_show_string(0,First_Line - 4 * GAP,"  Straight_s:");
    ips200_show_uint(120,First_Line - 4 * GAP,Straight_s,4);

    ips200_show_string(0,First_Line - 3 * GAP,"    MT_Duty:");
    ips200_show_uint(120,First_Line - 3 * GAP,Set_duty,3);

    ips200_show_string(0,First_Line - 2 * GAP,"R_Circle_IN:");
    ips200_show_int(120,First_Line - 2 * GAP,IN_A_R,3);

    ips200_show_string(0,First_Line - 1 * GAP,"R_Circle_OT:");
    ips200_show_int(120,First_Line - 1 * GAP,OUT_A_R,3);

    ips200_show_string(0,First_Line ,"L_Circle_IN:");
    ips200_show_uint(120,First_Line ,IN_A_L,3);

    ips200_show_string(0,Second_Line ,"L_Circle_OT:");
    ips200_show_uint(120,Second_Line ,OUT_A_L,3);




}

void PID_Set()
{

    pointer.y = First_Line - 6 * GAP + pointer.line *GAP;
    ips200_show_string(0,First_Line - 5 * GAP,"Location:");
    ips200_show_string(0,First_Line - 4 * GAP," Kt:");
    ips200_show_int(60,First_Line - 4 * GAP,TURN[KT],4);
    ips200_show_string(0,First_Line - 3 * GAP," Kp:");
    ips200_show_float(60,First_Line - 3 * GAP,TURN[KP],2,2);
    ips200_show_string(0,First_Line - 2 * GAP," Kd:");
    ips200_show_float(60,First_Line - 2 * GAP,TURN[KD],2,2);
    ips200_show_string(0,First_Line -     GAP," Kd1_g:");
    ips200_show_float(60,First_Line -     GAP,TURN[KD1],1,3);

    ips200_show_string(0,First_Line, "increase:");

    ips200_show_string(0,Second_Line, " Kt:");
    ips200_show_string(0,Third_Line,  " Kp:");
    ips200_show_string(0,Fourth_Line, " Ki_M:");
    ips200_show_string(0,fifth_Line,  " Kd:");

    ips200_show_int(60,Second_Line,MOTOR[KT],4);
    ips200_show_float(60,Third_Line,MOTOR[KP],2,2);
    ips200_show_float(60,Fourth_Line,MOTOR[KI],2,2);
    ips200_show_float(60,fifth_Line,MOTOR[KD],2,2);

}
void IMG_Element()
{
    ips200_show_gray_image(0, 0, image_use[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
    Draw_Line();

    ips200_show_string(0,100,"   Center:");
    ips200_show_int(100,100,center_line,3);

    ips200_show_string(0,120, "      Rsl:");
    ips200_show_float(100,120,Rsl,1,3);

    ips200_show_string(0,140, "      Rsr:");
    ips200_show_float(100,140,Rsr,1,3);

    ips200_show_string(0,160, " Left_f_t:");
    ips200_show_int(100,160,left_find_time,2);

    ips200_show_string(0,180, "right_f_t:");
    ips200_show_int(100,180,right_find_time,2);

    ips200_show_string(0,200, "   gyro_z:");
    ips200_show_int(100,200,gyro_angle_z,3);

    ips200_show_string(0,220, "Start_L_F:");
    ips200_show_int(100,220,start_line_flag,1);


}
void Vaulue_Increase()
{
    switch(Page.property)
    {
        case Pid :
            switch(pointer.line)
            {
                case 2: TURN[KT]  += 5;     break;
                case 3: TURN[KP]  += 0.01;  break;
                case 4: TURN[KD]  += 0.01;  break;
                case 5: TURN[KD1]  += 0.001; break;
                case 7: MOTOR[KT] += 1;  break;
                case 8: MOTOR[KP] += 0.01;  break;
                case 9: MOTOR[KI] += 0.01;  break;
                case 10: MOTOR[KD] += 0.01;  break;
            }
            break;
        case Speed_set :
            switch(pointer.line)
            {
                case 1: Set_speed  += 50;     break;
                case 2: Straight_s  += 50;  break;
                case 3: Set_duty  += 50;  break;
                case 4: IN_A_R  += 5;  break;
                case 5: OUT_A_R  += 5; break;
                case 6: IN_A_L += 5;  break;
                case 7: OUT_A_L += 5;  break;
            }
            break;
        default:                                                break;
    }
}

void Vaulue_Decrease()
{
    switch(Page.property)
    {
        case Pid :
            switch(pointer.line)
            {
                case 2: TURN[KT]  -= 5;     break;
                case 3: TURN[KP]  -= 0.01;  break;
                case 4: TURN[KD]  -= 0.01;  break;
                case 5: TURN[KD1]  -= 0.001; break;
                case 7: MOTOR[KT] -= 1;  break;
                case 8: MOTOR[KP] -= 0.01;  break;
                case 9: MOTOR[KI] -= 0.01;  break;
                case 10: MOTOR[KD] -= 0.01;  break;
            }
            break;
        case Speed_set :
            switch(pointer.line)
            {
                case 1: Set_speed  -= 50;     break;
                case 2: Straight_s  -= 50;  break;
                case 3: Set_duty  -= 50;  break;
                case 4: IN_A_R  -= 5;  break;
                case 5: OUT_A_R  -= 5; break;
                case 6: IN_A_L -= 5;  break;
                case 7: OUT_A_L -= 5;  break;
            }
            break;
        default:                                                 break;
    }
}

void KDI_GUI()
{
    if(start_flag) _Start_();
    if(pointer.fresh)
    {
        if(pointer.y - GAP>=0) ips200_show_char(pointer.x,pointer.y - GAP,' ');
        if(pointer.y + GAP<=340) ips200_show_char(pointer.x,pointer.y + GAP,' ');


        if(pointer.type)
            ips200_show_char(pointer.x,pointer.y,'O');
        else
            ips200_show_char(pointer.x,pointer.y,'<');
        pointer.fresh = 0;
    }

    if(Page.page == 0 && Page.change)
    {
        ips200_clear();
        for(uint8 i=0;i<164;++i)
            for(uint8 j=0;j<240;++j)
                ips200_draw_point(j,i,KDI[i][j]);
        ips200_show_string(10,First_Line, ">>   Start  <<");
        ips200_show_string(10,Second_Line,"Speed & Circle");
        ips200_show_string(10,Third_Line, "       PID_Set");
        ips200_show_string(10,Fourth_Line,"   IMG_Element");
        ips200_show_char(pointer.x,pointer.y,'<');
        Page.change = 0;
    }
    else if(Page.page == 1 && Page.change)
    {
        if(Page_channel == 1) {_Start_();     return;}
        if(Page_channel == 2) {Speed_Set();   return;}
        if(Page_channel == 3) {PID_Set();     return;}
        if(Page_channel == 4) {IMG_Element(); return;}
        Page.change = 0;
    }

    if(Page.page == 0 && pointer.confirm )
    {
        pointer.confirm=0;

        Page.page = 1;
        Page.change = 1;

        ips200_clear();
        pointer.y = First_Line;

        if     (pointer.line == 1) Page_channel = 1;
        else if(pointer.line == 2) {Page_channel = 2;pointer.y = First_Line - 5 * GAP;}
        else if(pointer.line == 3) {Page_channel = 3;pointer.y = First_Line - 5 * GAP;}
        else if(pointer.line == 4) Page_channel = 4;

        Page.property = Page_channel;
        switch(Page.property)
        {
            case Start :       pointer.range = 4;               break;
            case Speed_set :   pointer.range = 7;               break;
            case Pid :         pointer.range = 10;               break;
            case IMG_element : pointer.range = 2;               break;
            default:                                            break;
        }

        pointer.line = 1;
        ips200_show_char(pointer.x,pointer.y,'<');
    }

}

void UI_Init()
{
    Page_channel = 0;

    Page.property = Null;
    Page.change = 1;
    Page.page = 0;

    pointer.y = First_Line;
    pointer.line = 1;
    pointer.range = 4;
    pointer.type = 0;

}

void Key_Set()
{
        if(start_flag) return;
        uint8 i = 0;
        for(i = 1;i <= 4;i++)
            Key_num[i].Last = Key_num[i].Now;

        for(i = 1;i<=4;i++)
            Key_num[i].Now = key_get_state(i-1);

        for(i = 1;i<=4;i++)
            if(Key_num[i].Now && !Key_num[i].Last)
                Key_num[i].Flag = 1;

        if(Key_num[1].Flag) // 返回功能
        {
           Key_num[1].Flag = 0;
           Write_Flash();
           UI_Init();
        }

        if(Key_num[2].Flag)
        {
            Key_num[2].Flag = 0;
            pointer.fresh = 1;
            if(!pointer.type)                     //up
            {
                if(pointer.line > 1)
                {
                    pointer.line--;
                    pointer.y -= GAP;
                }
            }
            else                                  //increase
                Vaulue_Increase();
        }
        if(Key_num[3].Flag)
        {
            Key_num[3].Flag = 0;
            pointer.fresh = 1;
           if(!pointer.type)                      //down
           {
               if(pointer.line < pointer.range)
               {
                   pointer.line++;
                   pointer.y += GAP;
               }
           }
           else                                   //decrease
               Vaulue_Decrease();
        }

        if(Key_num[4].Flag)
        {
            Key_num[4].Flag = 0;
            pointer.confirm = (Page.page) ? 0 : 1;
            pointer.type = (Page.page) ? (~pointer.type) : 0;
            pointer.fresh = 1;
            if(Page_channel==1 && pointer.line == 4)
            {
                if(RON)
                {
                    basic_speed = Set_speed;
                    start_flag=1;
                }
                if(!start_flag)
                {
                    MT_duty = Set_duty;
                    MT_Lduty = Set_duty;
                    MT_Rduty = Set_duty;
                    RON=1;
                }
            }

        }
}
void Write_Flash(void)
{
    struct Dat {
        float Set_MOTOR[5];
        float Set_Turn[5];
        uint16_t Speed_set;
        uint16_t Straight_S;
        uint16_t Duty_Set;
        int16 RC_A_IN;
        int16 RC_A_OUT;
        uint8 LC_A_IN;
        uint8 LC_A_OUT;
    } dat = {
        {MOTOR[0], MOTOR[1], MOTOR[2], MOTOR[3], MOTOR[4],},
        {TURN[KP], TURN[KI], TURN[KD], TURN[KD1], TURN[KT], },
        Set_speed,
        Straight_s,
        Set_duty,
        IN_A_R,
        OUT_A_R,
        IN_A_L,
        OUT_A_L
    };
    flash_erase_page(0, 0);
    flash_write_page(0, 0, (const uint32_t)&dat, sizeof(dat));
}

void Read_Flash(void)
{
    struct Dat {
        float Set_MOTOR[5];
        float Set_Turn[5];
        uint16_t Speed_set;
        uint16_t Straight_S;
        uint16_t Duty_Set;
        int16 RC_A_IN;
        int16 RC_A_OUT;
        uint8 LC_A_IN;
        uint8 LC_A_OUT;
    } dat ={{0},{0},0,0,0,0,0,0,0};
    flash_read_page(0, 0, (uint32_t)&dat, sizeof(dat));
    memcpy(MOTOR,dat.Set_MOTOR, sizeof(MOTOR));
    memcpy(TURN,dat.Set_Turn, sizeof(TURN));
    Set_speed = dat.Speed_set;
    Straight_s= dat.Straight_S;
    Set_duty = dat.Duty_Set;
    IN_A_R = dat.RC_A_IN;
    OUT_A_R = dat.RC_A_OUT;
    IN_A_L = dat.LC_A_IN;
    OUT_A_L = dat.LC_A_OUT;
}
