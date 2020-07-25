/*
======= Output ==========
Drive:     PA5-2
Ready:     PA6

======= Input ===========
Home sw:   PB0
Direction: PB1
Mode:      PB2&3
Start:     PB4
Number:    PD3-0 & PC7-4
*/

#include "stepper.h"

void PortA_Init(void) {               
    *SYSCTL_RCGCGPIO_R |= 0x01;       // activate clock on Port A
    *GPIO_PORTA_DIR_R |= 0x7C;        // make PA6-2 pins output
    *GPIO_PORTA_DEN_R |= 0x7C;        // enable digital I/O on PA6-2
}

void PortB_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x02;       // activate clock on Port B
    *GPIO_PORTB_DIR_R &= ~0x1F;       // make PB4-0 pins input
    *GPIO_PORTB_DEN_R |= 0x1F;        // enable digital I/O on PB4-0
    *GPIO_PORTB_PDR_R |= 0x1F;        // enable pull-down resistors
}

void PortC_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x04;       // activate clock on Port C
    *GPIO_PORTC_DIR_R &= ~0xF0;       // make PC7-4 pins input
    *GPIO_PORTC_DEN_R |= 0xF0;        // enable digital I/O on PC7-4
    *GPIO_PORTC_PDR_R |= 0xF0;        // enable pull-down resistors
}

void PortD_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x08;       // activate clock on Port D
    *GPIO_PORTD_DIR_R &= ~0x0F;       // make PD3-0 pins input
    *GPIO_PORTD_DEN_R |= 0x0F;        // enable digital I/O on PD3-0
    *GPIO_PORTD_PDR_R |= 0x0F;        // enable pull-down resistors
}

void PortF_Init(void) {               // initialize sw2
    *SYSCTL_RCGCGPIO_R |= 0x20;       // activate clock on Port F
    *GPIO_PORTF_DIR_R &= ~0x10;       // make PF4 an input pin
    *GPIO_PORTF_DEN_R |= 0x10;        // enable digital i/o on PF4
    *GPIO_PORTF_PUR_R |= 0x10;        // enable PUR on sw2
}