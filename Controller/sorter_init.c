#include "sorter_init.h"

uint32_t* SYSCTL_RCGCGPIO_R = (uint32_t*) (0x400FE000 + 0x608); // Run-mode clock gating control

uint32_t* GPIO_PORTA_DIR_R  = (uint32_t*) (0x40004000 + 0x400); // Port A direction
uint32_t* GPIO_PORTA_DEN_R  = (uint32_t*) (0x40004000 + 0x51C); // Port A Digital Enable
uint32_t* GPIO_PORTA_DATA_R = (uint32_t*) (0x40004000 + 0x0F0); // Port A Data for Pins 5-2 

uint32_t* GPIO_PORTB_DIR_R  = (uint32_t*) (0x40005000 + 0x400); // Port B direction
uint32_t* GPIO_PORTB_DEN_R  = (uint32_t*) (0x40005000 + 0x51C); // Port B Digital Enable
uint32_t* GPIO_PORTB_DATA_R = (uint32_t*) (0x40005000 + 0x0FC); // Port B Data Pins 5-0
uint32_t* GPIO_PORTB_PDR_R  = (uint32_t*) (0x40005000 + 0x514); // Port B Pull-down Resistors
uint32_t* GPIO_PORTB_IS     = (uint32_t*) (0x40005000 + 0x404); // Port B Interrupt Sense
uint32_t* GPIO_PORTB_IM     = (uint32_t*) (0x40005000 + 0x410); // Interrupt mask
uint32_t* GPIO_PORTB_IBE    = (uint32_t*) (0x40005000 + 0x408); // Interrupt both edges
uint32_t* GPIO_PORTB_IEV    = (uint32_t*) (0x40005000 + 0x40C); // Interrupt event

uint32_t* GPIO_PORTD_DATA_R = (uint32_t*) (0x40007000 + 0x33C); // Port D 
uint32_t* GPIO_PORTD_DIR_R  = (uint32_t*) (0x40007000 + 0x400); // Port D direction
uint32_t* GPIO_PORTD_DEN_R  = (uint32_t*) (0x40007000 + 0x51C); // Port D Digital Enable
uint32_t* GPIO_PORTD_LOCK_R = (uint32_t*) (0x40007000 + 0x520); // Port D Lock register
uint32_t* GPIO_PORTD_CR     = (uint32_t*) (0x40025000 + 0x524); // Port D Commit register

uint32_t* GPIO_PORTF_DIR_R  = (uint32_t*) (0x40025000 + 0x400); // Port F direction
uint32_t* GPIO_PORTF_LOCK_R = (uint32_t*) (0x40025000 + 0x520); // Port F Lock register
uint32_t* GPIO_PORTF_CR     = (uint32_t*) (0x40025000 + 0x524); // Port F Commit register
uint32_t* GPIO_PORTF_DEN_R  = (uint32_t*) (0x40025000 + 0x51C); // Port F Digital Enable
uint32_t* GPIO_PORTF_DATA_R = (uint32_t*) (0x40025000 + 0x44);  // Port F Data for Pin PF4 (sw1)
uint32_t* GPIO_PORTF_PUR_R  = (uint32_t*) (0x40025000 + 0x510); // Port F Pull-up Resistors
uint32_t* GPIO_PORTF_AFSEL 	= (uint32_t*) (0x40025000 + 0x420); // Alternative function select for choosing function other than GPIO on Port F, page 671
uint32_t* GPIO_PORTF_PTCL   = (uint32_t*) (0x40025000 + 0x52C); // Port Control register for choosing PWM as the alternate function to use, pages 688,

uint32_t* SYSCTL_RCGCPWM_R  = (uint32_t*) (0x400FE000 + 0x640); // Run-mode clock gating control for PWM modules
uint32_t* SYSCTL_RCC_R      = (uint32_t*) (0x400FE000 + 0x060); // Run mode clock configuration
uint32_t* PWM_M1PWM2CTL_R   = (uint32_t*) (0x40029000 + 0x0C0); // PWM Module 1, Block 2 Control, for M1PWM4-5
uint32_t* PWM_M1PWM3CTL_R   = (uint32_t*) (0x40029000 + 0x100); // PWM Module 1, Block 3 Control, for M1PWM6-7
uint32_t* PWM_PWM2GENB_R    = (uint32_t*) (0x40029000 + 0x0E4); // PWM Module 1, Block 2, Generator B Configuration
uint32_t* PWM_PWM3GENA_R    = (uint32_t*) (0x40029000 + 0x120); // PWM Module 1, Block 3, Generator A Configuration
uint32_t* PWM_PWM3GENB_R    = (uint32_t*) (0x40029000 + 0x124); // PWM Module 1, Block 3, Generator B Configuration
uint32_t* PWM_PWM2LOAD_R    = (uint32_t*) (0x40029000 + 0x0D0); // PWM Module 1, Block 2, Counter Load Value
uint32_t* PWM_PWM3LOAD_R    = (uint32_t*) (0x40029000 + 0x110); // PWM Module 1, Block 2, Counter Load Value
uint32_t* PWM_PWMENABLE_R   = (uint32_t*) (0x40029000 + 0x008); // PWM Enable for Module 1
uint32_t* PWM_PWM3COMPA_R   = (uint32_t*) (0x40029000 + 0x118); // PWM Block 3 Comparator A Value
uint32_t* PWM_PWM2COMPA_R   = (uint32_t*) (0x40029000 + 0x0D8); // PWM Block 2 comparator A value
uint32_t* PWM_PWM3COMPB_R   = (uint32_t*) (0x40029000 + 0x11C); // PWM Block 3 Comparator B Value


