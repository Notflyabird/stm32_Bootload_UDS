#include "flash_program.h"
#include "iwdg.h"


const uint32_t fbl_version __attribute__((section(".fbl_version"))) = 0x20260301; // v1.0

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
HAL_StatusTypeDef flash_write_data(uint32_t addr, const uint8_t* data, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t i;
    uint32_t word;
    uint8_t bytesToWrite;
    uint8_t j;
    
    /* Unlock Flash */
    HAL_FLASH_Unlock();
    
    /* Write data word-by-word (STM32F1 requires 32-bit writes) */
    for(i = 0; i < len; i += 4)
    {
        word = 0;
        bytesToWrite = (len - i < 4) ? (len - i) : 4;
        
        /* Pack bytes into a 32-bit word (little-endian) */
        for(j = 0; j < bytesToWrite; j++)
        {
            word |= ((uint32_t)data[i + j] << (j * 8));
        }
        
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, word);
        if(status != HAL_OK)
        {
            break;
        }
        
        /* Feed watchdog every 2 words (every 8 bytes) to prevent timeout during long writes */
        if((i % 8) == 0)
        {
            HAL_IWDG_Refresh(&hiwdg);
        }
    }
    
    /* Lock Flash */
    HAL_FLASH_Lock();
    
    return status;
}


/**
 * @brief  读取Flash指定地址的4个字节内容（核心函数）
 */
HAL_StatusTypeDef flash_read_4bytes(uint32_t flash_addr, uint8_t *read_buf)
{
    // 1. 参数合法性检查
    if (read_buf == NULL)
    {
        return HAL_ERROR; // 缓冲区NULL
    }
    
    // 2. 检查地址是否在Flash有效范围内，且剩余空间≥4字节
    if (flash_addr < FLASH_BASE_ADDR || flash_addr + 3 > FLASH_END_ADDR)
    {
        return HAL_ERROR; // 地址非法/超出范围
    }

    // 3. 直接读取Flash内容（Flash只读，无需解锁）
    // 方式1：逐字节读取（直观，适合新手）
    read_buf[0] = *(uint8_t *)(flash_addr + 0);
    read_buf[1] = *(uint8_t *)(flash_addr + 1);
    read_buf[2] = *(uint8_t *)(flash_addr + 2);
    read_buf[3] = *(uint8_t *)(flash_addr + 3);

    // 方式2：按32位读取（效率更高，等价于方式1）
    // uint32_t flash_data = *(uint32_t *)flash_addr;
    // memcpy(read_buf, &flash_data, 4);

    return HAL_OK;
}

/**
 * @brief  读取FBL版本号（封装函数，简化调用）
 */
HAL_StatusTypeDef fbl_version_read(uint8_t *version_buf)
{
    return flash_read_4bytes(FBL_VERSION_ADDR, version_buf);
}

/**
 * @brief  读取APP有效标志（封装函数，简化调用）
 */
HAL_StatusTypeDef app_valid_flag_read(uint8_t *flag_buf)
{
    return flash_read_4bytes(APP_VALID_FLAG_ADDR, flag_buf);
}

