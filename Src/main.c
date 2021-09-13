#include "bsp_lib.h"
#include "main.h"
#include "smg.h"
#include "ir.h"
#include "ds1302.h"
#include "stdio.h"
#include "stdlib.h"
extern u8 STA_main;
extern char char1, char2, char3;
extern u8 smg_num;
extern u8 STA_P;    //速度默认是p1，最慢的速度
extern u8 STA_F;    //默认是F1模式
extern u8 STA_Mode; //振动，敲打或者混合
extern u8 key_flag; //保存按键状态
extern unsigned long tick_ms, tick_s;
extern unsigned long cnt_min;
extern unsigned long cnt_hour;
extern unsigned long cnt_s;
extern unsigned long cnt_s_2;
void task(unsigned long shutdown_tim1, unsigned long delay_tim2);
void shutdown(void);
void main(void)
{
    char temp_flag = 0;
    long j = 9000;
    led_init();
    UartInit();
    IR_init();
    Timer0Init();
    PCA_Init();
    led_init();
    // while (1)
    // {
    //     motor_set(motor1, 400);
    //     delay_ms(500);
    //     motor_set(motor1, -400);
    //     delay_ms(500);
    // }

    while (1)
    {
        if (key_flag == 0)
        {
            STA_main = STA_main & 0xf7;
            break;
        }
        smg_num = 0;
    }
    while (1)
    {
        display_user(0x07, 20, 18, 17);
        if (STA_Mode != 0)
        {
            STA_main = STA_main | (0x80);
            break;
        }
    }
    while (1)
    {
        if (((STA_main & 0x80) == 0x80))
        {
            switch (STA_F)
            {
            case 1:
                task(15, 0L);
                break;
            case 2:
                task(30, 10);
                break;
            case 3:
                task(60, 30);
                break;
            case 4:
                task(120, 60);
                break;
            case 5:
                task(240, 2 * 60);
                break;
            case 6:
                task(360, 5 * 60);
                break;
            case 7:
                task(720, 10 * 60);
                break;
            case 8:
                task(1440, 20 * 60);
                break;
            case 9:
                srand(tick_ms % 1000);
                task(60 + (rand() % 180), 30 * 60);
                break;
            default:
                break;
            }
        }
        if ((STA_main & 0x08) == 0x08)
        {
            STA_P = 1; //速度默认是p1，最慢的速度
            STA_F = 1; //默认是F1模式
            STA_Mode = 0;
            STA_main = 0x00; //主要的状态标志位
            key_flag = 20;
            stop_mode();
            while (1)
            {
                if ((STA_main & 0x08) == 0x08)
                {
                    STA_main = STA_main & 0xf7;
                    break;
                }
                smg_num = 0;
            }
            while (1)
            {
                display_user(0x07, 20, 18, 17);
                if (STA_Mode != 0)
                {
                    STA_main = STA_main | 0x80;
                    break;
                }
            }
        }
        // if ((STA_main & 0x10) == 0x10)
        // {
        //     stop_mode();
        // }
    }
}
void task(unsigned long shutdown_tim1, unsigned long delay_tim2)
{
    unsigned long tick_s_temp1 = 0, tick_s_temp2 = 0;
    unsigned long i = 0;
    char flag1 = 0, flag2 = 0;
    unsigned char STA_F_old = 0;
    tick_s_temp1 = tick_s;
    cnt_s = 0;
    cnt_min = 0;
    cnt_hour = 0;
    STA_F_old = STA_F;
    while (((STA_main & 0x08) != 0x08) && (STA_F_old == STA_F) && ((STA_main & 0x10) != 0x10))
    {
        if (cnt_min >= shutdown_tim1)
        {
            STA_main = 0x08;
        }
        if (STA_F == 1)
        {
            if (STA_Mode == mode1) //振动模式
            {
                zhendong_mode();
            }
            else if (STA_Mode == mode2) //敲打模式
            {
                qiaoda_mode();
            }
            else if (STA_Mode == mode3) //混合模式
            {
                hunhe_mode();
            }
        }
        else
        {
            cnt_s_2 = 0;
            STA_F_old = STA_F;
            while ((cnt_s_2 < delay_tim2) && (STA_F_old == STA_F) && ((STA_main & 0x10) != 0x10))
            {
                if (STA_Mode == mode1) //振动模式
                {
                    zhendong_mode();
                }
                else if (STA_Mode == mode2) //敲打模式
                {
                    qiaoda_mode();
                }
                else if (STA_Mode == mode3) //混合模式
                {
                    hunhe_mode();
                }
                if ((STA_main & 0x08) == 0x08)
                {
                    break;
                }
                if (cnt_min >= shutdown_tim1)
                {
                    STA_main = 0x08;
                }
            }
            cnt_s_2 = 0;
            while ((cnt_s_2 < delay_tim2) && (STA_F_old == STA_F) && ((STA_main & 0x10) != 0x10)) //停止
            {
                stop_mode();
                if ((STA_main & 0x08) == 0x08)
                {
                    break;
                }
                if (cnt_min >= shutdown_tim1)
                {
                    STA_main = 0x08;
                }
            }
        }
    }
}
