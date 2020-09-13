#include "interrupt.h"

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