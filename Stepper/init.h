#ifndef INIT_H
#define INIT_H

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

// I/O port addresses
uint32_t *SYSCTL_RCGCGPIO_R;// Run-mode clock gating control
uint32_t *GPIO_PORTA_DIR_R; // Port A direction
uint32_t *GPIO_PORTA_DEN_R; // Port A Digital Enable
uint32_t *GPIO_PORTA_DATA_R;// Port A Data for Pins 6-2

uint32_t *GPIO_PORTB_DIR_R; // Port B direction
uint32_t *GPIO_PORTB_DEN_R; // Port B Digital Enable
uint32_t *GPIO_PORTB_DATA_R; // Port B Data Pins 4-0
uint32_t *GPIO_PORTB_PDR_R; // Port B Pull-down Resistors

uint32_t *GPIO_PORTC_DIR_R; // Port C direction
uint32_t *GPIO_PORTC_DEN_R; // Port C Digital Enable
uint32_t *GPIO_PORTC_DATA_R;// Port C Data Pins 7-4
uint32_t *GPIO_PORTC_PDR_R; // Port C Pull-down Resistors

uint32_t *GPIO_PORTD_DIR_R; // Port D direction
uint32_t *GPIO_PORTD_DEN_R; // Port D Digital Enable
uint32_t *GPIO_PORTD_DATA_R; // Port D Data Pins 3-0
uint32_t *GPIO_PORTD_PDR_R; // Port D Pull-down Resistors

uint32_t *GPIO_PORTF_DIR_R; // Port F direction
uint32_t *GPIO_PORTF_DEN_R; // Port F Digital Enable
uint32_t *GPIO_PORTF_DATA_R; // Port F Data for Pin PF0 (sw2)
uint32_t *GPIO_PORTF_PUR_R; // Port F Pull-up Resistors

void gpioInit(void);
void PortA_Init(void);
void PortB_Init(void);
void PortC_Init(void);
void PortD_Init(void);
void PortF_Init(void);

#endif