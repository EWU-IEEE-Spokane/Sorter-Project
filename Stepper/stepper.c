#include <stdint.h>
#include <stdlib.h>
#include "stepper.h"

uint16_t cStep = 0; // Current step number; i.e. address for absolute positioning; 0 should be "home" position
uint8_t cState = 0; // Current motor state

// States in the 28byJ FSM
State_t byjFSM[4] = {
    {0, 0x04, {1, 3} },
    {1, 0x08, {2, 0} },
    {2, 0x10, {3, 1} },
    {3, 0x20, {0, 2} }
};

// States in the Nema-17 FSM
State_t nemaFSM[4] = {
    {0, 0x18, {1, 3} },
    {1, 0x28, {2, 0} },
    {2, 0x24, {3, 1} },
    {3, 0x14, {0, 2} }
};

// Half-stepping Nema-17 FSM
State_t nema2FSM[8] = {
    {0, 0x18, {1, 7} },
    {4, 0x08, {2, 0} },
    {1, 0x28, {3, 1} },
    {5, 0x20, {4, 2} },
    {2, 0x24, {5, 3} },
    {6, 0x04, {6, 4} },
    {3, 0x14, {7, 5} },
    {7, 0x10, {0, 6} }
};

// Quick and dirty busy-loop delay
// This will need updating to a more precise delay
void delayT(int maxCount) {
    for (volatile int j = 0; j < maxCount; j++) {continue;}
}

// Step the motor once
// Direction map: 0x00 = CW; 0x01 = CCW; 
void stepOnce(uint8_t direction, uint32_t* output) {
    cState = nema2FSM[cState].next[direction]; // Move to next state defined by direction
    *output = nema2FSM[cState].out;  // Output motor data defined in new state
    
    // Update cStep--this algorithm can be improved
    if (direction == DIRECTION_CCW) { 
        if (cStep-1 < 0) {         // wrap around
            cStep = MAX_STEP;
        } else {
            cStep--; 
        }
    } else { 
        if (cStep+1 == MAX_STEP) {  // wrap around
            cStep = 0;          
        } else { 
            cStep++; 
        }
    }                         
}

// Debounce limit switch
uint8_t limitDebounce(uint32_t* inputSW) {
    if (*inputSW == 0x00) {       // If switch activated
        delayT(1000);         // Wait for bouncing to stop
        if (*inputSW == 0x00) {   // If switch still activated
            return 0x01;      // Report switch activated
        }
        return 0x00;          // Otherwise: switch is unactivated
    } else {
        return 0x00;
    }
}

// Homing Mode
void homingMode(uint32_t* home, uint32_t* output) { // data = 0000 00 & direction & limitSw
    while((limitDebounce(home) == 0x00)) {          // while the mode is unchanged and the switch is unpressed
        stepOnce(DIRECTION_CCW, output);                                 // continuously step in the specified direction
        delayT(10000);
    }
    cStep = 0;                                               // reset current step #
}

// Absolute Positioning Mode 1
// Turns motor to specific step number CW from "home" position using shortest path
// This works for applications with full 360 range of motion
void absPosMode_360(uint8_t number, uint32_t* output) {
    
    uint8_t direction = 0x00;

    // Determine shortest path from cStep to "number" step
    // This algorithm can definitely be improved
    if (number > cStep) {
        if ((number - cStep) < (MAX_STEP / 2)) {
            direction = DIRECTION_CW;
        } else {
            direction = DIRECTION_CCW;
        }
    } else {
        if ((cStep - number) < (MAX_STEP / 2)) {
            direction = DIRECTION_CCW;
        } else {
            direction = DIRECTION_CW;
        }
    }

    // Continuously stepOnce in determined direction until cStep=="data"
    while(cStep != number) {
        stepOnce(direction, output);
        delayT(10000);
    }
}


// Absolute Positioning Mode 2
// Turns motor to specific step number CW from "home" position across "pie slice" of rotation
// This works for motors with limited range of motion due to an obstruction
void absPosMode_Slice(uint8_t stepNum, uint32_t* output) {
    
    uint8_t direction = 0x00;

    // Determine shortest path from cStep to "data" step
    // This algorithm can definitely be improved
    if (stepNum > cStep) {
        direction = DIRECTION_CW;
    } else {
        direction = DIRECTION_CCW;
    }

    // Continuously stepOnce in determined direction until cStep=="data"
    while(cStep != stepNum) {
        stepOnce(direction, output);
        delayT(10000);
    }
}

// Relative Positioning Mode
// Turns motor a specified number of steps in specified direction
// No stop condition
void relPosMode(uint8_t direction, uint8_t numSteps, uint32_t* output) {
    for (int i = 0; i < numSteps; i++) {            
        stepOnce(direction, output);
        delayT(10000);
    }
}

void run(uint8_t mode, uint8_t direction, uint8_t number, uint32_t* home, uint32_t* output) {
    switch (mode) {
        case 0 : // Homing mode
            homingMode(home, output);
            break;
        case 1 : // Absolute positioning mode full range
            absPosMode_360(number, output);
            break;
        case 2 : // Absolute positioning mode limited range
            absPosMode_Slice(number, output);
            break;
        case 3 : // Relative positioning mode
            relPosMode(direction, number, output);
            break;
    }
}