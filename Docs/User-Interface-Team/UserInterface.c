#include <stdbool.h>
#include <stdint.h>

//This code sets up a basic state machine. Using an interrupt, two buttons and a RGB LED, the user can control the color of the LED.
//Interrupt handler is at the end of the startup file. Important lines in startup file are 57 and 58 and after 365.

//Variables
int flag;//turned on when interrupt is hit. turned off once next state is defined
int current_state;//state register
int next_state;//next state

void clock_50Mhz() {
	
    unsigned long *rcc =(unsigned long*)(0x400FE000+0x60);
    unsigned long *rcc2 =(unsigned long*)(0x400FE000+0x70);
    unsigned long *ris =(unsigned long*)(0x400FE000+0x50);
    unsigned long *rcgcgpio =(unsigned long*)(0x400FE000+0x608);

    *rcc2 = *rcc2 | (1<<31);            //USERCC2
    *rcc2 = *rcc2 | (1<<11);            //BYPASS
    *rcc = *rcc &(~(1<<22));            //CLEAR USESYSDIV
    *rcc = *rcc &(~(0x1F<<6));          //CLEAR XTAL
    *rcc = *rcc | (0X15<<6);            //XTAL
    *rcc2 = *rcc2 &(~(0x7<<4));         //CLEAR OSCSRC
    *rcc2 = *rcc2 &(~(0x1<<13));        //CLEAR PWRDN
    *rcc2 = *rcc2 &(~(0x3F<<23));       //CLEAR SYSDIV2
    *rcc2 = *rcc2 | (0x3<<23);          //SYSDIV2
    *rcc = *rcc | (0x1<<22);            //USESYSDIV

    while(!(*ris&(0x1<<6)))               //POLL PLLRIS
        continue;

    *rcc2 = *rcc2 &(~(0x1<<11));        //CLEAR BYPASS

    *rcgcgpio = *rcgcgpio | (0x5<<3); //make sure to attach to the ports you are using (F and D)

}

void configure() {

    //Port D. Set up for switch inputs PD0 and PD1
    unsigned long *gpiolockD   = (unsigned long *) (0x40007000 + 0x520);
    unsigned long *gpiocrD     = (unsigned long *) (0x40007000 + 0x524);
    unsigned long *gpiopurD    = (unsigned long *) (0x40007000 + 0x510); 
    
    unsigned long *gpiodirD    = (unsigned long*)(0x40007000 + 0x400);    
    unsigned long *gpioafselD  = (unsigned long*)(0x40007000 + 0x420);  
    unsigned long *gpiodenD    = (unsigned long*)(0x40007000 + 0x51c);    
    
    //Port F. Set up for LEDs, PF1, PF2, PF3. Set up interrupt, PF0, PF4 (redundant here)
    unsigned long *gpiolock   = (unsigned long *) (0x40025000 + 0x520);
    unsigned long *gpiocr     = (unsigned long *) (0x40025000 + 0x524);
    unsigned long *gpiopur    = (unsigned long *) (0x40025000 + 0x510); //pull up resistor
    
    unsigned long *gpiodir    = (unsigned long *) (0x40025000+0x400);    //adress for all F direction
    unsigned long *gpioafsel  = (unsigned long *) (0x40025000+0x420);  //adress for all F alternate functions
    unsigned long *gpioden    = (unsigned long *) (0x40025000+0x51c);    //adress for all F enable
    
    unsigned long *gpiois     = (unsigned long *) (0x40025000 + 0x404); //edge/level sensitivity
    unsigned long *gpioibe    = (unsigned long *) (0x40025000 + 0x408); //set to 0 to be sensitive to one edge
    unsigned long *gpioiev    = (unsigned long *) (0x40025000 + 0x40c); //selects rising or falling edge
    unsigned long *gpioris    = (unsigned long *) (0x40025000 + 0x414);
    unsigned long *gpioim     = (unsigned long *) (0x40025000 + 0x410); //interrupt control bit 6
    unsigned long *nvicpri7   = (unsigned long *) (0xE000E000 + 0x41c); //interrupt priority
    unsigned long *nvicen0    = (unsigned long *) (0xE000E000 + 0x100); //enable interrupt

    //switch setup Port F
    
    *gpiolock   = 0x4C4F434B;
    *gpiocr     = 0x11;
    *gpiopur    = 0x11;

    //Port F setup

    *gpioafsel  = 0x00;
    *gpiodir    = 0xE; //sets up the output PF1, PF2, PF3
    *gpiodir    = (*gpiodir & ~(0x11)); //sets up input PF0 and PF4                                                                                                   //turns alternate functions off
    *gpioden    = 0xFF;

    //switch setup Port D for pins 0 and 1 as switch inputs
    
    *gpiolockD   = 0x4C4F434B; //unlocks switch
    *gpiocrD     = 0x3;
    *gpiopurD    = 0x3; //pull up resistor

    //Port D setup for pins 0 and 1 as switch inputs

    *gpioafselD  = 0x00;
    *gpiodirD    = (*gpiodirD & ~(0x3)); //switch inputs PD0 and PD1                                                                                                   //turns alternate functions off
    *gpiodenD    = 0xFF;                                                                       

    //interrupt controls

    *gpioim     = 0x00;
    *gpiois     &= ~(0x11); //sets to edge sensitivity
    
    *gpioibe    = 0x00; //sesnitive to one edge
    *gpioiev    |= 0xFF; //sets which edge
    *gpioris    = 0x00;

    *nvicpri7   = (1 << 21); //priority 7
    *nvicen0    = (1 << 30); //interrupt #30 enable
    *gpioim     = 0x11;

    //initialize variables
    
    flag                = 0;
    current_state       = 0;
    next_state          = 0;

}

