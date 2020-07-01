#include <stdint.h>
#include <stdlib.h>
#include "stepper.h"

uint16_t cStep = 0; // Current step number; i.e. address for absolute positioning; should be "home" position

void PortA_Init(void) {             // initialize stepper data pins
  *SYSCTL_RCGCGPIO_R |= 0x01;       // activate clock on Port A
  *GPIO_PORTA_DIR_R |= 0x3C;        // make PA5-2 pins output
  *GPIO_PORTA_DEN_R |= 0x3C;        // enable digital I/O on PA5-2
}

void PortF_Init(void) {               // initialize sw2
    *SYSCTL_RCGCGPIO_R |= 0x20;       // activate clock on Port F
    *GPIO_PORTF_DIR_R &= ~0x10;       // make PF4 an input pin
    *GPIO_PORTF_DEN_R |= 0x10;        // enable digital i/o on PF4
    *GPIO_PORTF_PUR_R |= 0x10;        // enable PUR on sw2
}

// Define the states in the 28byJ FSM
State_t stepFsm[4] = {
    {0, 0x04, {1, 3} },
    {1, 0x08, {2, 0} },
    {2, 0x10, {3, 1} },
    {3, 0x20, {0, 2} }
};

// Quick and dirty busy-loop delay
// This will need updating to a more precise delay
void delayT(int maxCount) {
    for (volatile int j = 0; j < maxCount; j++) {continue;}
}

// Step the motor once
// Direction map: 0x00 = CW; 0x01 = CCW; 
void stepOnce(uint8_t direction, uint8_t *cState) {
    *cState = stepFsm[*cState].next[direction]; // Move to next state defined by direction
    *GPIO_PORTA_DATA_R = stepFsm[*cState].out;  // Output motor data defined in new state
    
    // Update cStep--this algorithm can be improved
    if (direction == 0x01) { 
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
        cStep++; 
    }                         
}

// Debounce a button input
// This function must be called at a high frequency to work
// Inverts the "input" variable after the button is released
void debounce(uint8_t* input, uint8_t* flag) {
    if (*GPIO_PORTF_DATA_R == 16) {             // switch unpressed
            delayT(1000);
            if (*GPIO_PORTF_DATA_R == 16 && *flag == 0) {
                *input = (~(*input))&0x01;      // invert input
                *flag = 1;
            }
    } else {
        *flag = 0;
    }
}


// Debounce limit switch
uint8_t limitDebounce(uint32_t* limitSw) {
    if( (((*limitSw&0x10)>>4)) == 0x00) {       // If switch activated
        delayT(1000);                         // Wait for bouncing to stop
        if( (((*limitSw&0x10)>>4)) == 0x00) {   // If switch still activated
            return 0x00;                      // Report switch activated
        }
        return 0x01;                          // Otherwise: switch is unactivated
    } else {
        return 0x01;
    }
}

// Homing Mode
void homingMode(uint32_t* data, volatile uint8_t* mode, uint8_t* cState) { // data = 0000 00 & direction & limitSw
    while(*mode == 0 && (limitDebounce(data)) == 0x01) {                    // while the mode is unchanged and the switch is unpressed
        stepOnce((*data&0x02)>>1, cState);                                // continuously step in the specified direction
        delayT(10000);
    }

    cStep = 0;                                                            // reset current step #
}

// Absolute Positioning Mode 1
// Turns motor to specific step number CW from "home" position using shortest path
// This works for applications with full 360 range of motion
void absPosMode_360(uint8_t data, uint8_t* cState) {
    
    uint8_t direction = 0x00;

    // Determine shortest path from cStep to "data" step
    // This algorithm can definitely be improved
    if (data > cStep) {
        if ((data - cStep) < (MAX_STEP / 2)) {
            direction = DIRECTION_CW;
        } else {
            direction = DIRECTION_CCW;
        }
    } else {
        if ((cStep - data) < (MAX_STEP / 2)) {
            direction = DIRECTION_CCW;
        } else {
            direction = DIRECTION_CW;
        }
    }

    // Continuously stepOnce in determined direction until cStep=="data"
    while(cStep != data) {
        stepOnce(direction, cState);
        delayT(10000);
    }
}


// Absolute Positioning Mode 2
// Turns motor to specific step number CW from "home" position across "pie slice" of rotation
// This works for motors with limited range of motion due to an obstruction
void absPosMode_Slice(uint8_t data, uint8_t* cState) {
    
    uint8_t direction = 0x00;

    // Determine shortest path from cStep to "data" step
    // This algorithm can definitely be improved
    if (data > cStep) {
        direction = DIRECTION_CW;
    } else {
        direction = DIRECTION_CCW;
    }

    // Continuously stepOnce in determined direction until cStep=="data"
    while(cStep != data) {
        stepOnce(direction, cState);
        delayT(10000);
    }
}

// Relative Positioning Mode
// Turns motor a specified number of steps in specified direction
// No stop condition
void relPosMode(uint8_t direction, uint8_t* cState, uint8_t numSteps) {
    for (int i = 0; i < numSteps; i++) {            
        stepOnce(direction, cState);
        delayT(10000);
    }
}

uint16_t serialIn() {
    return 150;
}

void run(uint8_t mode, uint8_t* cState) {
    uint16_t stepNum;

    if (mode != 0) { stepNum = serialIn(); }

    switch (mode) {
        case 0 : // Homing mode
            homingMode(GPIO_PORTF_DATA_R, &mode, cState);
            break;
        case 1 : // Absolute positioning mode full range
            absPosMode_360(stepNum, cState);
            break;
        case 2 : // Absolute positioning mode limited range
            absPosMode_Slice(stepNum, cState);
            break;
        case 3 : // Relative positioning mode
            relPosMode(stepNum, cState, 200);
            break;
    }
}

/*
Important notes:
    - Everything here is built around the 28byj-48 motor, but switching to the nema-17 should just
      be a matter of adding a new stepper FSM data type

    - Currently the only real input is Sw1 on the launchpad, this is mapped to Pf4

    - An 8 bit input is necessary to specify a step number (0-199) for absolute positioning with
      the NEMA-17, this probably should be done serially, but for now, a 4-bit parallel bus
      could work to give us 16 positions across the range of motion, or we could use an 8-bit parallel bus
      which is a lot of pins but ¯\_(ツ)_/¯
*/
int main() {
    PortA_Init(); // initialize stepper data output port
    PortF_Init(); // initialize switch input port

    uint8_t cState = 0x00; // track current motor state
    uint8_t home = 0x01;   // replace with home input pin
    uint8_t dir = 0x01;    // replace with direction input pin
    uint8_t mode = 0;      // replace with mode input pin
    uint8_t start = 0x01;  // replace with start input pin  
    uint8_t ready = 0x00;  // map to output pin

    while(1) {
        ready = 0x01;
        if (start == 0x01) {
            ready = 0x00;
            run(mode, &cState);
        }
        break;
    }
    
    return 0;
}