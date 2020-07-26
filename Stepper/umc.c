#include "stepper.h"
#include "init.h"

#define HOME (*GPIO_PORTB_DATA_R & 0x01)

int main() {
    gpioInit();

    uint8_t cState = 0x00; // track current motor state
    uint8_t home = HOME;   // replace with home input pin
    uint8_t dir = 0x00;    // replace with direction input pin
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