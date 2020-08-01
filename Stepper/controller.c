#include "stepper.h"
#include "umc_init.h"
// include colorSensor.h

#define RED    0xC8
#define GREEN  0x96
#define ORANGE 0x64
#define PURPLE 0x32
#define YELLOW 0x00

uint8_t colors[5] = {RED, ORANGE, YELLOW, GREEN, PURPLE};

void chuteToColor(uint8_t color) {
    absPosMode_Slice(color);
}

int main() {
    gpioInit();
    
    for (int i = 0; i < 5; i++) {
        chuteToColor(colors[i]);
        delayT(1000000);
    }

    return 0;
} 