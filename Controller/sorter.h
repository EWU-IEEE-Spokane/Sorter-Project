#ifndef SORTER_H
#define SORTER_H

#include "stepper.h"
#include "sorter_init.h"

#define RED    0xC8
#define GREEN  0x96
#define ORANGE 0x64
#define PURPLE 0x32
#define YELLOW 0x00

void runUMC(uint8_t number, uint8_t mode, uint8_t direction);

void loadSkittle();
uint8_t readColor();
void chuteTo(uint8_t address);
void dispense();

#endif