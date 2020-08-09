#include "sorter_init.h"

// I/O port addresses
uint32_t* SYSCTL_RCGCGPIO_R = (uint32_t*) (0x400FE000 + 0x608);// Run-mode clock gating control
uint32_t* GPIO_PORTA_DIR_R  = (uint32_t*) (0x40004000 + 0x400); // Port A direction
uint32_t* GPIO_PORTA_DEN_R  = (uint32_t*) (0x40004000 + 0x51C); // Port A Digital Enable
uint32_t* GPIO_PORTA_DATA_R = (uint32_t*) (0x40004000 + 0x1F0);// Port A Data for Pins 6-2

uint32_t* GPIO_PORTB_DIR_R  = (uint32_t*) (0x40005000 + 0x400); // Port B direction
uint32_t* GPIO_PORTB_DEN_R  = (uint32_t*) (0x40005000 + 0x51C); // Port B Digital Enable
uint32_t* GPIO_PORTB_DATA_R = (uint32_t*) (0x40005000 + 0x07C); // Port B Data Pins 4-0
uint32_t* GPIO_PORTB_PDR_R  = (uint32_t*) (0x40005000 + 0x514); // Port B Pull-down Resistors

uint32_t* GPIO_PORTF_DIR_R = (uint32_t*) (0x40025000 + 0x400); // Port F direction
uint32_t* GPIO_PORTF_DEN_R = (uint32_t*) (0x40025000 + 0x51C); // Port F Digital Enable
uint32_t* GPIO_PORTF_DATA_R = (uint32_t*) (0x40025000 + 0x44); // Port F Data for Pin PF4 (sw1)
uint32_t* GPIO_PORTF_PUR_R = (uint32_t*) (0x40025000 + 0x510); // Port F Pull-up Resistors

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

void PortF_Init(void) {               // initialize sw2
    *SYSCTL_RCGCGPIO_R |= 0x20;       // activate clock on Port F
    *GPIO_PORTF_DIR_R &= ~0x11;       // make PF4&0 input
    *GPIO_PORTF_DEN_R |= 0x11;        // enable digital i/o on PF4
    *GPIO_PORTF_PUR_R |= 0x11;        // enable PUR on sw2 & sw1
}

void gpioInit(void) {
    PortA_Init();
    PortB_Init();
    PortF_Init();
}