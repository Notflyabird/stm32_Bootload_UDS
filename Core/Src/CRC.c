#include "CRC.h"


unsigned short crc_1 = 0xffff;


void crc_init() {
    crc_1 = 0xffff; // 初始化CRC寄存器
}

/**
 * @brief 计算CRC16-CCITT (多项式0x1021, 初始值0xFFFF)
 * @param data 数据指针
 * @param length 数据长度（字节数）
 * @return CRC16校验值
 */

unsigned short crc16_ccitt(unsigned char *data, unsigned short length) {
  
    for (unsigned short i = 0; i < length; ++i) {
        crc_1 ^= (unsigned short)(data[i] << 8); // 将当前字节左移8位后与crc异或
        for (int j = 0; j < 8; ++j) {
            if (crc_1 & 0x8000) { // 检查最高位
                crc_1 = (crc_1 << 1) ^ 0x1021;
            } else {
                crc_1 = crc_1 << 1;
            }
        }
    }
    return crc_1 & 0xFFFF; // 确保16位
}









