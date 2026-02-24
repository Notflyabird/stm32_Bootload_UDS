#ifndef FLASH_ERASE_H
#define FLASH_ERASE_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef flash_erase_region(uint32_t startAddr, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif // FLASH_ERASE_H
