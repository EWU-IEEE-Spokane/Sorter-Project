#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "i2c.h"
#include "uart.h"
#include "color_sensor.h"
#include "color_process.h"

void config_sensor() {
	//configure the color sensor registers by sending it addressed values over I2C
	//blink(0X8);
	//write_sensor(0x00, 0xE6);	//clear pending interrupts
	//write_sensor(0x00, 0x00);
	write_sensor(0x00, 0x01); //turn on sensor clock
	delay(1000000);	//allow setup time for sensor clock - at least 2.4ms (this delay is probably overkill)
	//blink(0x08); //blink green to show oscillators up
	write_sensor(0x0F, 0x1); //set gain to 1xs
	write_sensor(0x01, 0xC0);	//set integration time to 154ms
	write_sensor(0x00, 0x3); //turn on sensor ADC's with 0x2 (plus 0x1 to keep oscillators enabled)
	//blink(0x08); //blink green to show ADC's up

	/* single write version
	single_write(0x80);
	single_write(0x01);
	delay(1000000);
	single_write(0x03);
	*/
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

void read_colors() {
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
	determine_color(redData, greenData, blueData);
}