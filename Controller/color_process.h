#ifndef COLOR_PROCESS_H
#define COLOR_PROCESS_H

#include <stdint.h>

float process_values(uint16_t, uint16_t, uint16_t);
uint16_t applyGammaTable(float);
int decideInf(char*, char);
int decideMid(char*, char, char);
int decideOne(char*, char);
int determine_color(uint16_t, uint16_t, uint16_t );
void colorSort(uint8_t *);
void say_rgb(uint8_t, uint8_t, uint8_t);
void mapColorChars(char*, uint8_t*, uint8_t, uint8_t, uint8_t);

#endif