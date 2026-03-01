#ifndef FLASH_PROGRAM_H
#define FLASH_PROGRAM_H

#include <stdint.h>
#include "stm32f1xx_hal.h"


#define FBL_VERSION_ADDR    0x08007FF0  // FBL版本号地址
#define APP_VALID_FLAG_ADDR 0x0800A000  // APP有效标志地址
#define FLASH_BASE_ADDR     0x08000000  // STM32 Flash起始地址
#define FLASH_END_ADDR      0x0807FFFF  // STM32F103ZET6 Flash结束地址（512K）

HAL_StatusTypeDef flash_write_data(uint32_t addr, const uint8_t* data, uint16_t len);

#define Set_APP_valid_flag()    flash_write_data(APP_VALID_FLAG_ADDR, (const uint8_t*)&(uint32_t){0x5A5A5A5A}, 4)
#endif /* FLASH_PROGRAM_H */
