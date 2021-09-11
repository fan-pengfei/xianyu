#include "bsp_lib.h"
#include "main.h"
#include "intrins.h"
#include "ir.h"
#include "smg.h"
#include "stdio.h"
volatile u8 STA_P = 1; //速度默认是p1，最慢的速度
volatile u8 STA_F = 1; //默认是F1模式
volatile u8 STA_Mode = 0;
volatile u8 STA_main = 0x00; //主要的状态标志位
//第一位代表机器是否在工作
//第二位代表是否开启了预约功能
void delay_ms(unsigned int nms) //22.1184MHz
{
    unsigned int cnt;
    unsigned char i, j;
    for (cnt = 0; cnt < nms; cnt++)
    {
        i = 29;
        j = 183;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}
void delay_us(unsigned int nus)
{
    unsigned char i;
    unsigned int cnt;
    for (cnt = 0; cnt < nus; cnt++)
    {
        _nop_();
        i = 5;
        while (--i)
            ;
    }
}
void delay_10us(unsigned int nus)
{
    unsigned char i;
    unsigned int cnt;
    for (cnt = 0; cnt < nus; cnt++)
    {
        _nop_();
        _nop_();
        i = 71;
        while (--i)
            ;
    }
}
void led_init(void)
{
    P0M1 = 0x00;
    P0M0 = 0x00;
    P1M1 = 0x00;
    P1M0 = 0x00;
    P2M1 = 0x00;
    P2M0 = 0x00;
    P3M1 = 0x00;
    P3M0 = 0x03;
    P4M1 = 0x00;
    P4M0 = 0x00;
    P5M1 = 0x00;
    P5M0 = 0xff;
    P6M1 = 0x00;
    P6M0 = 0x00;
    P7M1 = 0x00;
    P7M0 = 0x00;
    P1M0 = 0xff; //设置P1.0~P1.7为推挽输出模式
    P1M1 = 0x00;
}
void PCA_Init()
{
    CCON = 0x00; //关闭PCA计数器，清除相关标志位
    CMOD = 0x0e; //PCA时钟源为系统时钟2分频  33.1776/2/1024=0.0162MHZ ==>16.2KHZ
    CL = 0x00;   //计数器清零
    CH = 0x00;
    /*------------------------PWM0部分-----------------------------*/
    CCAPM0 = 0x40;   //失能PCA模块0_PWM输出
    CCAP0L = 0X00;   //捕获比较寄存器低8位，比较值
    CCAP0H = 0X00;   //捕获比较寄存器高8位，重装值
    PCA_PWM0 = 0xC0; //10位PWM输出
    CCAPM0 = 0x42;   //使能PCA模块0_PWM输出

    /*------------------------PWM1部分-----------------------------*/
    CCAPM1 = 0x40;   //失能PCA模块1_PWM输出
    CCAP1L = 0X00;   //捕获比较寄存器低8位，比较值
    CCAP1H = 0X00;   //捕获比较寄存器高8位，重装值
    PCA_PWM1 = 0xC0; //10位PWM输出
    CCAPM1 = 0x42;   //使能PCA模块1_PWM输出
    /*------------------------PWM2部分-----------------------------*/
    CCAPM2 = 0x40;   //失能PCA模块2_PWM输出
    CCAP2L = 0X00;   //捕获比较寄存器低8位，比较值
    CCAP2H = 0X00;   //捕获比较寄存器高8位，重装值
    PCA_PWM2 = 0xC0; //10位PWM输出
    CCAPM2 = 0x42;   //使能PCA模块2_PWM输出
    CCON |= 1 << 6;  //启动计数器
}
//设置脉冲宽度
void PWM0_Set_Duty(u16 Duty)
{
    //注意：在更新 10 位 PWM 的重载值时，必须先写高两位 XCCAPnH[1:0]，再写低 8 位 CCAPnH[7:0]。

    CCAPM0 = 0x40;                  //失能PCA模块0_PWM输出
    PCA_PWM0 &= ~(3 << 4);          //清零重装值高2位
    PCA_PWM0 |= (Duty >> 4) & 0x30; //设置新的重装值高2位
    CCAP0H = Duty;                  //重装值低8位
    CCAPM0 = 0x42;                  //使能PCA模块0_PWM输出
}
//设置脉冲宽度
void PWM1_Set_Duty(u16 Duty)
{
    //注意：在更新 10 位 PWM 的重载值时，必须先写高两位 XCCAPnH[1:0]，再写低 8 位 CCAPnH[7:0]。

    CCAPM1 = 0x40;                  //失能PCA模块1_PWM输出
    PCA_PWM1 &= ~(3 << 4);          //清零重装值高2位
    PCA_PWM1 |= (Duty >> 4) & 0x30; //设置新的重装值高2位
    CCAP1H = Duty;                  //重装值低8位
    CCAPM1 = 0x42;                  //使能PCA模块0_PWM输出
}
//设置脉冲宽度
void PWM2_Set_Duty(u16 Duty)
{
    //注意：在更新 10 位 PWM 的重载值时，必须先写高两位 XCCAPnH[1:0]，再写低 8 位 CCAPnH[7:0]。
    CCAPM2 = 0x40;                  //失能PCA模块2_PWM输出
    PCA_PWM2 &= ~(3 << 4);          //清零重装值高2位
    PCA_PWM2 |= (Duty >> 4) & 0x30; //设置新的重装值高2位
    CCAP2H = Duty;                  //重装值低8位
    CCAPM2 = 0x42;                  //使能PCA模块2_PWM输出
}
void motor_set(char num, int speed)
{
    if (num == motor1)
    {
        if (speed > 0)
        {
            PWM0_Set_Duty(1023 - speed);
            PWM1_Set_Duty(1023);
        }
        else
        {
            PWM0_Set_Duty(1023);
            PWM1_Set_Duty(1023 - (-speed));
        }
    }
    else
    {
        PWM3 = 0;
        if (speed > 0)
        {
            PWM2_Set_Duty(1023 - speed);
        }
        else
        {
            PWM2_Set_Duty(1023 - (-speed));
        }
    }
}
#define FOSC 22118400L //系统频率
#define fre 1000
#define T0_TIM (65536 - (FOSC / 1 / fre))
void Timer0Init(void) //1微秒@11.0592MHz
{
    P5M0 = 0x00;
    P5M1 = 0x00;
    AUXR |= 0x80;       //定时器时钟1T模式
    TMOD &= 0xF0;       //设置定时器模式
    TL0 = T0_TIM % 256; //装载低8位计时值
    TH0 = T0_TIM / 256; //装载高8位
    TF0 = 0;            //清除TF0标志
    TR0 = 1;            //定时器0开始计时
    ET0 = 1;            //使能定时器中断
    EA = 1;
}
void UartInit(void) //115200bps@22.1184MHz
{
    SCON = 0x50;  //8位数据,可变波特率
    AUXR |= 0x40; //定时器时钟1T模式
    AUXR &= 0xFE; //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F; //设置定时器模式
    TL1 = 0xD0;   //设置定时初始值
    TH1 = 0xFF;   //设置定时初始值
    ET1 = 0;      //禁止定时器%d中断
    TR1 = 1;      //定时器1开始计时
}
/*implemented for printf() */
char putchar(char c)
{
    SBUF = c;
    while (TI == 0)
        ;
    TI = 0;
    return c;
}
extern u8 gired_data[4];
extern u8 key_table[17];
volatile unsigned long tick_ms = 0;
volatile unsigned long tick_s = 0;
volatile char char1, char2, char3;
volatile u8 smg_num = 0x00;
volatile u8 key_flag = 29;
void smg_display(u8 num, char s1, char s2, char s3);
void timer0_ISR(void) interrupt 1 using 0
{
    tick_ms++;
    if (tick_ms % 1000 == 0)
    {
        tick_s++;
        //  printf("%bd,%bu", flag_running, key_flag);
    }
    if (((STA_main & 0x80) == 0x80))
    {
        smg_num = 0x07;
        if (STA_Mode == mode1)
        {
            char1 = 20;
        }
        else if (STA_Mode == mode2)
        {
            char1 = 18;
        }
        else if (STA_Mode == mode3)
        {
            char1 = 17;
        }
        char2 = 15;
        char3 = STA_F;
    }
    if ((STA_main & 0x40) == 0x40)
    {
        smg_num = 0x07;
        char1 = 8;
        char2 = 8;
        char3 = 8;
    }
    smg_display(smg_num, char1, char2, char3);
}
void smg_display(u8 num, char s1, char s2, char s3)
{
    if (num == 0)
    {
        display(0, 0);
    }
    else if (num == 1)
    {
        display(3, s3);
    }
    else if (num == 2)
    {
        display(2, s2);
    }
    else if (num == 4)
    {
        display(1, s1);
    }
    else if (num == 3)
    {
        if (tick_ms % 2 == 1)
        {
            display(2, s2);
        }
        else if (tick_ms % 2 == 0)
        {
            display(3, s3);
        }
    }
    else if (num == 5)
    {
        if (tick_ms % 2 == 1)
        {
            display(1, s1);
        }
        else if (tick_ms % 2 == 0)
        {
            display(3, s3);
        }
    }
    else if (num == 6)
    {
        if (tick_ms % 2 == 1)
        {
            display(1, s1);
        }
        else if (tick_ms % 2 == 0)
        {
            display(2, s2);
        }
    }
    else if (num == 7)
    {
        if (tick_ms % 3 == 1)
        {
            display(1, s1);
        }
        else if (tick_ms % 3 == 2)
        {
            display(2, s2);
        }
        else if (tick_ms % 3 == 0)
        {
            display(3, s3);
        }
    }
}
void display_user(u8 num, char s1, char s2, char s3)
{
    smg_num = num;
    char1 = s1;
    char2 = s2;
    char3 = s3;
}
void zhendong_mode(void)
{
    static u16 i;
    char tick_ms_temp, flag = 0;
    i++;
    tick_ms_temp = tick_ms;
    while ((tick_ms - tick_ms_temp) < 1)
        ;
    if (i % 500 == 0)
    {
        flag = !flag;
        if (flag)
        {
            motor_set(2, STA_P * 100);
        }
        else
        {
            motor_set(2, 0);
        }
    }
}
void qiaoda_mode(void)
{
    static u16 i;
    char tick_ms_temp, flag = 0;
    i++;
    tick_ms_temp = tick_ms;
    while ((tick_ms - tick_ms_temp) < 1)
        ;
    if (i % 500 == 0)
    {
        flag = !flag;
        if (flag)
        {
            motor_set(2, STA_P * 100);
        }
        else
        {
            motor_set(2, 0);
        }
    }
}
void hunhe_mode(void)
{
    static u16 i;
    char tick_ms_temp, flag1 = 0, flag2 = 0;
    i++;
    tick_ms_temp = tick_ms;
    while ((tick_ms - tick_ms_temp) < 1)
        ;
    if (i % 1000 == 0)
    {
        flag1 = !flag1;
        if (flag1)
        {
            motor_set(1, STA_P * 100);
        }
        else
        {
            motor_set(1, -STA_P * 100);
        }
    }
    if (i % 500 == 0)
    {
        flag2 = !flag2;
        if (flag2)
        {
            motor_set(2, STA_P * 100);
        }
        else
        {
            motor_set(2, 0);
        }
    }
}