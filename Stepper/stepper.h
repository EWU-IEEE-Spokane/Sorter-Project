#include <stdint.h>

#ifndef STEPPER_H
#define STEPPER_H

#define MAX_STEP 200
#define DIRECTION_CW 0x00
#define DIRECTION_CCW 0x01

#define HOME (*GPIO_PORTB_DATA_R & 0x01)
#define DIRECTION (*GPIO_PORTB_DATA_R & 0x02)
#define MODE ((*GPIO_PORTB_DATA_R & 0x0C) >> 2)
#define START ((*GPIO_PORTB_DATA_R & 0x10) >> 4)
#define NUMBER (((*GPIO_PORTD_DATA_R & 0x0F) << 4) | (*GPIO_PORTC_DATA_R & 0xF0))

// Holds the data in a motor state
struct State {
    uint8_t cState;
    uint8_t out;
    uint8_t next[2];
};

// New type for the FSM
typedef const struct State State_t;

void delayT(int maxCount);
void stepOnce(uint8_t direction);
void debounce(uint8_t* input, uint8_t* flag);
void homingMode();
void absPosMode_360();
void absPosMode_Slice();
void relPosMode(uint8_t direction, uint8_t numSteps);
void run(uint8_t mode);
uint8_t limitDebounce();
uint16_t serialIn();

#endif