#ifndef FLASH_PROGRAM_H
#define FLASH_PROGRAM_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

/******************************************************************************
* 函数名称: HAL_StatusTypeDef flash_write_data(uint32_t addr, const uint8_t* data, uint16_t len)
* 功能说明: 将数据写入 Flash
* 输入参数: uint32_t addr           --写入起始地址
    　　　　const uint8_t* data     --待写入数据缓冲区
    　　　　uint16_t len            --数据长度
* 输出参数: 无
* 函数返回: HAL_OK: 成功; 其他值: 失败
* 其它说明: 使用 FLASH_TYPEPROGRAM_WORD (32-bit) 进行编程
******************************************************************************/
HAL_StatusTypeDef flash_write_data(uint32_t addr, const uint8_t* data, uint16_t len);

#endif /* FLASH_PROGRAM_H */
