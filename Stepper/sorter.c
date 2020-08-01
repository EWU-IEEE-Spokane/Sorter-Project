#include "stepper.h"
#include "init.h"

#define RED    0xC8
#define GREEN  0x96
#define ORANGE 0x64
#define PURPLE 0x32
#define YELLOW 0x00

int main() {
    gpioInit();
    uint8_t colors[10] = {RED, ORANGE, YELLOW, GREEN, ORANGE, RED, PURPLE, RED, PURPLE, YELLOW};

    for (int i = 0; i < 10; i++) {
        absPosMode_Slice(colors[i]);
        delayT(1000000);
    }

    return 0;
}