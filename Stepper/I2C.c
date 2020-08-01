/*	
 * EWU IEEE Industrial Sorter Project
 * I2C communications for TCS34725 Color Sensor
 * Cody Birkland
 * For TM4C123G
 *
 * 
 * Description:
 * Initialization and config for I2C module 1 on port a pins 6 and 7 (6 is SCL, 7 is SDA)
 * Based on TM4C datasheet and Sensor (TCS34725) Datasheet.
 * Paper notes have been written up for specific initialization procedure, as well as for the color sorting algorithm used.
 * These documents will be converted into a legible format and published soon.
 * This is a work in progress.
 *  
 * The wait_on_bus() bus delay is still broken, but for now all transmissions are single writes or reads, and the bus delay is 
 * simply a busy wait loop that kills time while the data is transmitting.
 * 
 * Digital saturation problems were resolved by keeping gain at 1x.
 * Analog saturation (sensor overwhelmed) problems were resolved by reducing integration time from 700ms to 154ms (config_sensor)
 * as well as reducing intensity of sensor LED light by adding PWM to PF1 and plugging the sensor LED pin in there.
 * 
 * How to use this code:
 * You'll need the Adafruit TCS34725 color sensor breakout board to use this code.
 * TM4C Port A pin 7 is the Data line	(PA7 SDA)
 * TM4c Port A Pin 6 is the Clock line	(PA6 SCL)
 * Ground the Sensor's GND pin and Connect the TM4C VBUS pin to Vin on the sensor (USB 5V tolerant)
 * Grounding the LED pin on the sensor turns off the LED, this could be wired to the TM4C for control,
 * and eventually PWM (pulse-width modulation) could be used to vary the light brightness.
 * 
 *  * ---TCS34725 Sensor Pin Mapping (to TM4C)
 * ---VIN - VBUS
 * ---GND - GND
 * ---3V3 - NC, No Connection
 * ---SCL - PA6
 * ---SDA - PA7
 * ---INT - NC, No Connection
 * ---LED - PF1 (same as red onboard LED)
 */


#include "I2C.h"
//*****************************************************************************
//
// Start of Code:
//
//*****************************************************************************

int main() {
	//Call core functions in required sequence
	int foo = 0;	//arbitrary integer value, in the system this integer will be the value sent to the controller
	/*the controller will call read_colors to get the sort result:
	* 0 = reject (colors other than skittle colors or bad data)
	* 1 = red
	* 2 = orange
	* 3 = yellow
	* 4 = green
	* 5 = violet/purple
	*/
	//uint8_t sensorVersion = 0;
	setup();
	
	while(1){
		//runtime loop
		//blink(0xE);	//white
		
		//sensorVersion = read_sensor(0x12);		//read sensor version register at sensor address 0x12
		//say_byte(sensorVersion);
		//new_line();
		//if(sensorVersion == 0x44) blink(0x8);	//blink green to show that a valid sensor version was obtained from the device
		
		//blink(0xE);	//white

		// wait for ADC was right here
		//blink(0x4);	//blue
		delay(10000000);
		foo = read_colors();
		
		
	}
	
}

void setup(){
	setup_clock();
	setup_LEDs();
	configure_pwm();
	setup_I2C();
	
	
	config_sensor();
	delay(10000);
	setup_uart();
}



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


//I2C CODE STARTS ----------------------------------------------------------------------------



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



