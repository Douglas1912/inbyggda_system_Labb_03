#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

volatile uint8_t ADC_val; //adc value

unsigned int ADC_read();
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
	ADMUX |= (1<<REFS0); //set reference voltage to 5v (AREF)
	ADMUX |= (1 << ADLAR); //left adjusted
	
	ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC also set Prescaler 8
	ADCSRA |= (1 << ADATE); // enable auto trigger
    ADCSRA |= (1 << ADSC);   // start conversion
	ADCSRA |= (1 << ADIE);   // enable interrupts 
}


unsigned int ADC_read()
{
    while (!(ADCSRA & (1 << ADIF)));   // wait for ADIF conversion complete return
    ADCSRA |= (1 << ADIF);   // clear ADIF when conversion complete by writing 1
    return (ADCH); //return calculated ADC value	
}

void set_PWM_Output(uint8_t duty)
{
   OCR0A=duty;
}

ISR(ADC_vect) { //Interrupt
    ADC_val = ADC_read();  // Save ADC value
	printf("%d\n", ADC_val);	
}

ISR(TIMER2_COMPA_vect)//Interrupt
{	
	set_PWM_Output(ADC_val);
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
