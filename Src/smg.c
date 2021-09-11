#include "smg.h"
#include "bsp_lib.h"
unsigned char xdata smgduan[21] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d,
                                  0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x79, 0x76, 0x38, 0x73, 0x3e};
//	{'E', 0x79},{'H', 0x76},{'L', 0x38},{'P', 0x73},{'U', 0x3E},

void display(u8 cnt, u8 hex)
{
    if (cnt == 3)
    {
        SMG_1 = 0;
        SMG_2 = 1;
        SMG_3 = 1;
        SMG_IO1 = smgduan[hex] & (0x01 << 0);
        SMG_IO2 = smgduan[hex] & (0x01 << 1);
        SMG_IO3 = smgduan[hex] & (0x01 << 2);
        SMG_IO4 = smgduan[hex] & (0x01 << 3);
        SMG_IO5 = smgduan[hex] & (0x01 << 4);
        SMG_IO6 = smgduan[hex] & (0x01 << 5);
        SMG_IO7 = smgduan[hex] & (0x01 << 6);
        SMG_IO8 = smgduan[hex] & (0x01 << 7);
    }
    else if (cnt == 2)
    {
        SMG_1 = 1;
        SMG_2 = 0;
        SMG_3 = 1;
        SMG_IO1 = smgduan[hex] & (0x01 << 0);
        SMG_IO2 = smgduan[hex] & (0x01 << 1);
        SMG_IO3 = smgduan[hex] & (0x01 << 2);
        SMG_IO4 = smgduan[hex] & (0x01 << 3);
        SMG_IO5 = smgduan[hex] & (0x01 << 4);
        SMG_IO6 = smgduan[hex] & (0x01 << 5);
        SMG_IO7 = smgduan[hex] & (0x01 << 6);
        SMG_IO8 = smgduan[hex] & (0x01 << 7);
    }
    else if (cnt == 1)
    {
        SMG_1 = 1;
        SMG_2 = 1;
        SMG_3 = 0;
        SMG_IO1 = smgduan[hex] & (0x01 << 0);
        SMG_IO2 = smgduan[hex] & (0x01 << 1);
        SMG_IO3 = smgduan[hex] & (0x01 << 2);
        SMG_IO4 = smgduan[hex] & (0x01 << 3);
        SMG_IO5 = smgduan[hex] & (0x01 << 4);
        SMG_IO6 = smgduan[hex] & (0x01 << 5);
        SMG_IO7 = smgduan[hex] & (0x01 << 6);
        SMG_IO8 = smgduan[hex] & (0x01 << 7);
    }
    else
    {
        SMG_1 = 1;
        SMG_2 = 1;
        SMG_3 = 1;
    }
}