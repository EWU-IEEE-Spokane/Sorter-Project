#include "stepper.h"
#include "umc_init.h"
#include "I2C.h"

#define RED    0xC8
#define GREEN  0x96
#define ORANGE 0x64
#define PURPLE 0x32
#define YELLOW 0x00
#define REJECT 0xFF

uint8_t colors[5] = {REJECT, RED, ORANGE, YELLOW, GREEN, PURPLE};

void chuteToColor(int color) {
    absPosMode_Slice(colors[color]);
}

int main() {
    gpioInit();
    setup();
    
    while (true) {
        chuteToColor(read_colors());
        delayT(1000000);
    }

    return 0;
}