#ifndef __DS1302_H
#define __DS1302_H
#include "main.h"

struct DS1302_time{
    u8 hour;
    u8 min;
    u8 sec;
};
void get_time(void);
void DS1302_Init(void);
#endif
