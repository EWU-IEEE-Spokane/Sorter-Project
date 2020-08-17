#include "stepper.h"
#include "umc_init.h"

int main() {
    gpioInit();
    *GPIO_PORTA_DATA_R = 0x00;  // READY output signal

    while(1) {
        *GPIO_PORTA_DATA_R |= (0x01 << 6);      // Set Ready Bit
        if (START == 0x01) {
            *GPIO_PORTA_DATA_R &= ~(0x01 << 6); // Clear Ready Bit
            run(MODE, DIRECTION, NUMBER, GPIO_PORTB_DATA_R, GPIO_PORTA_DATA_R);
        }
        break;
    }
    
    return 0;
}