#include <stdint.h>
#include "i2c.h"
#include "led.h"

void setup_I2C() {
	//initialize I2C module 1 for 100Kbps on PA7 (SDA), PA6 (SCL)
	
	uint32_t *RCGCI2C = (uint32_t *) (0x400FE000 + 0x620);				//Clock control for I2C modules p348
	uint32_t *RCGCGPIO = (uint32_t *) (0x400FE000 + 0x608);				//Clock control for input/output p340
	uint32_t *GPIOAFSEL_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x420);
	uint32_t *GPIOODR_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x50C);	//GPIO Open Drain Register
	uint32_t *GPIOPCTL_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x52C);	//Port A control p 688
	uint32_t *I2CMCR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x020);		//I2C1 master control
	uint32_t *I2CMTPR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x00C);		// I2C1 Timing
	uint32_t *I2CMIMR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x010);		// I2C1 Interrupt control
	uint32_t *GPIODEN_A = (uint32_t *) (PORT_A_BASE_ADDRESS + 0x51c );
	uint32_t *gpiopur = (uint32_t *) (PORT_A_BASE_ADDRESS +0x510);		//pull up resistor register.

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

void single_write(uint8_t dataIn){
	//write a single data byte on the I2C bus
	volatile uint32_t *I2CMSA = (uint32_t *) (I2C1_BASE_ADDRESS + 0x000);	// I2C1 Master Slave Address
	volatile uint32_t *I2CMDR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x008);	// I2C1 Master Data Register
	volatile uint32_t *I2CMCS = (uint32_t *) (I2C1_BASE_ADDRESS + 0x004);	// I2C1 Master Control/Status

	
	*I2CMSA = SENSOR_ADDRESS_WRITE;	//0 in bit 1 specifies write operation
	*I2CMDR = dataIn; 	//data to be written
	*I2CMCS = 0x7;	//I2CMSA specified write, so 0x7 starts a write operation with stop
	wait_on_bus();
	if(*I2CMCS & 0x8) blink(0x2);	//if data not acked, blink red
	if(*I2CMCS & 0x4) blink(0x2);	//if address not acked, blink red
}


uint8_t single_read(){
	//read a single data byte from the I2C bus
	volatile uint32_t *I2CMSA = (uint32_t *) (I2C1_BASE_ADDRESS + 0x000);	// I2C1 Master Slave Address
	volatile uint32_t *I2CMDR = (uint32_t *) (I2C1_BASE_ADDRESS + 0x008);	// I2C1 Master Data Register
	volatile uint32_t *I2CMCS = (uint32_t *) (I2C1_BASE_ADDRESS + 0x004);	// I2C1 Master Control/Status
	
	*I2CMSA = SENSOR_ADDRESS_READ;	//1 in bit 1 specifies read operation
	*I2CMCS = 0x7;	//I2CMSA specified read, so 0x7 starts a read operation with stop
	wait_on_bus();
	if(*I2CMCS & 0x8) blink(0x2);	//if data not acked, blink red
	if(*I2CMCS & 0x4) blink(0x2);	//if address not acked, blink red
	return *I2CMDR;
}

void wait_on_bus(){
	uint32_t *I2CMCS = (uint32_t *) (I2C1_BASE_ADDRESS + 0x004);	// I2C1

	delay(5000);	//fixed wait time, not optimal, but currently necessary.
					//is the below bus wait loop not operational?
	while(*I2CMCS & (1<<6)){
		//wait until bus is not busy as indicated by bit 6, in the meantime, blink
		blink(0xC); //blink teal LED to show waiting on bus. This is probably too long of a delay since the LED is on for so long
	}
	if(*I2CMCS & (1<<1)){
		//if I2C module reports error as indicated by bit 1, show with blue LED
		blink(0x04); //blink blue LED to show error

		//add error handling code here -----------------------------------------------------------------------------
	}
}