void config_sensor() {
	//configure the color sensor registers by sending it addressed values over I2C
	//blink(0X8);
	//write_sensor(0x00, 0xE6);	//clear pending interrupts
	//write_sensor(0x00, 0x00);
	write_sensor(0x00, 0x01); //turn on sensor clock
	delay(1000000);	//allow setup time for sensor clock - at least 2.4ms (this delay is probably overkill)
	//blink(0x08); //blink green to show oscillators up
	write_sensor(0x0F, 0x03); //set gain to 4x
	write_sensor(0x01, 0x01);	//set integration time to 700ms (register 0x01)
	write_sensor(0x00, 0x3); //turn on sensor ADC's with 0x2 (plus 0x1 to keep oscillators enabled)
	//blink(0x08); //blink green to show ADC's up

	/* single write version
	single_write(0x80);
	single_write(0x01);
	delay(1000000);
	single_write(0x03);
	*/
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


void write_sensor(uint8_t addressIn, uint8_t dataIn) {
	//write a single data byte to the addressed register in the sensor
	single_write(0x80|addressIn); 	//in first command byte, tell the sensor what register to move to (bit 7 enables commands)
	single_write(dataIn);	//write data to the above register
}



uint8_t read_sensor(uint8_t addressIn) {
	//read a single data byte from the addressed register
	uint8_t data;

	single_write(0x80|addressIn);	//1000 0000 + 0x12
	data = single_read();
	return data;
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

void wait_on_adc() {
	//wait for the on chip ADC to be ready, check by polling status reg 0x13 bit 0 (0x1 = ADC Valid)
	uint8_t sensorStatus = 0x0;
	bool colorValid = false;

		while(!colorValid){
			//wait here until a valid result is obtained from the sensor
			sensorStatus = read_sensor(0x13); //read status register (8 bits)
			colorValid = (sensorStatus & 0x1);	//color is valid if bit 1 is true
			//blink(0x2); too much delay
		}
}

int read_colors() {
	//read each color register and return the collection of data
	uint16_t redData = 0x0;
	uint16_t greenData = 0x0;
	uint16_t blueData = 0x0;

	uint16_t whiteValue = 0x0;
	//uint16_t irValue = 0x0;
	uint16_t sum = 0x0;

	//float redValue = 0x0;
	//float greenValue = 0x0;
	//float blueValue = 0x0;
	wait_on_adc();

	//read color value registers and combine bytes:
	whiteValue = (read_sensor(0x15)<<8) + read_sensor(0x14);
	//wait_on_adc();

	redData = (read_sensor(0x17)<<8) + read_sensor(0x16);		//high byte plus low byte
	//wait_on_adc();
	greenData = (read_sensor(0x19)<<8) + read_sensor(0x18);
	//wait_on_adc();
	blueData = (read_sensor(0x1B)<<8) + read_sensor(0x1A);

	sum = redData + greenData + blueData;

	/*
	if(sum>whiteValue){
		irValue = (sum-whiteValue)/2;
	}else{
		irValue = 0;
	}
	*/

	//combine values into one data packet (like hex color value)? for now just read them out with UART
	uartwrite('R');
	uartwrite('-');
	say_two_bytes(redData);
	uartwrite('G');
	uartwrite('-');
	say_two_bytes(greenData);
	uartwrite('B');
	uartwrite('-');
	say_two_bytes(blueData);
	uartwrite('W');
	uartwrite('-');
	say_two_bytes(whiteValue);
	uartwrite('S');
	uartwrite('-');
	say_two_bytes(sum);
	return determine_color(redData, greenData, blueData);
}

//COLOR PROCESSING CODE -----------------------------------------------------------

int determine_color(uint16_t r, uint16_t g, uint16_t b) {
	uint32_t sum = 0;
	uint32_t redPart = 0;
	uint32_t greenPart = 0;
	uint32_t bluePart = 0;
	int resultInt = 0;	//reject by default
	
	sum = r+g+b;

	//the following assignments give the protion of the total light recieved seen by each of the 3 channels (as a percentage)
	//the values are scaled to compensate for asymmetric color sensitivities as seen in the color sensor datasheet, page 6 figure 2
	//from figure 2, these values were obtained by calculating the inverse of the peak magnitude of each channel's color response
	redPart = (r*100)/sum;		//120 is color scaling modifier (originally 119)
	greenPart = (g*150)/sum;	//135 is color scaling modifier (originally 155)
	bluePart = (b*165)/sum;		//190 is color scaling modifier (originally 182)
	/**/
	/*
	uartwrite('h');
	uartwrite('e');
	uartwrite('x');
	uartwrite(':');
	uartwrite(' ');
	say_byte(redPart);		//print hex values
	say_byte(greenPart);
	say_byte(bluePart);
	new_line();
	*/
	say_rgb(redPart, greenPart, bluePart);

	char decision[16];		//result string 

	uint8_t colorVals[] = {redPart, greenPart, bluePart};
	colorSort(colorVals);	//sort in ascending order
	char colorChars[3]; 
	mapColorChars(colorChars, colorVals, redPart, greenPart, bluePart);
	float ratio = (float) 0.0;	//casting as float since compiler recognizes fractional constants as doubles - overflows 32 bit MCU
	int numerator = (colorVals[2]-colorVals[0]);
	int denominator = (colorVals[1]-colorVals[0]);
	if (denominator == 0) denominator = 1;	//avoid divide by 0
	ratio = numerator/denominator;

	if(ratio>(float)6.0){		//2.2 threshold
		resultInt = decideInf(decision, colorChars[2]);	//decide based on largest color value
	}else if(ratio>(float)1.3){		//threshold 1.3
		resultInt = decideMid(decision, colorChars[2],colorChars[0]);	//decide based on largest and smallest color value
	}else if(ratio>=(float)0){
		resultInt = decideOne(decision, colorChars[0]);	//decide based on smallest color value
	}else{
		strcpy(decision, "ratioCalcError");
		resultInt = 0;
	}
	say_string(decision, 16);
	return resultInt;
}


void mapColorChars(char* chars, uint8_t* colorVals, uint8_t r, uint8_t g, uint8_t b){
	for(int i=0; i<3; i++){
		chars[i] =  (colorVals[i]==r)? 'r':
					(colorVals[i]==g)? 'g':
					(colorVals[i]==b)? 'b':
					/*error inidcator*/'x';
	}
}

int decideInf(char* decision, char max){
	switch (max)
	{
	case 'r':
		strcpy(decision, "red");
		return 1;
		break;
	case 'g':
		strcpy(decision, "green");
		return 4;
		break;
	case 'b':
		strcpy(decision, "blue");
		return 0;
		break;
	default:
		strcpy(decision, "charMappingError");
		return 0;
		break;
	}
}

int decideMid(char* decision, char max, char min){
	if(max=='r'){
		if(min=='b'){
			strcpy(decision, "orange");
			return 2;
		}else{
			strcpy(decision, "magenta");
			return 0;
		}
	}else if(max=='g'){
		if(min=='b'){
			strcpy(decision, "yellow-green");
			return 0;
		}else{
			strcpy(decision, "blue-green");
			return 0;
		}
	}else if(max=='b'){
		if(min=='g'){
			strcpy(decision, "indigo");
			return 0;
		}else{
			strcpy(decision, "light blue");
			return 0;
		}
	}else{
		strcpy(decision, "charMappingError");
		return 0;
	}
}

int decideOne(char* decision, char min){
	switch (min)
	{
	case 'r':
		strcpy(decision, "teal");
		return 5;	//say violet for now
		break;
	case 'g':
		strcpy(decision, "violet");
		return 5;
		break;
	case 'b':
		strcpy(decision, "yellow");
		return 3;
		break;
	default:
		strcpy(decision, "charMappingError");
		return 0;
		break;
	}
}

void colorSort(uint8_t *array){   
    //just a bubble sort for now :/

    bool swapNeeded = true;
	int length = 3;
    while(swapNeeded){
        //run until everything is sorted, as indicated by no more swaps needed
        swapNeeded = false; //flag for element swap condition
        for(int i = 0; i<(length-1); i++){
            //walk through array indexes, if out of order, swap
            if(array[i]>array[i+1]){
                int temp = array[i+1];
                array[i+1] = array[i];
                array[i] = temp;
                swapNeeded = true;  //swap was needed, repeat while loop
            }
        }
    }


}


void say_rgb(uint8_t r, uint8_t g, uint8_t b){
	say_string("RGB: ", 5);
	uartwrite(hexToChar((r/16)%16));
	uartwrite(hexToChar((r)%16));
	uartwrite(hexToChar((g/16)%16));
	uartwrite(hexToChar((g)%16));
	uartwrite(hexToChar((b/16)%16));
	uartwrite(hexToChar((b)%16));
	new_line();
}



float process_values(uint16_t colorData, uint16_t whiteValue, uint16_t irValue){
	//return ((((colorData*0xFF))/(whiteValue*9)));	//worked moderately well, brown instead of red - this was a crude 1st try

	//ir compensation attempt, produced inconsistent results
	//if(colorData > irValue){
	//	colorData -= irValue;
	//}else{
	//	colorData = 0;
	//}
	return (((colorData*0xFF))/(whiteValue*6));	//attempted to have denom 0xFFFF, but this required gain correction of 7, which was the ratio of whiteValue to 0xFFFF
}

uint16_t applyGammaTable(float colorData){
	colorData /= 0xFF;
	colorData = colorData*colorData*(colorData/2);	//raise to 2.5th power
	colorData *= 0xFF;
	//return (uint16_t)(7*colorData);	//was working well for red values with gamma exponent of 2.2
	//return (uint16_t)(colorData/0x80);	//eliminating ir comp must've made base of gamma exponent greater than 1, since values exploded. attempting to scale
	return (uint16_t)colorData;	//no post gamma gain factors
}

//PWM CODE -----------------------------------------------------------

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
    unsigned long *gpioden 		= (unsigned long *)(PORT_F_BASE_ADDRESS + 0x51c );	//Digital enable registry address for enabling or disabling digital enable. Page 682 in the datasheet
    unsigned long *gpioAMSEL 	= (unsigned long *)(PORT_F_BASE_ADDRESS + 0x528 ); //analog function enable register

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
	
    *gpioAFSEL |= (1<<1);	//make Port F pin 1 an alternate function
    *gpioPCTL |= (0x5<<4); 	//set PF1 to get alt func M1PWM5 pages 689, 1352
     
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
    *pwm2cmpB = 0x996;	//arbitrary value for testing, comp B must be less than load
	
	/*doesnt work if this block is included, despite datasheet recommending it. Need additional controls?
	*SRPWM |= (1<<1);	//clear pwm1 counter so it can be started over by command
	delay(100);
	*SRPWM &= ~(1<<1);	//re-enable all pwm counters
	delay(100);		//wait for PWM to be ready - should be done by checking pwm periphrial ready register, PRPWM page 420
	*/

    *pwm2ctl = 1; 	//The PWM5/6 block is enabled and produces PWM signals (starts timers). p1270

    *pwm2enable |= (1<<5);  //pass pwmB to PF1, p 1232

}


//UART CODE -----------------------------------------------------------

void setup_uart() {
	// Add the configuration steps necessary to enable U0TX at
	//   115200bps
	//   8 data bits
	//   No parity
	//   1 Stop bit
	// See datasheet Section 14.4, pg 902-903 for Init Procedure
	// See datasheet Section 14.3.2, pg 896 for the Baudrate (115200) calculation
	// Also in Valvano 8.3.2 UART Device Driver, pg 319 in 4th ed
	
	unsigned long * RCGCUART = (unsigned long *)(0x400FE000 + 0x618); 	//Run mode clock gating control for UART, page 344
	unsigned long * RCGCGPIO = (unsigned long *)(0x400FE000 + 0x608); 	//Run-mode clock gating control for GPIO, page 340
	unsigned long * GPIOAFSEL =(unsigned long *) (0x40004000 + 0x420); 	//Alternate function select for Port A, page 672
	unsigned long * GPIOPCTL = (unsigned long *)(0x40004000 + 0x52C);	//GPIO port A control, page 689
	unsigned long * UARTCTL  = (unsigned long *)(0x4000C000 + 0x030);	//UART0 control page 918
	unsigned long * UARTIBRD = (unsigned long *)(0x4000C000 + 0x024);	//Integer part of baud rate divisor page 914
	unsigned long * UARTFBRD  =(unsigned long *) (0x4000C000 + 0x028);	//Fractional part of baud rate divisor page 915
	unsigned long * UARTLCRH  =(unsigned long *) (0x4000C000 + 0x02C);	//UART line control for word length and fifo enable, page 916
	unsigned long * GPIODEN = (unsigned long *)(0x40004000 + 0x51C);	//Digital enable register for gpio port A

	*RCGCUART |= (1<<0); 	//Enable UART 0, which is connected to USB Tx/Rx 
	*RCGCGPIO |=  (1<<0); 	//Enable Port A. Port a is used for UART0 as per page 1351
	*GPIOAFSEL |= (0x3<<0);	//configure port A pins 0 and 1 as using periphrial
	*GPIOPCTL |= (0x11);		//Make port A pins 0 and 1 UART0, see page 650 for pin assignments
	*UARTCTL |= (1<<0); 		//Enable UART 0
	*GPIODEN |= 0x03;					//Enable pins 0 and 1 of port A
	//blink(0x04);						//blink blue LED to indicate progress through register assignment

	//to obtain 115200 baud,  clock rate divisor is 27.126736 since system clock is 50MHz and HSE in  UARTCTL is clear.
	//thus the fractional part times 64 plus 0.5 is 8.61111. Assign 27 to UARTIBRD and floor(8.61111) to UARTFBRD. See p 869
	*UARTIBRD = 27;		//Integer part, hex equivalent of decimal 27
	*UARTFBRD = 8;		//fractional part, decimal 8
	*UARTLCRH |= ((0x3<<5)+(1<<4));	//Word length enable to 8 by asserting bits 5 and 6, enable fifo by asserting bit 4
		//leaving bit 1 and 3 as zero results in 1 stop bit and no parity bit. This UARTLCRH write must occur following any BRD change
	//blink(0x04);					//blink blue LED again to indicate register assignment is complete
}



void uartwrite(uint8_t data) {
	// Use UARTFR, datasheet pg 911, TXFF bit to make sure the TX FIFO memory is not full
	// then UARTDR, datasheet pg 906 to write this new character into the TX FIFO
	unsigned long * UARTFR = (unsigned long *)(0x4000C000 + 0x018);		//UART Flag for fifo status, bit 5 = 1, fifo full
	unsigned long * UARTDR = (unsigned long *)(0x4000C000 + 0x00);		//UART transmits bits 0:7
	//blink(0x02);		//blink red LED
	while(*UARTFR&(1<<5)){
		//wait until fifo is not full
	}
	*UARTDR = data;		//assign data register to current value of data, which is the current char
	//blink(0x08);		//blink green LED - slows typing speed dramatically
}



void say_hello() {
	// Modify hello string constant to include your name
	char hello[] = "Hello EWU IEEE, from Cody Birkland\n\r";
	//blink(0x08);	//blink green LED
		// Send hello string one character at a time, repeating forever
		for(int i = 0; i<(sizeof(hello)/sizeof(hello[0]));i++){
			uartwrite(hello[i]);
			delay(10000);	//LED blinks in uartwrite do the majority of the write speed limiting
		}
}



void say_two_bytes(uint16_t data) {
	char dataChar[4];
	dataChar[0] = hexToChar((data/4096)%16);	//most significant nibble
	dataChar[1] = hexToChar((data/256)%16);		//2nd most significant nibble
	dataChar[2] = hexToChar((data/16)%16);		//3rd most significant nibble
	dataChar[3] = hexToChar((data)%16);			//least signifigant nibble
	/*
	uartwrite(dataChar[0]);
	uartwrite(dataChar[1]);
	uartwrite(dataChar[2]);
	uartwrite(dataChar[3]);
	new_line();
	*/
	say_string(dataChar, 4);
}



void say_byte(uint8_t data) {
	char dataChar[2];
	dataChar[0] = hexToChar((data/16)%16);	//most significant nibble
	dataChar[1] = hexToChar((data)%16);		//least signifigant nibble
	/*
	uartwrite(dataChar[0]);
	uartwrite(dataChar[1]);
	*/
	say_string(dataChar, 2);
}

void new_line(){
	uartwrite('\n');	//line feed
	uartwrite('\r');	//carriage return
	delay(10000);	//LED blinks in uartwrite do the majority of the write speed limiting
}


void say_nibble(uint8_t dataNibble) {
	// Read out a nibble (half byte) on UART0 as hexadecimal
	char dataChar;

	dataChar = hexToChar(dataNibble);
	uartwrite(dataChar);
	uartwrite('\n');	//line feed
	uartwrite('\r');	//carriage return
	delay(10000);	//LED blinks in uartwrite do the majority of the write speed limiting
		
}



char hexToChar(uint8_t inputNum) {
	//converts a passed in data byte into its corresponding ASCII character for display on UART
	unsigned char charDisplayed = 'X'; //Initializes char as invalid option X, should be changed by below code
	
	charDisplayed = inputNum == 0x0 ?	'0':
					inputNum == 0x1 ?	'1':
					inputNum == 0x2 ?	'2':
					inputNum == 0x3 ?	'3':
					inputNum == 0x4 ?	'4':
					inputNum == 0x5 ?	'5':
					inputNum == 0x6 ?	'6':
					inputNum == 0x7 ?	'7':
					inputNum == 0x8 ?	'8':
					inputNum == 0x9 ?	'9':
					inputNum == 0xA ?	'A':
					inputNum == 0xB ?	'B':
					inputNum == 0xC ?	'C':
					inputNum == 0xD ?	'D':
					inputNum == 0xE ?	'E':
					inputNum == 0xF ?	'F':
					/*input invalid*/ 	'-';

	return charDisplayed;
}



void say_string(char * string, int length) {
		// Send input string one character at a time
		for(int i = 0; i<length;i++){
			if(string[i]=='\0') break; //null terminator signifies end of string
			uartwrite(string[i]);
			delay(10000);	//LED blinks in uartwrite do the majority of the write speed limiting
		}
		new_line();
}


//*****************************************************************************
//
// The End.
//
//*****************************************************************************