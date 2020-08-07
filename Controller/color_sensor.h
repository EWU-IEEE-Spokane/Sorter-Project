#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

void config_sensor();
void write_sensor(uint8_t addressIn, uint8_t dataIn);
uint8_t read_sensor(uint8_t addressIn);
int read_colors();
void wait_on_adc();
int read_colors();

#endif