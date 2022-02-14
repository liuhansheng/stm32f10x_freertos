#include "main.h"

#define __IO volatile

#define BOOT_ADDRESS 0x08000000

typedef void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t  JumpAddress;

void app_jump_to_boot(uint32_t magic_num)
{
    //HAL_PWR_EnableBkUpAccess();
    //__BKPSRAM_CLK_ENABLE();
    //BACK_RAM_RES = magic_num;

    HAL_DeInit();
    HAL_RCC_DeInit();
    
    // 关闭所有中断
    for (int i = NonMaskableInt_IRQn; i <= DMA2_Channel4_5_IRQn; i++)
    {
       HAL_NVIC_DisableIRQ(i);
       HAL_NVIC_ClearPendingIRQ(i);
    }
    __disable_irq();
    // 需复位systick才能正常跳转
    {
        SysTick->LOAD = 0;
        SysTick->VAL  = 0;
        SysTick->CTRL = 0;
    }

    {
        __set_BASEPRI(0);
        __set_FAULTMASK(0);
        __set_CONTROL(0);
        __set_PSP(*((volatile unsigned long int *)BOOT_ADDRESS));
        __set_MSP(*(__IO uint32_t *)BOOT_ADDRESS);
        __ISB();
    }

    {
        /* Jump to user application */
        JumpAddress         = *(__IO uint32_t *)(BOOT_ADDRESS + 4);
        Jump_To_Application = (pFunction)JumpAddress;
        /* Initialize user application's Stack Pointer */
        // SCB->VTOR = BOOT_ADDRESS;
        HAL_NVIC_SystemReset();
        // Jump_To_Application();
    }
}