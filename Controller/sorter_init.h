#ifndef SORTER_INIT_H
#define SORTER_INIT_H
#include <stdint.h>

/*
======= Output ==========
Drive:     PA5-2
Ready:     PA6

======= Input ===========
Home sw:   PB0
Start:     PB4
*/

#define HOME (*GPIO_PORTB_DATA_R & 0x01)
#define MODE ((*GPIO_PORTB_DATA_R & 0x0C) >> 2)
#define START ((*GPIO_PORTB_DATA_R & 0x10) >> 4)

#define GPIO_PORTB_ICR (*(uint32_t*)(0x40005000 + 0x41C))
#define GPIO_PORTB_RIS (*(uint32_t*)(0x40005000 + 0x414))
#define NVIC_EN0_R (*(uint32_t*)(0xE000E000 + 0x100))
#define NVIC_PRI0_R (*(uint32_t*)(0xE000E000 + 0x400))

// I/O port addresses
uint32_t *SYSCTL_RCGCGPIO_R;// Run-mode clock gating control
uint32_t *GPIO_PORTA_DIR_R; // Port A direction
uint32_t *GPIO_PORTA_DEN_R; // Port A Digital Enable
uint32_t *GPIO_PORTA_DATA_R;// Port A Data for Pins 6-2

uint32_t *GPIO_PORTB_DIR_R; // Port B direction
uint32_t *GPIO_PORTB_DEN_R; // Port B Digital Enable
uint32_t *GPIO_PORTB_DATA_R;// Port B Data Pins 4-0
uint32_t *GPIO_PORTB_PDR_R; // Port B Pull-down Resistors

uint32_t *GPIO_PORTF_DIR_R; // Port F direction
uint32_t *GPIO_PORTF_DEN_R; // Port F Digital Enable
uint32_t *GPIO_PORTF_DATA_R;// Port F Data for Pin PF0 (sw2)
uint32_t *GPIO_PORTF_PUR_R; // Port F Pull-up Resistors

uint32_t* GPIO_PORTD_DIR_R;
uint32_t* GPIO_PORTD_DEN_R;
uint32_t* GPIO_PORTD_DATA_R;

uint32_t* PWM_M1PWM2CTL_R; // PWM Module 1 Control R for PWM on PF1
uint32_t* PWM_M1PWM3CTL_R; // PWM Module 1 Control R for PWM on PF2-3
uint32_t* PWM_PWMENABLE_R; // PWM Enable
uint32_t* PWM_PWM3COMPA_R; // PWM Gen 3 Comparator Value
uint32_t* PWM_PWM2COMPA_R; // PWM Gen 2 Comparator Value
uint32_t* PWM_PWM3COMPB_R; // PWM Gen 3 Comp B Value

void gpioInit(void);
void PortA_Init(void);
void PortB_Init(void);
void PortF_Init(void);
void PortD_Init(void);

#endif