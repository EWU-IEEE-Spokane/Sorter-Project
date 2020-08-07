#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "color_process.h"
#include "uart.h"

int determine_color(uint16_t r, uint16_t g, uint16_t b) {
	uint32_t sum = 0;
	uint32_t redPart = 0;
	uint32_t greenPart = 0;
	uint32_t bluePart = 0;
	int resultInt = 0;	//reject by default
	
	sum = r+g+b;

	//the following assignments give the protion of the total light recieved seen by each of the 3 channels (as a percentage)
	//the values are scaled to compensate for asymmetric color sensitivities as seen in the color sensor datasheet, page 6 figure 2
	//from figure 2, these values were obtained by calculating the inverse of the peak magnitude of each channel's color response
	redPart = (r*100)/sum;		//120 is color scaling modifier (originally 119)
	greenPart = (g*150)/sum;	//135 is color scaling modifier (originally 155)
	bluePart = (b*165)/sum;		//190 is color scaling modifier (originally 182)
	/**/
	/*
	uartwrite('h');
	uartwrite('e');
	uartwrite('x');
	uartwrite(':');
	uartwrite(' ');
	say_byte(redPart);		//print hex values
	say_byte(greenPart);
	say_byte(bluePart);
	new_line();
	*/
	say_rgb(redPart, greenPart, bluePart);

	char decision[16];		//result string 

	uint8_t colorVals[] = {redPart, greenPart, bluePart};
	colorSort(colorVals);	//sort in ascending order
	char colorChars[3]; 
	mapColorChars(colorChars, colorVals, redPart, greenPart, bluePart);
	float ratio = (float) 0.0;	//casting as float since compiler recognizes fractional constants as doubles - overflows 32 bit MCU
	int numerator = (colorVals[2]-colorVals[0]);
	int denominator = (colorVals[1]-colorVals[0]);
	if (denominator == 0) denominator = 1;	//avoid divide by 0
	ratio = numerator/denominator;

	if(ratio>(float)6.0){		//2.2 threshold
		resultInt = decideInf(decision, colorChars[2]);	//decide based on largest color value
	}else if(ratio>(float)1.3){		//threshold 1.3
		resultInt = decideMid(decision, colorChars[2],colorChars[0]);	//decide based on largest and smallest color value
	}else if(ratio>=(float)0){
		resultInt = decideOne(decision, colorChars[0]);	//decide based on smallest color value
	}else{
		strcpy(decision, "ratioCalcError");
		resultInt = 0;
	}
	say_string(decision, 16);
	return resultInt;
}


void mapColorChars(char* chars, uint8_t* colorVals, uint8_t r, uint8_t g, uint8_t b){
	for(int i=0; i<3; i++){
		chars[i] =  (colorVals[i]==r)? 'r':
					(colorVals[i]==g)? 'g':
					(colorVals[i]==b)? 'b':
					/*error inidcator*/'x';
	}
}

int decideInf(char* decision, char max){
	switch (max)
	{
	case 'r':
		strcpy(decision, "red");
		return 1;
		break;
	case 'g':
		strcpy(decision, "green");
		return 4;
		break;
	case 'b':
		strcpy(decision, "blue");
		return 0;
		break;
	default:
		strcpy(decision, "charMappingError");
		return 0;
		break;
	}
}

int decideMid(char* decision, char max, char min){
	if(max=='r'){
		if(min=='b'){
			strcpy(decision, "orange");
			return 2;
		}else{
			strcpy(decision, "magenta");
			return 0;
		}
	}else if(max=='g'){
		if(min=='b'){
			strcpy(decision, "yellow-green");
			return 0;
		}else{
			strcpy(decision, "blue-green");
			return 0;
		}
	}else if(max=='b'){
		if(min=='g'){
			strcpy(decision, "indigo");
			return 0;
		}else{
			strcpy(decision, "light blue");
			return 0;
		}
	}else{
		strcpy(decision, "charMappingError");
		return 0;
	}
}

int decideOne(char* decision, char min){
	switch (min)
	{
	case 'r':
		strcpy(decision, "teal");
		return 5;	//say violet for now
		break;
	case 'g':
		strcpy(decision, "violet");
		return 5;
		break;
	case 'b':
		strcpy(decision, "yellow");
		return 3;
		break;
	default:
		strcpy(decision, "charMappingError");
		return 0;
		break;
	}
}

void colorSort(uint8_t *array){   
    //just a bubble sort for now :/

    bool swapNeeded = true;
	int length = 3;
    while(swapNeeded){
        //run until everything is sorted, as indicated by no more swaps needed
        swapNeeded = false; //flag for element swap condition
        for(int i = 0; i<(length-1); i++){
            //walk through array indexes, if out of order, swap
            if(array[i]>array[i+1]){
                int temp = array[i+1];
                array[i+1] = array[i];
                array[i] = temp;
                swapNeeded = true;  //swap was needed, repeat while loop
            }
        }
    }


}

void say_rgb(uint8_t r, uint8_t g, uint8_t b){
	say_string("RGB: ", 5);
	uartwrite(hexToChar((r/16)%16));
	uartwrite(hexToChar((r)%16));
	uartwrite(hexToChar((g/16)%16));
	uartwrite(hexToChar((g)%16));
	uartwrite(hexToChar((b/16)%16));
	uartwrite(hexToChar((b)%16));
	new_line();
}

float process_values(uint16_t colorData, uint16_t whiteValue, uint16_t irValue){
	//return ((((colorData*0xFF))/(whiteValue*9)));	//worked moderately well, brown instead of red - this was a crude 1st try

	//ir compensation attempt, produced inconsistent results
	//if(colorData > irValue){
	//	colorData -= irValue;
	//}else{
	//	colorData = 0;
	//}
	return (((colorData*0xFF))/(whiteValue*6));	//attempted to have denom 0xFFFF, but this required gain correction of 7, which was the ratio of whiteValue to 0xFFFF
}

uint16_t applyGammaTable(float colorData){
	colorData /= 0xFF;
	colorData = colorData*colorData*(colorData/2);	//raise to 2.5th power
	colorData *= 0xFF;
	//return (uint16_t)(7*colorData);	//was working well for red values with gamma exponent of 2.2
	//return (uint16_t)(colorData/0x80);	//eliminating ir comp must've made base of gamma exponent greater than 1, since values exploded. attempting to scale
	return (uint16_t)colorData;	//no post gamma gain factors
}