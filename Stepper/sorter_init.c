#include "sorter_init.h"

// I/O port addresses
uint32_t* SYSCTL_RCGCGPIO_R = (uint32_t*) (0x400FE000 + 0x608); // Run-mode clock gating control
uint32_t* GPIO_PORTA_DIR_R  = (uint32_t*) (0x40004000 + 0x400); // Port A direction
uint32_t* GPIO_PORTA_DEN_R  = (uint32_t*) (0x40004000 + 0x51C); // Port A Digital Enable
uint32_t* GPIO_PORTA_DATA_R = (uint32_t*) (0x40004000 + 0x1F0); // Port A Data for Pins 6-2
uint32_t* GPIO_PORTA_PDR_R  = (uint32_t*) (0x40004000 + 0x514); // Port A Pull-down Resistors

uint32_t* GPIO_PORTB_DIR_R  = (uint32_t*) (0x40005000 + 0x400); // Port B direction
uint32_t* GPIO_PORTB_DEN_R  = (uint32_t*) (0x40005000 + 0x51C); // Port B Digital Enable
uint32_t* GPIO_PORTB_DATA_R = (uint32_t*) (0x40005000 + 0x78);  // Port B Data Pins 4-1
uint32_t* GPIO_PORTB_PDR_R  = (uint32_t*) (0x40005000 + 0x514); // Port B Pull-down Resistors

uint32_t* GPIO_PORTC_DIR_R  = (uint32_t*) (0x40006000 + 0x400); // Port C direction
uint32_t* GPIO_PORTC_DEN_R  = (uint32_t*) (0x40006000 + 0x51C); // Port C Digital Enable
uint32_t* GPIO_PORTC_DATA_R = (uint32_t*) (0x40006000 + 0x3C0); // Port C Data Pins 7-4
uint32_t* GPIO_PORTC_PDR_R  = (uint32_t*) (0x40006000 + 0x514); // Port C Pull-down Resistors

uint32_t* GPIO_PORTD_DIR_R  = (uint32_t*) (0x40007000 + 0x400); // Port D direction
uint32_t* GPIO_PORTD_DEN_R  = (uint32_t*) (0x40007000 + 0x51C); // Port D Digital Enable
uint32_t* GPIO_PORTD_DATA_R = (uint32_t*) (0x40007000 + 0x3C);  // Port D Data Pins 3-0
uint32_t* GPIO_PORTD_PDR_R  = (uint32_t*) (0x40007000 + 0x514); // Port D Pull-down Resistors

uint32_t* GPIO_PORTF_DIR_R  = (uint32_t*) (0x40025000 + 0x400); // Port F direction
uint32_t* GPIO_PORTF_DEN_R  = (uint32_t*) (0x40025000 + 0x51C); // Port F Digital Enable
uint32_t* GPIO_PORTF_DATA_R = (uint32_t*) (0x40025000 + 0x40);  // Port F Data for Pin PF0 (sw2)
uint32_t* GPIO_PORTF_PUR_R  = (uint32_t*) (0x40025000 + 0x510); // Port F Pull-up Resistors

void PortA_Init(void) {               
    *SYSCTL_RCGCGPIO_R |=  0x01;      // activate clock on Port A
    *GPIO_PORTA_DIR_R  &= ~0x40;      // make PA6 pins input
    *GPIO_PORTA_DEN_R  |=  0x40;      // enable digital I/O on PA6
    *GPIO_PORTA_PDR_R  |=  0x40;      // enable pull-down resistors
}

void PortB_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x02;       // activate clock on Port B
    *GPIO_PORTB_DIR_R  |= 0x1E;       // make PB4-1 pins output
    *GPIO_PORTB_DEN_R  |= 0x1E;       // enable digital I/O on PB4-1
}

void PortC_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x04;       // activate clock on Port C
    *GPIO_PORTC_DIR_R  |= 0xF0;       // make PC7-4 pins output
    *GPIO_PORTC_DEN_R  |= 0xF0;       // enable digital I/O on PC7-4
}

void PortD_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x08;       // activate clock on Port D
    *GPIO_PORTD_DIR_R  |= 0x0F;       // make PD3-0 pins output
    *GPIO_PORTD_DEN_R  |= 0x0F;       // enable digital I/O on PD3-0
}

void PortF_Init(void) {               // initialize sw2
    *SYSCTL_RCGCGPIO_R |= 0x20;       // activate clock on Port F
    *GPIO_PORTF_DIR_R  &= ~0x10;      // make PF4 an input pin
    *GPIO_PORTF_DEN_R  |= 0x10;       // enable digital i/o on PF4
    *GPIO_PORTF_PUR_R  |= 0x10;       // enable PUR on sw2
}

void gpioInit(void) {
    PortA_Init();
    PortB_Init();
    PortC_Init();
    PortD_Init();
//  PortF_Init();
}