void PortA_Init(void) {               
    *SYSCTL_RCGCGPIO_R |= 0x01;       // activate clock on Port A
    *GPIO_PORTA_DIR_R |= 0x7C;        // make PA6-2 pins output
    *GPIO_PORTA_DEN_R |= 0x7C;        // enable digital I/O on PA6-2
}

void PortB_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x02;       // activate clock on Port B
    *GPIO_PORTB_DIR_R &= ~0xC3;       // make PB1-0, PB7-6 pins input
    *GPIO_PORTB_DIR_R |= 0x3C;        // make PB5-2 output
    *GPIO_PORTB_DEN_R |= 0xFF;        // enable digital I/O on PB7-0
    *GPIO_PORTB_PDR_R |= 0xC3;        // enable pull-down resistor for PB1-0, PB7-6

    *GPIO_PORTB_IM    &= ~0x82;       // disable interrupts on pin 7,1
    *GPIO_PORTB_IS    &= ~0x82;       // make pin 7,1 edge triggered
    *GPIO_PORTB_IBE   &= ~0x82;       // don't trigger at both edges
    *GPIO_PORTB_IEV   |= 0x82;        // rising edge triggers
    GPIO_PORTB_ICR    |= 0x82;        // clear RIS and MIS registers
    *GPIO_PORTB_IM    |= 0x82;        // enable interrupts on pin 7,1

    NVIC_EN0_R        |= 0x02;        // enable port B interrupts (interrupt #1)
    NVIC_PRI0_R       = (NVIC_PRI0_R&0xFFFF1FFF)|(0x01 << 13); // set port B priority to 1
}

void PortF_Init(void) {               // initialize sw2
    *SYSCTL_RCGCGPIO_R |= 0x20;       // activate clock on Port F
    *GPIO_PORTF_LOCK_R =  0x4C4F434B; // unlock commit register
    *GPIO_PORTF_CR    |= 0x01;        // enable committing to pin 0 for DEN, PUR
    *GPIO_PORTF_DIR_R &= ~0x11;       // make PF4&0 input
    *GPIO_PORTF_DEN_R |= 0xFE;        // enable digital i/o on PF4
    *GPIO_PORTF_PUR_R |= 0x11;        // enable PUR on sw2 & sw1

    // The following registers configure PWM on pins PF1-3, which are mapped to M1PWM5-7, respectively
    *SYSCTL_RCGCPWM_R |= 0x02;        // Provide a clock to PWM module 1
    *GPIO_PORTF_AFSEL |= 0x0E;        // Choose an alternate function for pins PF3-1
    *GPIO_PORTF_PTCL  |= 0x5550;      // Sets alternate function to PWM signals for PF3-1
    *SYSCTL_RCC_R     |= 0x120000;    // Use PWM divider, set divider to divide sys clock by 4

    *PWM_PWMENABLE_R  &= ~0xE0;        // Disable PWM signals on Module 1, PWM 5-7
    *PWM_M1PWM2CTL_R  &= ~0x01;        // Disable PWM generators for M1, Block 2 (we only use Generator B)
    *PWM_M1PWM3CTL_R  &= ~0x01;        // Disable PWM generators for M1, Block 3 (A and B)

    *PWM_PWM2GENB_R   |= 0x8C;        // Toggle the output of PWM Module 1, Block 2, Generator B when down-count reaches zero
    *PWM_PWM3GENA_R   |= 0x8C;        // Toggle the output of PWM Module 1, Block 3, Generator A when down-count reaches zero
    *PWM_PWM3GENB_R   |= 0x80C;       // Toggle the output of PWM Module 1, Block 3, Generator B when down-count reaches zero
    *PWM_PWM2LOAD_R   |= 0x1F3;       // Set Module 1, Block 2 Load Counter to 499 to get a PWM period of 40 microseconds 
    *PWM_PWM3LOAD_R   |= 0x1F3;       // Set Module 1, Block 3 Load Counter to 499 to get a PWM period of 40 microseconds
    *PWM_PWM3COMPA_R  |= 0x1DA;       // Set Comp A to 449, for a 10% duty cycle
    *PWM_PWM2COMPA_R  |= 0x1DA;       // Set Comp A to 499, for a 10% duty cycle
    *PWM_PWM3COMPB_R  |= 0x1DA;       // Set Comp B to 449, for a 10% duty cycle

    *PWM_M1PWM2CTL_R  |= 0x01;        // Enable PWM generators for M1, Block 2 (we only use Generator B)
    *PWM_M1PWM3CTL_R  |= 0x01;        // Enable PWM generators for M1, Block 3 (A and B)
    *PWM_PWMENABLE_R  |= 0xE0;        // Enable PWM signals on Module 1, PWM 5-7
}

void PortD_Init(void) {
    *SYSCTL_RCGCGPIO_R |= 0x08;
    *GPIO_PORTD_LOCK_R = 0x4C4F434B;  // unlock commit register
    *GPIO_PORTD_DIR_R |= 0xCF;        // Set pins 0-3 & 6-7 as output
    *GPIO_PORTD_CR    |= 0x80;        // enable committing to pin 0 for DEN, PUR
    *GPIO_PORTB_DEN_R |= 0xCF;        // enable digital I/O on PB7-0
}

void gpioInit(void) {
    PortA_Init();
    PortB_Init();
    PortF_Init();
    PortD_Init();
}