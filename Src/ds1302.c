#include "main.h"
#include "bsp_lib.h"
#include <stdio.h>
#include "ds1302.h"
struct DS1302_time my_time;
// 各寄存器的写命令。KEIL对中文支持不太好，原文注释这里原样表述一遍
#define SECOND_REG_WRITE 0x80
#define MINUTE_REG_WRITE 0x82
#define HOUR_REG_WRITE 0x84
#define DAY_REG_WRITE 0x86
#define MONTH_REG_WRITE 0x88
#define WEEK_REG_WRITE 0x8A
#define YEAR_REG_WRITE 0x8C

// 各寄存器的读命令
#define SECOND_REG_READ 0x81
#define MINUTE_REG_READ 0x83
#define HOUR_REG_READ 0x85
#define DAY_REG_READ 0x87
#define MONTH_REG_READ 0x89
#define WEEK_REG_READ 0x8B
#define YEAR_REG_READ 0x8D

// 向DS1302写入一个字节
void DS1302_WriteByte(u8 writeByte)
{
    u8 retry = 0;
    // DS1302 接收或发送数据时，低位在前，高位在后
    for (retry = 0; retry != 8; ++retry)
    {
        if (1 == (writeByte & 0x01))
        {
            DS1302_IO = 1;
        }
        else
        {
            DS1302_IO = 0;
        }
        // 参考了别人的例程，这里竟然不需要延时..好吧
        DS1302_SCK = 1;
        DS1302_SCK = 0;
        writeByte >>= 1;
    }
}

// 从DS1302中读出一个字节
u8 DS1302_ReadByte(void)
{
    u8 retry = 0;
    u8 readByte = 0;
    // 最低位在前，高位在后。跟一般的通讯不太一样
    for (retry = 0; retry != 8; ++retry)
    {
        readByte >>= 1;
        if (DS1302_IO)
        {
            readByte |= 0x80;
        }
        DS1302_SCK = 1;
        DS1302_SCK = 0;
    }
    return readByte;
}

// 向某个寄存器写入一个字节数值
void DS1302_WriteReg(u8 addr, u8 value)
{
    DS1302_RST = 0;
    DS1302_SCK = 0;
    DS1302_RST = 1;
    DS1302_WriteByte(addr);
    DS1302_WriteByte(value);
    DS1302_SCK = 1;
    DS1302_RST = 0;
}

// 读取某个寄存器的值
u8 DS1302_ReadReg(u8 addr)
{
    u8 readByte = 0;
    DS1302_RST = 0;
    DS1302_SCK = 0;
    DS1302_RST = 1;
    DS1302_WriteByte(addr);
    readByte = DS1302_ReadByte();
    DS1302_SCK = 1;
    DS1302_RST = 0;

    return readByte;
}

// DS1302的初始化
void DS1302_Init(void)
{
    // 先关闭写保护
    DS1302_WriteReg(0x8E, 0x00);
    // 设置时间，秒、分、时等等，注意噢，写入的是BCD码
    DS1302_WriteReg(SECOND_REG_WRITE, 0);
    DS1302_WriteReg(MINUTE_REG_WRITE, 0x20);
    DS1302_WriteReg(HOUR_REG_WRITE, 0x17);
    DS1302_WriteReg(DAY_REG_WRITE, 0x52);
    DS1302_WriteReg(MONTH_REG_WRITE, 0x12);
    DS1302_WriteReg(YEAR_REG_WRITE, 0x13);
    DS1302_WriteReg(WEEK_REG_WRITE, 0x04);

    // 一些设置后，继续开户写保护
    DS1302_WriteReg(0x90, 0x01);
    DS1302_WriteReg(0xC0, 0xF0);
    DS1302_WriteReg(0x8E, 0x80);
}

// 读时间，然后在八段数码管上显示出来
void get_time(void)
{
    u8 hour_, min_, sec_;

    hour_ = DS1302_ReadReg(HOUR_REG_READ);
    // BCD码转换成十六进制
    hour_ = (hour_ / 16) * 10 + (hour_ % 16);

    min_ = DS1302_ReadReg(MINUTE_REG_READ);
    min_ = (min_ / 16) * 10 + (min_ % 16);

    sec_ = DS1302_ReadReg(SECOND_REG_READ);
    sec_ = (sec_ / 16) * 10 + (sec_ % 16);
    my_time.hour=hour_;
    my_time.min=min_;
    my_time.sec=sec_;
}