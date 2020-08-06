#ifndef I2C_H
#define I2C_H
//*****************************************************************************
//
// Include standard boolean (true, false, bool type) and standard integer 
// (uint32_t == unsigned integer 32-bit type, etc) libraries:
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define PORT_A_BASE_ADDRESS 0x40004000
#define PORT_B_BASE_ADDRESS 0x40005000
#define PORT_C_BASE_ADDRESS 0x40006000
#define PORT_D_BASE_ADDRESS 0x40007000
#define PORT_E_BASE_ADDRESS 0x40024000
#define PORT_F_BASE_ADDRESS 0x40025000
#define I2C1_BASE_ADDRESS	0x40021000
#define SENSOR_ADDRESS_WRITE 0x52
#define SENSOR_ADDRESS_READ	0x53

//*****************************************************************************
//
// Forward declaration of functions below main (prototypes):
//
//*****************************************************************************

void setup();

void blink(uint32_t);	//blink function for debugging with on-board LEDs
void delay(uint32_t);	//delay function - keeps processor busy
void setup_LEDs();
void setup_clock();			//initialize system clock at 50MHz
void wait_for_pll();		//wait for system clock phase-locked-loop to be ready

void setup_I2C();
void write_sensor(uint8_t, uint8_t);
uint8_t read_sensor(uint8_t);
void wait_on_bus();
void wait_on_adc();
void config_sensor();

uint8_t single_read();
void single_write(uint8_t);
int read_colors();
float process_values(uint16_t, uint16_t, uint16_t);
uint16_t applyGammaTable(float);

int decideInf(char*, char);
int decideMid(char*, char, char);
int decideOne(char*, char);

int determine_color(uint16_t, uint16_t, uint16_t );

void colorSort(uint8_t *);

void say_rgb(uint8_t, uint8_t, uint8_t);
void mapColorChars(char*, uint8_t*, uint8_t, uint8_t, uint8_t);

void configure_pwm();

void setup_uart();
void say_hello();
void say_nibble(uint8_t);
void say_byte(uint8_t);
void say_two_bytes(uint16_t);
char hexToChar(uint8_t);
void uartwrite(uint8_t);
void say_string(char *, int);
void new_line();

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************

// uint32_t writeOffset;
#endif