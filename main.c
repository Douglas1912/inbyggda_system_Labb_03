#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))

uint8_t buttonState = 0; 
int buttonPressed = 0;
int pressedConfidenceLevel = 0; //Measure button press cofidence
int releasedConfidenceLevel = 0; //Measure button release confidence


int main(void) {

	DDRD &= ~ (0 << PD2); //PD2 as input
	uart_init();

	while (1)
	{
		if (BIT_IS_SET(PIND, PD2)){
			pressedConfidenceLevel++; //Increase Pressed Conficence
			releasedConfidenceLevel = 0; //Reset released button confidence since there is a button press
			
			if (pressedConfidenceLevel > 500){ //Indicator of good button press
			
				if (buttonPressed == 0){
					printf_P(PSTR("pushed\r\n"));
					buttonPressed = 1;
					buttonState = 0;
				}
				//Zero it so a new pressed condition can be evaluated
				pressedConfidenceLevel = 0;
			}
		}
		
		else{
			releasedConfidenceLevel++; //This works just like the pressed
			pressedConfidenceLevel = 0; //Reset pressed button confidence since the button is released
			
			if (releasedConfidenceLevel > 500){
				buttonPressed = 0;
				releasedConfidenceLevel = 0;

				if(buttonState == 0){
					printf_P(PSTR("released\r\n"));
					buttonState = 1;
				}
			}
		}
	}
	return 0;
}