// Defining the states. Each state, for the time being, only outputs a solid color. Can create more states here

//yellow LED shown (green and blue at the same time) PF2 and PF3. when current_state = 0
void standby(){

    unsigned long *gpiodata = (unsigned long*)(0x40025000+0x000+(0x1F<<2));

    //output of state
    while(flag==0){
        
        *gpiodata = 0xB; //color defined here  
    
    }
}
//Green LED shown PF3. when current_state = 1
void green_state(){

    unsigned long *gpiodata = (unsigned long*)(0x40025000+0x000+(0x1F<<2)); 
    
    //output of state
    while(flag==0){
        
        *gpiodata = 0x08; //color defined here 
    
    }
}
// LED Blue PF2. when current_state = 2
void blink_state(){

    unsigned long *gpiodata = (unsigned long*)(0x40025000+0x000+(0x1F<<2)); 
    
    //output of sate
    while(flag==0){
        
        *gpiodata = 0x04; //color defined here 
    }
}
//Other State. Red LED. when current_state = 3
void other_state(){

    unsigned long *gpiodata = (unsigned long*)(0x40025000+0x000+(0x1F<<2)); 
    
    //output of sate
    while(flag==0){

        *gpiodata = 0x02; //color defined here  

    }
}

//used to kind of debounce in handler
void delay (unsigned long d){                       

    for(int i=0; i < d; i++){}

}

//based on current_state, decides which state to execute. Can add as many states as desired
void Decide_Current(){

    if(current_state == 0){
        
        standby();
    }

    else if(current_state == 1){
        
        green_state();
    
    }

    else if(current_state == 2){

        blink_state();

    }

    else if(current_state == 3){

        other_state();

    }

}

//Based on inputs, decide what the next state is. This will be where most of the modifications happen later
void Decide_Next(){

    unsigned long *gpiodataD = (unsigned long*)(0x40007000+0x000+(0x1F<<2));

    if(!(*gpiodataD & 0x03)){
		//Both switches are pressed
		next_state = 1;	// green_state
	}else if(!(*gpiodataD & 0x01)){
		//SW 1 is pressed, SW 2 is not
		next_state = 2;	//blink_state
	}else if(!(*gpiodataD & 0x02)){
		//SW 2 is pressed, SW 1 is not
		next_state = 3;	//other_state
	}else{
		next_state = 0;
	}

}
//when flag is 0, Decide the current state. when flag is 1 set current state, set flag back to zero and go back into loop
void loop(){
    
    unsigned long *gpioim   = (unsigned long *) (0x40025000 + 0x410);

    if(flag==0){

        Decide_Current();

    }
        
    if(flag==1){
            
            Decide_Next();
            current_state=next_state; //set state register to next state
            *gpioim = 0x00; //pauses interupts
            *gpioim = 0x11;
            flag=0;    
            loop();//go back into loop
        }   
    
}

int main(){
    
    clock_50Mhz();
    
    configure();

    loop();
    
    
}