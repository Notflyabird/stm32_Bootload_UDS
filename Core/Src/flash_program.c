#include "flash_program.h"
#include "iwdg.h"

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
