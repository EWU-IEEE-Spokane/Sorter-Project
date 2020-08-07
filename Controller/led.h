#ifndef LED_H
#define LED_H

#include <stdint.h>

#define PORT_F_BASE_ADDRESS 0x40025000

uint32_t writeOffset;

void setup_LEDs();
void blink(uint32_t LED);
void delay(uint32_t d);

#endif