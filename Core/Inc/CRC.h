#ifndef __CRC_H__
#define __CRC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "CRC.h"


extern unsigned short crc_1;
extern void crc_init(void);
unsigned short crc16_ccitt(unsigned char *data, unsigned short length);

#endif






