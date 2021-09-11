#include "bsp_lib.h"
#include "main.h"
#include "smg.h"
#include "ir.h"
#include "ds1302.h"
extern u8 STA_main;
extern char char1, char2, char3;
extern u8 smg_num;
extern u8 STA_P;    //速度默认是p1，最慢的速度
extern u8 STA_F;    //默认是F1模式
extern u8 STA_Mode; //振动，敲打或者混合
extern u8 key_flag; //保存按键状态
extern unsigned long tick_ms, tick_s;
void task(unsigned long shutdown_tim1, unsigned long delay_tim2, char mode);
void shutdown(void);
void main(void)
{
    char temp_flag = 0;
    unsigned long tick_ms_temp;
    led_init();
    UartInit();
    IR_init();
    Timer0Init();
    PCA_Init();
    while (1)
    {
        if (key_flag == 0)
        {
            break;
        }
        smg_num = 0;
    }
    while (1)
    {
        display_user(0x07, 20, 18, 17);
        if (STA_Mode!=0)
        {
            STA_main = STA_main | (0x80);
            break;
        }
    }
    while (1)
    {
        smg_num = 0x07;
        switch (STA_F)
        {
        case 1:
            task(900L, 0L, STA_Mode);
            break;
        case 2:
            task(0.5 * 60 * 60, 10, STA_Mode);
            break;
        case 3:
            task(1 * 60 * 60, 30, STA_Mode);
            break;
        case 4:
            task(2 * 60 * 60, 60, STA_Mode);
            break;
        case 5:
            task(4 * 60 * 60, 2 * 60, STA_Mode);
            break;
        case 6:
            task(6 * 60 * 60, 5 * 60, STA_Mode);
            break;
        case 7:
            task(12 * 60 * 60, 10 * 60, STA_Mode);
            break;
        case 8:
            task(24 * 60 * 60, 20 * 60, STA_Mode);
            break;
        case 9:
            task(0.5 * 60 * 60, 30 * 60, STA_Mode);
            break;
        default:
            break;
        }
    }
}
void task(unsigned long shutdown_tim1, unsigned long delay_tim2, char mode)
{
    unsigned long tick_s_temp1, tick_s_temp2, tick_s_sleep;
    unsigned long i = 0;
    char flag1 = 0, flag2 = 0;
    tick_s_temp1 = tick_s;
    while (1)
    {
        // if ((tick_s - tick_s_temp1) > shutdown_tim1)
        // {
        //     printf("%lu,%lu,%lu,%lu\r\n", tick_s, tick_s_temp1, (tick_s - tick_s_temp1), shutdown_tim1);
        //     //break;
        // }
        if (STA_F == 1)
        {
            if (mode == mode1) //振动模式
            {
                zhendong_mode();
            }
            else if (mode == mode2) //敲打模式
            {
                qiaoda_mode();
            }
            else if (mode == mode3) //混合模式
            {
                hunhe_mode();
            }
        }
        else
        {
            tick_s_temp2 = tick_s;
            while ((tick_s - tick_s_temp2) < delay_tim2)
            {
                if (mode == mode1) //振动模式
                {
                    zhendong_mode();
                }
                else if (mode == mode2) //敲打模式
                {
                    qiaoda_mode();
                }
                else if (mode == mode3) //混合模式
                {
                    hunhe_mode();
                }
            }
        }
        tick_s_temp2 = tick_s;
        while ((tick_s - tick_s_temp2) < delay_tim2)
        {
        }
    }
    // shutdown();
}
// void shutdown(void)
// {
//     STA_main = 0x00;
//     motor_set(1, 0);
//     motor_set(2, 0);
//     display_user(0x00, 0, 0, 0);
//     while (1)
//     {

//         printf("Shutdown!\r\n");
//         delay_ms(100);
//     }
// }
