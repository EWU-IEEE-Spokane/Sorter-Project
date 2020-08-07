#include "sorter.h"
#include "stepper.h"
#include "sorter_init.h"
#include "i2c.h"
#include "color_sensor.h"
#include "clock_init.h"
#include "uart.h"
#include "led.h"

uint8_t colors[6] = {REJECT, RED, ORANGE, YELLOW, GREEN, PURPLE};

void configure() {
    setup_clock();
    gpioInit();
    setup_I2C();
    setup_LEDs();
    setup_uart();
    config_sensor();
}

void chuteToColor(int color) {
    absPosMode_Slice(colors[color], GPIO_PORTA_DATA_R);
}

void turnTable() {
    // IMPORTANT NOTE: MAX_STEP IS NOT DIVISIBLE BY 3, THIS WILL CAUSE DRIFT
    relPosMode(DIRECTION_CCW, MAX_STEP / 3, GPIO_PORTA_DATA_R);
}

void singleSort() {
    // load skittle (turn auger)
    // turnTable (move skittle to sensor)
    turnTable();

    // read color
    // move chute to color
    chuteToColor(read_colors());

    // turnTable (drop skittle to chute)
    turnTable();
}

int main() {
    configure();
    
    while (1) {
        chuteToColor(read_colors());
        homingMode(GPIO_PORTB_DATA_R, GPIO_PORTA_DATA_R);
        delayT(1000000);
    }

    return 0;
}