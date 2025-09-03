#include "boot_jump.h"





/**
 * @brief 从Bootloader跳转到应用程序
 * @retval 无（成功跳转后不会返回）
 */
void Boot_JumpToApplication(void)
{
    pFunction Jump_To_Application;
    uint32_t JumpAddress;
    
    // 检查应用程序栈顶地址是否有效（必须在RAM范围内）
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        
        // 关闭所有外设中断
        for (int i = 0; i < 8; i++) {
            NVIC->ICER[i] = 0xFFFFFFFF;  // 清除使能
            NVIC->ICPR[i] = 0xFFFFFFFF;  // 清除挂起
        }
        __disable_irq();  // 关闭全局中断
        
        // 关闭所有外设时钟，降低功耗并避免干扰
        HAL_RCC_DeInit();


        // 关闭SysTick定时器
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;
        
        // 获取应用程序的复位函数地址（栈顶地址+4）
        JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);
        Jump_To_Application = (pFunction)JumpAddress;
        
        // 设置主堆栈指针（MSP）为应用程序的栈顶地址
        __set_MSP(*(__IO uint32_t*)APPLICATION_ADDRESS);
        
        // 跳转到应用程序
        Jump_To_Application();
    }
}
    