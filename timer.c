#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void timer_init() { 

    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

	
    TCCR0B |= (1 << CS01)|(1 << CS00);
    // set prescaler to 64 and starts PWM
}

void timer2_init(void) { 

	TCCR2A |= (1 << WGM21) ; 
	// set timer 2 for CTC mode
	
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
	
	OCR2A = 195;
	// initialize compare value
	
	TIMSK2 |= (1 << OCIE2A);
	
	sei();
	
   
}


