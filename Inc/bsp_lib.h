#ifndef __BSP_LIB_H_
#define __BSP_LIB_H_
#include "main.h"
sfr PWMCFG = 0xf1;
sfr PWMIF = 0xf6;
sfr PWMFDCR = 0xf7;
sfr PWMCR = 0xfe;

#define PWMC (*(unsigned int volatile xdata *)0xfff0)
#define PWMCKS (*(unsigned char volatile xdata *)0xfff2)
#define TADCP (*(unsigned int volatile xdata *)0xfff3)
#define PWM0T1 (*(unsigned int volatile xdata *)0xff00)
#define PWM0T2 (*(unsigned int volatile xdata *)0xff02)
#define PWM0CR (*(unsigned char volatile xdata *)0xff04)
#define PWM0HLD (*(unsigned char volatile xdata *)0xff05)
#define PWM1T1 (*(unsigned int volatile xdata *)0xff10)
#define PWM1T2 (*(unsigned int volatile xdata *)0xff12)
#define PWM1CR (*(unsigned char volatile xdata *)0xff14)
#define PWM1HLD (*(unsigned char volatile xdata *)0xff15)
#define PWM2T1 (*(unsigned int volatile xdata *)0xff20)
#define PWM2T2 (*(unsigned int volatile xdata *)0xff22)
#define PWM2CR (*(unsigned char volatile xdata *)0xff24)
#define PWM2HLD (*(unsigned char volatile xdata *)0xff25)
#define PWM3T1 (*(unsigned int volatile xdata *)0xff30)
#define PWM3T2 (*(unsigned int volatile xdata *)0xff32)
#define PWM3CR (*(unsigned char volatile xdata *)0xff34)
#define PWM3HLD (*(unsigned char volatile xdata *)0xff35)
#define PWM4T1 (*(unsigned int volatile xdata *)0xff40)
#define PWM4T2 (*(unsigned int volatile xdata *)0xff42)
#define PWM4CR (*(unsigned char volatile xdata *)0xff44)
#define PWM4HLD (*(unsigned char volatile xdata *)0xff45)
#define PWM5T1 (*(unsigned int volatile xdata *)0xff50)
#define PWM5T2 (*(unsigned int volatile xdata *)0xff52)
#define PWM5CR (*(unsigned char volatile xdata *)0xff54)
#define PWM5HLD (*(unsigned char volatile xdata *)0xff55)
#define PWM6T1 (*(unsigned int volatile xdata *)0xff60)
#define PWM6T2 (*(unsigned int volatile xdata *)0xff62)
#define PWM6CR (*(unsigned char volatile xdata *)0xff64)
#define PWM6HLD (*(unsigned char volatile xdata *)0xff65)
#define PWM7T1 (*(unsigned int volatile xdata *)0xff70)
#define PWM7T2 (*(unsigned int volatile xdata *)0xff72)
#define PWM7CR (*(unsigned char volatile xdata *)0xff74)
#define PWM7HLD (*(unsigned char volatile xdata *)0xff75)

enum
{
    mode1 = 1, //振动模式
    mode2,     //敲打模式
    mode3,     //混合模式
};
void delay_ms(unsigned int nms);
void delay_10us(unsigned int ten_us);
void delay_us(unsigned int nus);
void led_init(void);
void Timer0Init(void);
void Timer1Init(void);
void Timer2Init(void);
void PCA_Init();
void motor_set(char num, int speed);
void UartInit(void); //9600bps@11.0592MHz
char putchar(char c);
void smg_display(u8 num, char s1, char s2, char s3);
void display_user(u8 num, char s1, char s2, char s3);
void zhendong_mode(void);
void qiaoda_mode(void);
void hunhe_mode(void);
void stop_mode(void);
//设置脉冲宽度
void PWM0_Set_Duty(u16 Duty);

//设置脉冲宽度;
void PWM1_Set_Duty(u16 Duty);

//设置脉冲宽度
void PWM2_Set_Duty(u16 Duty);

#endif
