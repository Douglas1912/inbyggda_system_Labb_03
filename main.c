#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

int readValueADC = 0; // define an integer to save adc read value
uint8_t duty = 0;

unsigned int ADC_read(unsigned char chnl);
void init_ADC();
void set_PWM_Output(uint8_t duty);


int main(void)
{
	uart_init();
	timer_init();
	timer2_init();
	LED_init();
	init_ADC();

	while (1)
	{

	}
	return 0;
}


void init_ADC()
{
	ADMUX = (1 << REFS0); // Selecting internal reference voltage For Aref=AVcc
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC also set Prescaler as 128

}


unsigned int ADC_read(unsigned char chnl)
{
	ADCSRA=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0); //Enable ADC with Prescalar=Fcpu/128 

	chnl = chnl & 0b00000111; // select adc channel between 0 to 7
	ADMUX = 0x40;        //channel A0 selected
	ADMUX |= (1<<REFS0)|(1<<REFS1); //set reference voltage to 5v (AREF)
	ADCSRA |= (1 << ADSC);   // start conversion
	while (!(ADCSRA & (1 << ADIF)));   // wait for ADIF conversion complete return
	ADCSRA |= (1 << ADIF);   // clear ADIF when conversion complete by writing 1
	return (ADC); //return calculated ADC value
}

void set_PWM_Output(uint8_t duty)
{
   OCR0A=duty;
}

ISR(TIMER2_COMPA_vect)
{
	
	printf("%d\n", readValueADC);

	readValueADC = ADC_read(0);   //save adc read value in integer
	
	if (readValueADC >= 256)
	{
		duty = 255;
		set_PWM_Output(duty);
	}

	else if (readValueADC <= 255)
	{
		duty = readValueADC;
		set_PWM_Output(duty);

	}
}













//Delluppgift 2
/*
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
}*/
