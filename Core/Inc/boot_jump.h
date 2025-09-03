#ifndef BOOT_JUMP_H
#define BOOT_JUMP_H


#include "stm32f1xx_hal.h"


// 应用程序起始地址（与链接脚本中的FLASH起始地址一致）
#define APPLICATION_ADDRESS 0x08008000

// 函数指针类型定义（用于指向应用程序的复位向量）
typedef void (*pFunction)(void);
void Boot_JumpToApplication(void);

#endif  