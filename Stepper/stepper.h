#include <stdint.h>

#ifndef STEPPER_H
#define STEPPER_H

#define MAX_STEP 400
#define DIRECTION_CW 0x01
#define DIRECTION_CCW 0x00

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
void absPosMode_Slice(uint8_t stepNum);
void relPosMode(uint8_t direction, uint8_t numSteps);
void run(uint8_t mode);
uint8_t limitDebounce();
uint16_t serialIn();

#endif