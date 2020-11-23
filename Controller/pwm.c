#include "pwm.h"

void configure_pwm(){
    //This initialization is based on the information in the datasheet starting on page 1230, primarily the initialization instructions on page 1239

	//Clock gating to enable gpio port and pwm module
	unsigned long * rcc = (unsigned long *)(0x400FE000 + 0x60);		//run-mode clock control register p.254
    unsigned long * rcgcgpio 	= (unsigned long *)(0x400FE000+0x608); 		//Run-Mode Clock Gating Control registry, page 340 of datasheet
    unsigned long * rcgcPWM 	= (unsigned long *)(0x400FE000+0x640); 		//Run-Mode Clock Gating Control registry for PWM, page 354 of datasheet
	unsigned long * rcgc0 		= (unsigned long *)(0x400FE000 + 0x100);	//run-mode clock gating control register 0 for enabling PWM module, page 456
    //unsigned long * rcgc2 		= (unsigned long *)(0x400FE000 + 0x108);  	//LEGACY run-mode clock gating control register 2, for enabling port F, page 464

	//gpio stuff, for getting port ready for pwm
    unsigned long * gpioAFSEL 	= (unsigned long *)(PORT_F_BASE_ADDRESS + 0x420);  //Alternative function select for choosing function other than GPIO on Port F, page 671
    unsigned long * gpioPCTL 	= (unsigned long *)(PORT_F_BASE_ADDRESS + 0x52C);  //Port Control register for choosing PWM as the alternate function to use, pages 688, 1352
    unsigned long * gpioden 	= (unsigned long *)(PORT_F_BASE_ADDRESS + 0x51c );	//Digital enable registry address for enabling or disabling digital enable. Page 682 in the datasheet
    unsigned long * gpioAMSEL 	= (unsigned long *)(PORT_F_BASE_ADDRESS + 0x528 ); //analog function enable register

    //PWM specific stuff
    unsigned long * pwm2genb 	= (unsigned long *)(0x40029000 + 0x0E4); 	//use gen b register desc p1285
    unsigned long * pwm2load 	= (unsigned long *)(0x40029000 + 0x0D0);  	//counter load value setting register
    unsigned long * pwm2cmpB 	= (unsigned long *)(0x40029000 + 0x0DC); 	//comparator B for counter value comparison trigger, p 1281
	unsigned long * pwm2ctl 	= (unsigned long *)(0x40029000 + 0x0C0);   	//PWM control register, used to enable counters, page 1266
    unsigned long * pwm2enable 	= (unsigned long *)(0x40029000 + 0x008);	//PWM output enabling register, used to mask PWM at pins to diable output, page 1247
	//unsigned long * SRPWM 	= (unsigned long *)(0x400FE000 + 0x540);	//software reset for PWM, page 330

	*rcgcgpio |= (1<<5);	//enable and provide a clock to port F. This allows port F to operate by enabling its bus and register access
	
    *rcgc0 |= (0x1<<20);   //provide a clock to PWM module

	*gpioden = 0xFF;	//Enable all pins of port F using the digital enable register. Must be done after clock provided and 3 clocks occured
    //*rcgc2 |= (1<<5);    //provide a clock to port F (uneccessary, this register is the LEGACY version of rcgcgpio)
    *rcgcPWM |= (1<<1);		//enable and provide a clock to PWM module 1. This allows PWM mod 1 to operate by enabling its bus and register access
	
	*gpioAMSEL &= ~(1<<1);		//disable analog functions on pin 1
	
    *gpioAFSEL |= (7<<1);	//make Port F pin 3-1 an alternate function
    *gpioPCTL |= (0x5550); 	//set PF1 to get alt func M1PWM5 pages 689, 1352
     
    *rcc |= (1<<20);	//usepwmdiv page 254
	*rcc &= ~(0x7<<17);	//clear PWMdiv value as per initialization instructions
	*rcc |= (1<<17); 	//set PWMdiv to 4

	//if the following two lines are placed higher in this initialization sequence, the PWM doesnt work. These disable PWM while making changes.
	*pwm2enable = 0x0;	//disable pwm - forces output at pin to 0 (masks pwm)
    *pwm2ctl = 0x0;		//turn off pwm module
	
    *pwm2genb |= (0x2<<10);		//go low when count equals compB
	*pwm2genb |= (0x3<<2);    	//go high when count equals load. Page 1286

	//Change these values to change PWM Duty Cycle. Load determines max count value, cmpB is counter value at which the ouput goes high
    *pwm2load = 0x999;	//arbitrary value for testing
    *pwm2cmpB = 0x700;	//arbitrary value for testing, comp B must be less than load
	
	/*doesnt work if this block is included, despite datasheet recommending it. Need additional controls?
	*SRPWM |= (1<<1);	//clear pwm1 counter so it can be started over by command
	delay(100);
	*SRPWM &= ~(1<<1);	//re-enable all pwm counters
	delay(100);		//wait for PWM to be ready - should be done by checking pwm periphrial ready register, PRPWM page 420
	*/

    *pwm2ctl = 1; 	//The PWM5/6 block is enabled and produces PWM signals (starts timers). p1270

    *pwm2enable |= (1<<5);  //pass pwmB to PF1, p 1232

}