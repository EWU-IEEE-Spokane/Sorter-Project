#include <stdint.h>
#include "clock_init.h"

void setup_clock() {
	//initializes and configures the main clock that will be initially used and modified during runtime clock configuration. 
	//The clock-control block diagram can be found on page 222 of the datasheet.
	uint32_t * rcc = (uint32_t *)(0x400FE000 + 0x60);		//run-mode clock control register p.254
	uint32_t * rcc2 = (uint32_t *)(0x400FE000 + 0x70);	//run-mode clock control 2 register p.260 of datasheet

	*rcc2 = *rcc2 | (1<<31);		//RCC2 overrides RCC, so SYSDIV2 governs clock division
	*rcc2 = *rcc2 | (1<<11);		//bypass the PLL for safety while changing oscillator source (BYPASS2)
	*rcc = *rcc &(~(1<<22));		//make bit 22 low so that the system clock is undivided
	*rcc = *rcc & (~(0x1F<<6));		//XTAL clear set value
	*rcc = *rcc | (0x15<<6);		//XTAL set to 16mhz - tells PLL that its input is 16MHz
	*rcc2 = *rcc2 &(~(0xF<<4));		//nulls OSCSRC2, shuts off 32.768khz oscillator and uses 16MHz external crystal
	*rcc2 = *rcc2 &(~(0x1<<13));	//PWRON PLL2, the phase-lock-loop that boosts the source oscillator to 200MHz
	*rcc2 = *rcc2 & (~(0x3f<<23));	//clears SYSDIV2 bits 23 through 28, making the clock divider 1
	*rcc2 = *rcc2|(0x3<<23);		//assign SYSDIV2 to divide by 4, so that system divider yields 50MHz
	*rcc = *rcc|(0x1<<22);			//enable the system clock divider defined by SYSDIV above
	wait_for_pll();					//wait for pll steady-state
	*rcc2 = *rcc2 &(~(0x1<<11));	//shut off PLL bypass2 now that its stabilized
}



void wait_for_pll(){
	//Provides a delay until the PLL asserts that it is in steady-state by providing an interrupt
	uint32_t * ris = (uint32_t *)(0x400FE000 + 0x50);		//raw interrupt status register p.244

	while((!(*ris))&(0x1<<6)){		//once bit 6 in ris goes high, PLL is settled; run until then
		continue;
	}
}