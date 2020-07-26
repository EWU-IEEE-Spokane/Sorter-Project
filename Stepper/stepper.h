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

void stepOnce(uint8_t direction, uint8_t *cState);
void debounce(uint8_t* input, uint8_t* flag);
void homingMode(uint32_t* data, volatile uint8_t* mode, uint8_t* cState);
void absPosMode_360(uint8_t data, uint8_t* cState);
void absPosMode_Slice(uint8_t data, uint8_t* cState);
void relPosMode(uint8_t direction, uint8_t* cState, uint8_t numSteps);
void run(uint8_t mode, uint8_t* cState);
uint8_t limitDebounce(uint32_t* limitSw);
uint16_t serialIn();

#endif