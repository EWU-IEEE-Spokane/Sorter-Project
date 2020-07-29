#include "stepper.h"
#include "init.h"



int main() {
    gpioInit();

    uint8_t cState = 0x00;      // track current motor state
    //uint8_t home = HOME;      // replace with home input pin
    //uint8_t dir = DIRECTION;  // replace with direction input pin
    //uint8_t mode = MODE;      // replace with mode input pin
    //uint8_t start = START;    // replace with start input pin  
    *GPIO_PORTA_DATA_R = 0x00;  // READY output signal

    while(1) {
        *GPIO_PORTA_DATA_R |= (0x01 << 6);
        if (START == 0x01) {
            *GPIO_PORTA_DATA_R &= ~(0x01 << 6);
            run(MODE, &cState);
        }
        break;
    }
    
    return 0;
}