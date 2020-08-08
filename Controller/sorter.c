#include "sorter.h"
#include "stepper.h"
#include "sorter_init.h"
#include "i2c.h"
#include "color_sensor.h"
#include "clock_init.h"
#include "uart.h"
#include "led.h"
#include "pwm.h"

uint8_t colors[6] = {REJECT, RED, ORANGE, YELLOW, GREEN, PURPLE};

void configure() {
    setup_clock();
    
    setup_I2C();
    setup_LEDs();
    setup_uart();
    //config_sensor();
    configure_pwm();

    gpioInit();
}

void chuteToColor(int color) {
    absPosMode_Slice(colors[color], GPIO_PORTA_DATA_R);
}

void turnTable() {
    // IMPORTANT NOTE: MAX_STEP IS NOT DIVISIBLE BY 3, THIS WILL CAUSE DRIFT
    relPosMode(DIRECTION_CW, MAX_STEP / 3, GPIO_PORTA_DATA_R);
}

void singleSort() {
    // load skittle A
    // move skittle B to sensor
    // dispense skittle C
    turnTable();

    // read color of skittle B
    // move chute to skittle B color's bin
    chuteToColor(read_colors());

    // on next iteration, skittle B becomes skittle C; A becomes B
}

int main() {
    configure();
    
    while (1) {
        //chuteToColor(read_colors());
        //homingMode(GPIO_PORTB_DATA_R, GPIO_PORTA_DATA_R);
        for (int i = 0; i < 6; i++) {
            chuteToColor(i);
        }
        delayT(1000000);
    }

    return 0;
}