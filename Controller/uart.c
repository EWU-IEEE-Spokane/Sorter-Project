#include <stdint.h>
#include "uart.h"
#include "led.h"

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