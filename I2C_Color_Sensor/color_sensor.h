#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include "led.h"
#include "i2c.h"
#include "uart.h"
#include "color_sensor.h"
#include "color_process.h"

void config_sensor();
void write_sensor(uint8_t addressIn, uint8_t dataIn);
uint8_t read_sensor(uint8_t addressIn);
int read_colors();

#endif