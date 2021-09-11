#include "main.h"
#include "ir.h"
#include "bsp_lib.h"
volatile u8 gired_data[4];
volatile u8 key_table[17] = {69, 70, 71, 68, 64, 67,
                             7, 21, 9, 25, 22, 13, 24, 82, 8, 90, 28};
extern u8 key_flag;
extern u8 STA_Mode;
void IR_init(void)
{
    IT0 = 1; //使能 INT0 下降沿中断
    EX0 = 1; //使能 INT0 中断
    EA = 1;
    IRED = 1; //I/O口初始化
}
extern char flag_running;
extern u8 STA_F;        //默认是F1模式
extern u8 STA_main;     //主要的状态标志位
void ired() interrupt 0 //外部中断0服务函数
{
    u8 ired_high_time = 0;
    u16 time_cnt = 0;
    u8 i = 0, j = 0;
    static char flag_lock = 0;
    if (IRED == 0)
    {
        time_cnt = 15;
        while ((!IRED) && (time_cnt)) //等待引导信号9ms低电平结束，若超过10ms强制退出
        {
            delay_ms(1);
            time_cnt--;
            if (time_cnt == 0)
                return;
        }
        if (IRED) //引导信号9ms低电平已过，进入4.5ms高电平
        {
            time_cnt = 7;
            while (IRED && time_cnt) //等待引导信号4.5ms高电平结束，若超过5ms强制退出
            {
                delay_ms(1);
                time_cnt--;
                if (time_cnt == 0)
                    return;
            }
            for (i = 0; i < 4; i++) //循环4次，读取4个字节数据
            {
                for (j = 0; j < 8; j++) //循环8次读取每位数据即一个字节
                {
                    // time_cnt = 600;
                    while ((IRED == 0) && time_cnt) //等待数据1或0前面的0.56ms结束，若超过6ms强制退出
                    {
                        delay_us(5);
                        // time_cnt--;
                        // if (time_cnt == 0)
                        //     return;
                    }
                    time_cnt = 15;
                    while (IRED) //等待数据1或0后面的高电平结束，若超过2ms强制退出
                    {
                        delay_10us(10); //约0.1ms
                        ired_high_time++;
                        if (ired_high_time > 20)
                            return;
                    }
                    gired_data[i] >>= 1;     //先读取的为低位，然后是高位
                    if (ired_high_time >= 8) //如果高电平时间大于0.8ms，数据则为1，否则为0
                        gired_data[i] |= 0x80;
                    ired_high_time = 0; //重新清零，等待下一次计算时间
                }
            }
        }
        if (gired_data[2] != ~gired_data[3]) //校验控制码与反码，错误则返回
        {

            for (i = 0; i < 4; i++)
                gired_data[i] = 0;
            return;
        }
        for (i = 0; i < 17; i++)
        {
            if (gired_data[2] == key_table[i])
            {
                key_flag = i;
                if (key_flag == 16)
                {

                    if (flag_lock % 2 == 0)
                    {
                        STA_main = STA_main | 0x40;
                    }
                    else
                    {
                        STA_main = STA_main & 0xbf;
                    }
                    flag_lock++;
                }
                if ((STA_main & 0x40) != 0x40)
                {
                    if ((key_flag == 15) && ((STA_main & 0x80) == 0x80))
                    {
                        STA_F++;
                        if (STA_F > 9)
                        {
                            STA_F = 9;
                        }
                    }
                    else if ((key_flag == 14) && ((STA_main & 0x80) == 0x80))
                    {
                        STA_F--;
                        if (STA_F < 1)
                        {
                            STA_F = 1;
                        }
                    }
                    else if ((key_flag == 3) || (key_flag == 4) || (key_flag == 5))
                    {
                        STA_Mode = key_flag - 2;
                    }
                }
            }
        }
    }
}
