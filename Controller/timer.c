#include "timer.h"

void Disable_Interrupts(void)
{
__asm (" CPSID I\n"
        " BX LR\n");
}
void Enable_Interrupts(void)
{
__asm (" CPSIE I\n"
        " BX LR\n");
}

void Config_SysTick(void)
{
    NVIC_ST_CTRL_R = 0x0;
    NVIC_ST_RELOAD_R = 0x00FFFFFF;
    NVIC_ST_CURRENT_R = 0x0;
    NVIC_ST_CTRL_R = 0x00000005;
}

void SysTick_delay(void)
{
    NVIC_ST_RELOAD_R = 0xC34F;                      // Load value for 1 ms delay
    NVIC_ST_CURRENT_R = 0x0;                        // Clear current and start timer
    while((NVIC_ST_CTRL_R & 0x00010000) == 0){};    // Wait until COUNT flag is set
}

void ms_delay(unsigned long count)
{
    unsigned long i;
    for(i=0; i<count; i++)
    {
        SysTick_delay();
    }
}