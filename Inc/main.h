#ifndef __MAIN_H
#define __MAIN_H
#include "stc8g.h"
#include "stdio.h"
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned
#define LED_Pin P12
//红外接收IO定义
#define IRED P32
//数码管IO定义
#define SMG_1 P34
#define SMG_2 P35
#define SMG_3 P36
#define SMG_IO1 P30
#define SMG_IO2 P31
#define SMG_IO3 P12
#define SMG_IO4 P13
#define SMG_IO5 P14
#define SMG_IO6 P15
#define SMG_IO7 P16
#define SMG_IO8 P17
//敲打电机IO定义，电机满速是1024
#define PWM1 P10
#define PWM2 P11
//振动电机IO
#define PWM3 P33
#define PWM4 P37 //pwm口
//枚举电机类型
enum
{
    motor1 = 1, //敲打电机
    motor2,     //振动电机
};
#endif // !__MAIN_H
