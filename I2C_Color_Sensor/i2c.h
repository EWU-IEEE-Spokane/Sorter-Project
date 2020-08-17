#ifndef I2C_H
#define I2C_H

#define PORT_A_BASE_ADDRESS 0x40004000
#define I2C1_BASE_ADDRESS	0x40021000
#define SENSOR_ADDRESS_WRITE 0x52
#define SENSOR_ADDRESS_READ	0x53

void setup_I2C();
void single_write(uint8_t dataIn);
uint8_t single_read();
void wait_on_bus();


#endif