#include "sorter.h"
#include "stepper.h"
#include "sorter_init.h"
#include "I2C.h"

uint8_t colors[6] = {REJECT, RED, ORANGE, YELLOW, GREEN, PURPLE};

void chuteToColor(int color) {
    absPosMode_Slice(colors[color]);
}

void turnTable() {
    // IMPORTANT NOTE: MAX_STEP IS NOT DIVISIBLE BY 3, THIS WILL CAUSE DRIFT
    relPosMode(DIRECTION_CCW, MAX_STEP / 3);
}

int main() {
    gpioInit();
    //setup();
    
    while (true) {
        //chuteToColor(read_colors());
        homingMode();
        delayT(1000000);
    }

    return 0;
}