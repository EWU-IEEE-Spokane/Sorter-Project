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
void stepOnce(uint8_t direction, uint32_t* output, uint8_t* cState, uint16_t* cStep);
void homingMode(uint32_t* homeSW_Reg, uint8_t homeSW_Pin, uint32_t* output, uint8_t* cState, uint16_t* cStep);
void absPosMode_360(uint8_t number, uint32_t* output, uint8_t* cState, uint16_t* cStep);
void absPosMode_Slice(uint8_t stepNum, uint32_t* output, uint8_t* cState, uint16_t* cStep);
void relPosMode(uint8_t direction, uint8_t numSteps, uint32_t* output, uint8_t* cState, uint16_t* cStep);
void run(uint8_t mode, uint8_t direction, uint8_t number, uint32_t* home, uint32_t* output, uint8_t* cState, uint16_t* cStep);
uint8_t limitDebounce(uint32_t* inputSW_Reg, uint8_t inputSW_Pin);

#endif