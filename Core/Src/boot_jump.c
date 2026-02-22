#include "boot_jump.h"





/**
 * @brief Jump from Bootloader to Application
 * @retval None (will not return after successful jump)
 */
void Boot_JumpToApplication(void)
{
    pFunction Jump_To_Application;
    uint32_t JumpAddress;
    
    // Check if the application stack top address is valid (must be within RAM range)
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        
        // Disable all peripheral interrupts
        for (int i = 0; i < 8; i++) {
            NVIC->ICER[i] = 0xFFFFFFFF;  // Clear enable
            NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear pending
        }
        __disable_irq();  // Disable global interrupts
        
        // Disable all peripheral clocks to reduce power consumption and avoid interference
        HAL_RCC_DeInit();


        // Disable SysTick timer
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;
        
        // Get the reset function address of the application (stack top address + 4)
        JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);
        Jump_To_Application = (pFunction)JumpAddress;
        
        // Set the main stack pointer (MSP) to the application's stack top address
        __set_MSP(*(__IO uint32_t*)APPLICATION_ADDRESS);
        
        // Jump to application
        Jump_To_Application();
    }
}
    