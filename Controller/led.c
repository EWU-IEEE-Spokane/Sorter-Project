#include <stdint.h>
#include "led.h"

uint32_t writeOffset = (0x1F<<2);   //write offset for pins 4 through 0. 4:SW1, 0:SW2, 3:Green, 2:Blue, 1:Red (in Port F)

void setup_LEDs() {
	uint32_t *gpiodir = (uint32_t *) (PORT_F_BASE_ADDRESS + 0x400 );
		//The direction registry address for IO config. Page 663 in the datasheet
	uint32_t *gpioafsel = (uint32_t *)(PORT_F_BASE_ADDRESS + 0x420);
		//Alternate Function Select registry address for selecting GPIO or other pin functions. Page 671 in the datasheet
    uint32_t *gpioden = (uint32_t *) (PORT_F_BASE_ADDRESS + 0x51c );
		//Digital enable registry address for enabling or disabling digital enable. Page 682 in the datasheet
	uint32_t * rcgcgpio = (uint32_t *)(0x400FE000+0x608); //Run-Mode Clock Gating Control registry, page 340 of datasheet
	
	*rcgcgpio = *rcgcgpio|(1<<5);	//enable and provide a clock to port F. This allows port F to operate by enabling its bus and register access
	*gpioafsel =  0x00;	//Choose GPIO using the alternative function select registry
	*gpiodir = 0x0E;	//Make LED pins outputs using the direction registry
	//*gpiodir = (*gpiodir & ~(0x11));	make switches inputs
	*gpioden = 0xFF;	//Enable all pins using the digital enable registry
}



void blink(uint32_t LED) {
	//passed in value is the bit number of the led pin in the gpio register. red: 0x02, blue: 0x04, green:0x08
	uint32_t * gpiodata = (uint32_t *) (PORT_F_BASE_ADDRESS+writeOffset);
		//data register address for definition of led states with mask for bits 4-0.
		*gpiodata = LED;	//selects color of LED. red: 0x02, blue: 0x04, green:0x08. Combinations can be used to get other colors, i.e. 0xE is white
		delay(1000000);
		*gpiodata = 0x00;	//shut off LEDs
		delay(1000000);
}



void delay(uint32_t d) {
	//This is a busy loop, will wait for a loop of length d
	for(int i=0; i<d; i++){
		//kill time by changing increment variable and checking loop condition, but dont do anything
	}
}