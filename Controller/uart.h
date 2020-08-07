#ifndef UART_H
#define UART_H

void setup_uart();
void uartwrite(uint8_t data);
void say_hello();
void say_two_bytes(uint16_t data);
void say_byte(uint8_t data);
void new_line();
void say_nibble(uint8_t dataNibble);
char hexToChar(uint8_t inputNum);
void say_string(char * string, int length);

#endif