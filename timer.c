#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void timer_init() { 

    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode
	
    TCCR0B |= (1 << CS01)|(1 << CS00);
    // set prescaler to 64 and starts PWM, 255 0xFF as TOP
}

void timer2_init(void) { 

	TCCR2A |= (1 << WGM21); // set timer 2 for CTC mode (TOP = OCR2A)
	
	//TCCR2A |= (1 << COM2A0); // Clear output pin OC2A on compare match PIN11 Arduino Uno
	
	TCCR2B |= ((1 << CS22) | (1 << CS21) | (1 << CS20));
    // set prescaler to 1024 
	//The clock frequency becomes 16000000 / 1024 = 15625 Hz
	//Time for each Tick is 1 / 15625 = 0.000064 sec = 0,064ms
	//For 16 msec to elapse the count is 16 / 0,064 = 250
	//			**Faster calculation formula***
	//Divide CPU Frequency though chosen prescaler (MAX PS = 1024): 
    //16MHz/1024 = 15625Hz
	//Divide the Result though inverse of the desired milli-frequency (16ms): 
	//15625/((1/16)*1000) = 250 (OCR2A = 250)
	
	TIMSK2 |= (1 << OCIE2A);  // Enable Timer 2 Output Compare Match Interrupt for channel A
	TIFR2 |= (1 << TOV2) | (1<<OCF2A); //clear pending interrupt flag for channel A
 
	
	OCR2A = 156; // Set Output Compare Value to compare match register 16MHz/1024/100Hz
	
	TCNT2  = 0; // initialize counter

	sei(); // Enable interrupts globally
	
   
}


