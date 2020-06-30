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
uint32_t *SYSCTL_RCGCGPIO_R = (uint32_t*) (0x400FE000 + 0x608); // Run-mode clock gating control
uint32_t *GPIO_PORTA_DIR_R = (uint32_t*) (0x40004000 + 0x400); // Port A direction
uint32_t *GPIO_PORTA_DEN_R = (uint32_t*) (0x40004000 + 0x51C); // Port A Digital Enable
uint32_t *GPIO_PORTA_DATA_R = (uint32_t*) (0x40004000 + 0xF0); // Port A Data for Pins PA2-PA5

uint32_t *GPIO_PORTF_DIR_R = (uint32_t*) (0x40025000 + 0x400); // Port F direction
uint32_t *GPIO_PORTF_DEN_R = (uint32_t*) (0x40025000 + 0x51C); // Port F Digital Enable
uint32_t *GPIO_PORTF_DATA_R = (uint32_t*) (0x40025000 + 0x40); // Port F Data for Pin PF0 (sw2)
uint32_t *GPIO_PORTF_PUR_R = (uint32_t*) (0x40025000 + 0x510);



#endif