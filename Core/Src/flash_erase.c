#include "flash_erase.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "iwdg.h"

/*
 * Erase flash region starting at startAddr for length bytes.
 * Returns HAL_OK on success, HAL_ERROR on failure.
 * This implementation executes from Flash (NOT placed to RAM).
 */
HAL_StatusTypeDef flash_erase_region(uint32_t startAddr, uint32_t length)
{
    if (length == 0u)
        return HAL_ERROR;

    /* prevent overflow */
    if (startAddr + length - 1u < startAddr)
        return HAL_ERROR;

    uint32_t endAddr = startAddr + length - 1u;
    if ((startAddr < FLASH_BASE) || (endAddr < FLASH_BASE) || (startAddr > FLASH_BANK1_END) || (endAddr > FLASH_BANK1_END) || (endAddr < startAddr))
        return HAL_ERROR;

    uint32_t startPageAddr = startAddr & ~(FLASH_PAGE_SIZE - 1u);
    uint32_t nbPages = ((endAddr - startPageAddr) / FLASH_PAGE_SIZE) + 1u;

    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError = 0xFFFFFFFFu;

    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Banks = FLASH_BANK_1;

    if (HAL_FLASH_Unlock() != HAL_OK)
        return HAL_ERROR;

    HAL_StatusTypeDef flash_ret = HAL_OK;
    for (uint32_t p = 0; p < nbPages; ++p)
    {
        eraseInit.NbPages = 1u;
        eraseInit.PageAddress = startPageAddr + p * FLASH_PAGE_SIZE;

        __disable_irq();
        flash_ret = HAL_FLASHEx_Erase(&eraseInit, &pageError);
        __enable_irq();

        /* feed watchdog to avoid reset during long erase */
        HAL_IWDG_Refresh(&hiwdg);

        if (flash_ret != HAL_OK)
            break;
    }

    HAL_FLASH_Lock();

    return flash_ret;
}
