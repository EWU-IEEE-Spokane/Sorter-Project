#include "stepper.h"
#include "init.h"

#define HOME (*GPIO_PORTB_DATA_R & 0x01)
#define DIRECTION (*GPIO_PORTB_DATA_R & 0X02)
#define MODE (*GPIO_PORTB_DATA_R & 0x0C)
#define START (*GPIO_PORTB_DATA_R & 0x10)
#define NUMBER (((*GPIO_PORTD_DATA_R & 0x0F) << 4) | (*GPIO_PORTC_DATA_R & 0xF0))

int main() {
    gpioInit();

    uint8_t cState = 0x00; // track current motor state
    uint8_t home = HOME;   // replace with home input pin
    uint8_t dir = DIRECTION;    // replace with direction input pin
    uint8_t mode = MODE;      // replace with mode input pin
    uint8_t start = START;  // replace with start input pin  
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