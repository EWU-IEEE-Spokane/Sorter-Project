#include <stdint.h>

#ifndef STEPPER_H
#define STEPPER_H

#define MAX_STEP 200
#define DIRECTION_CW 0x00
#define DIRECTION_CCW 0x01

// Holds the data in a motor state
struct State {
    uint8_t cState;
    uint8_t out;
    uint8_t next[2];
};

// New type for the FSM
typedef const struct State State_t;

// I/O port addresses
uint32_t *SYSCTL_RCGCGPIO_R = (uint32_t*) (0x400FE000 + 0x608);// Run-mode clock gating control
uint32_t *GPIO_PORTA_DIR_R = (uint32_t*) (0x40004000 + 0x400); // Port A direction
uint32_t *GPIO_PORTA_DEN_R = (uint32_t*) (0x40004000 + 0x51C); // Port A Digital Enable
uint32_t *GPIO_PORTA_DATA_R = (uint32_t*) (0x40004000 + 0x1F0);// Port A Data for Pins 6-2

uint32_t *GPIO_PORTB_DIR_R = (uint32_t*) (0x40005000 + 0x400); // Port B direction
uint32_t *GPIO_PORTB_DEN_R = (uint32_t*) (0x40005000 + 0x51C); // Port B Digital Enable
uint32_t *GPIO_PORTB_DATA_R = (uint32_t*) (0x40005000 + 0x7C); // Port B Data Pins 4-0
uint32_t *GPIO_PORTB_PDR_R = (uint32_t*) (0x40005000 + 0x514); // Port B Pull-down Resistors

uint32_t *GPIO_PORTC_DIR_R = (uint32_t*) (0x40006000 + 0x400); // Port C direction
uint32_t *GPIO_PORTC_DEN_R = (uint32_t*) (0x40006000 + 0x51C); // Port C Digital Enable
uint32_t *GPIO_PORTC_DATA_R = (uint32_t*) (0x40006000 + 0x3C0);// Port C Data Pins 7-4
uint32_t *GPIO_PORTC_PDR_R = (uint32_t*) (0x40006000 + 0x514); // Port C Pull-down Resistors

uint32_t *GPIO_PORTD_DIR_R = (uint32_t*) (0x40007000 + 0x400); // Port D direction
uint32_t *GPIO_PORTD_DEN_R = (uint32_t*) (0x40007000 + 0x51C); // Port D Digital Enable
uint32_t *GPIO_PORTD_DATA_R = (uint32_t*) (0x40007000 + 0x3C); // Port D Data Pins 3-0
uint32_t *GPIO_PORTD_PDR_R = (uint32_t*) (0x40007000 + 0x514); // Port D Pull-down Resistors

uint32_t *GPIO_PORTF_DIR_R = (uint32_t*) (0x40025000 + 0x400); // Port F direction
uint32_t *GPIO_PORTF_DEN_R = (uint32_t*) (0x40025000 + 0x51C); // Port F Digital Enable
uint32_t *GPIO_PORTF_DATA_R = (uint32_t*) (0x40025000 + 0x40); // Port F Data for Pin PF0 (sw2)
uint32_t *GPIO_PORTF_PUR_R = (uint32_t*) (0x40025000 + 0x510); // Port F Pull-up Resistors

#endif