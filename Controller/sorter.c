#include "sorter.h"
#include "stepper.h"
#include "sorter_init.h"
#include "i2c.h"
#include "color_sensor.h"
#include "clock_init.h"
#include "uart.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"

#define TABLE_OFFSET_FROM_HOME 0x00

const uint8_t colors[6] = {REJECT, RED, ORANGE, YELLOW, GREEN, PURPLE};

/*
 *  PIN MAPPING:
 *      PA7: I2C SDA
 *      PA6: I2C SCL
 *      PA5-2: TABLE STEPPER DRIVER OUTPUT
 *      PB0: TABLE HOME LIMIT SW
 *      PB1: START SW
 *      PB5-2: CHUTE STEPPER DRIVER OUTPUT
 *      PF0: SW2 (TEST HOME SW)
 *      PF1: ON-BOARD RED LED
 *      PF2: ON-BOARD BLUE LED
 *      PF3: ON-BOARD GREEN LED
 *      PF4: SW1 (TEST START)
 */


/*
 * Each module used for the sorter has its own set of configuration steps, these are ran once
 * at startup. Any additional modules' configuration code should be added to the configure() 
 * function.
 */
void configure() {
    setup_clock();
    Config_SysTick();
    
    setup_I2C();
    setup_LEDs();
    setup_uart();
    //  config_sensor();
    configure_pwm();

    gpioInit();
}


/*
 * When a color is read from the sensor, this function handles moving the chute to the correct bin.
 */
void chuteToColor(int color) {
    absPosMode_Slice(colors[color], GPIO_PORTA_DATA_R);
}


/*
 * Every third call to turnTable applies a "leap step" to account for the fact there are
 * an even number of steps to one full rotation, and we are aiming for 1/3 of one rotation
 * for each funtion call.
 */
void turnTable() {
    static uint8_t currentRotation;
    uint8_t numberOfSteps = 133;
    currentRotation++;
    
    if (currentRotation == (uint8_t)3)
    {
        currentRotation = 0;
        numberOfSteps++;
    }

    relPosMode(DIRECTION_CW, numberOfSteps, GPIO_PORTA_DATA_R);
}


/* 
 * When the table moves to the home position, the point at which the limit switch triggers
 * is not exactly at zero degrees, so the table is not exactly aligned with the base plate. A
 * couple additional steps indicated by TABLE_OFFSET_FROM_HOME may be necessary to align precisely.
 * This value may need to be calibrated on a machine to machine basis.
 */
void homeTable() {
    // Move to home switch
    homingMode(GPIO_PORTB_DATA_R, 0, GPIO_PORTA_DATA_R);
    ms_delay(10000);
    // Adjust for offset from home switch triggering
    relPosMode(DIRECTION_CCW, TABLE_OFFSET_FROM_HOME, GPIO_PORTA_DATA_R);
}


/*
 * Each rotation of the table involves the movement of 3 skittles.
 *  (1) Skittle A is loaded from the hopper
 *  (2) Skittle B is moved from the hopper station to the sensor
 *  (3) Skittle C is moved from the sensor station and dispensed to the chute
 * 
 * After each rotation, the color of skittle B is read, and the chute is moved
 * to the correct bin to prepare for dispensing during the next rotation.
 * 
 * After each rotation, the hopper auger rotates to stage the next "Skittle A" for loading
 */
void singleSort() {
    // rotate table
    turnTable();

    // read color of skittle B
    // move chute to skittle B color's bin
    chuteToColor(read_colors());

    //hopperCycle();
}


int main() {
    configure();

    // 3 Green blinks to show successful configure() execution
    blink(0x08);
    blink(0x08);
    blink(0x08);
    
    while (1) {
        while ((*GPIO_PORTF_DATA_R & 0x10) >> 4) { ; } /* Wait for SW1 to be pressed */

        /* ====================== TEST CODE WITH SENSOR ================================ 
        homeTable();
        chuteToColor(read_colors());

         ====================== TEST CODE WITHOUT SENSOR ============================= */
        //ms_delay(10000);
        // Move to home position
         homingMode(GPIO_PORTB_DATA_R, 0, GPIO_PORTA_DATA_R); // Use PB0 for home switch
        // homingMode(GPIO_PORTF_DATA_R, 0, GPIO_PORTA_DATA_R); // Use SW2 for home switch
        //homeTable();

        // Wait at home for short period
        ms_delay(10000);

        // Run test sequence
        for (int i = 0; i < 200; i++) {
            //chuteToColor(i);
            turnTable();
            ms_delay(1000);
        }
         /*========================== END TEST CODE ==================================== */


        /* =========================== CONTROL LOOP ==================================== 

        homeTable();

        // A button triggered "stop" interrupt could pause execution after current operation is done
        // A button triggered "e-stop" could pause immediately and cut motor power
        while (1)
        {
            singleSort();
        }
         ========================== END CONTROL LOOP ================================= */
    }

    return 0;
}