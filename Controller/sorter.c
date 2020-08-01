#include "sorter.h"

void runUMC(uint8_t number, uint8_t mode, uint8_t direction) {
    
    while (UMC_READY == 0) {                      // wait for ready signal
        continue;
    }

    *GPIO_PORTB_DATA_R |= (direction << 1);       // direction
    *GPIO_PORTB_DATA_R |= (mode << 2);            // mode
    *GPIO_PORTD_DATA_R |= ((number & 0xF0) >> 4); // number bits 7-4
    *GPIO_PORTC_DATA_R |= ((number & 0x0F) << 4); // number bits 3-0

    delayT(1000);

    *GPIO_PORTB_DATA_R |= (0x01 << 4);            // set start signal
    delayT(1000);
    *GPIO_PORTB_DATA_R &= ~(0x01 << 4);           // clear start signal
}

void chuteTo(uint8_t address) {
    runUMC(address, 2, 0);
}

int main() {
    gpioInit();
    uint8_t colors[10] = {RED, ORANGE, YELLOW, GREEN, ORANGE, RED, PURPLE, RED, PURPLE, YELLOW};

    for (int i = 0; i < 10; i++) {
        chuteTo(colors[i]);
        delayT(1000000);
    }

    return 0;
}