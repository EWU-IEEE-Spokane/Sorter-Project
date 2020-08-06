void setup_I2C() {
	//initialize I2C module 1 for 100Kbps on PA7 (SDA), PA6 (SCL)
	
	uint32_t *RCGCI2C = (uint32_t *) (0x400FE000 + 0x620);		//Clock control for I2C modules p348
	uint32_t *RCGCGPIO = (uint32_t *) (0x400FE000 + 0x608);	//Clock control for input/output p340
	uint32_t *GPIOAFSEL_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x420);
	uint32_t *GPIOODR_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x50C);	//GPIO Open Drain Register
	uint32_t *GPIOPCTL_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x52C);	//Port A control p 688
	uint32_t *I2CMCR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x020);	//I2C1 master control
	uint32_t *I2CMTPR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x00C);	// I2C1 Timing
	uint32_t *I2CMIMR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x010);	// I2C1 Interrupt control
	uint32_t *GPIODEN_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x51c );
	uint32_t *gpiopur = (uint32_t *) (PORT_A_BASE_ADDRESS +0x510);	//pull up resistor register.

	//blink(0x08);
	//*GPIODEN_A = 0xC0;	can only enable port A bits 6 and 7, others have special considerations DONT ENABLE HERE MUST BE DONE AFTER CLOCK PROVIDED OR A BUS FAULT IS GENERATED
	//blink(0x08);

	*RCGCI2C |= 0x2; 				//1: enable clock to I2C1
	delay(100);
	//blink(0x08);
	*RCGCGPIO |= (0x1);				//2: enable clock to GPIO Port A
	delay(100);
	*GPIOAFSEL_A = 0xC0;			//3: pins 6 and 7 get alternative function
	*GPIOODR_A = 0x80;				//4: pin 7 (data) gets open drain setup
	*gpiopur = 0xC0;				//pins 6 and 7 get pull-ups
	*GPIOPCTL_A = 0x33000000;		//5: pins 6 and 7 get I2C1
	delay(100);
	*GPIODEN_A = 0xC0;			//Enable pins 6 and 7 of port A using the digital enable register. Must be done after clock provided and 3 clocks occured
	//blink(0x04);	//blue
	*I2CMIMR = 0x0;			//ensure interrupts are disabled
	//blink(0x08);	//green
	delay(100);
	*I2CMCR = *I2CMCR|0x10;					//6: I2C1 is a master
	//blink(0x08);
	*I2CMTPR = 0x18;				//7: SCL is 100kbps for 50MHz clock p 1003
	//blink(0x8);
	
}