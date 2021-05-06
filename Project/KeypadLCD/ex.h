#define F_CPU  16000000UL
#include <avr/io.h>
#include <util/delay.h>

void Delay_ms(unsigned int MilliSeconds){
	unsigned int i;
	
	for(i=0;i<MilliSeconds;i++) _delay_ms(1);
}

void Delay_us(unsigned int MicroSeconds){
	unsigned int i;
	
	for(i=0;i<MicroSeconds;i++) _delay_us(1);
}

#define  DELAY_TIME_1S 1000 //ms