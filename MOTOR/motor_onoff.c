#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRF = 0xFF;
	
    while (1) 
    {	
		PORTF = 0x01; // forward
		_delay_ms(1000);
		
		PORTF = 0x03; // stop
		_delay_ms(1000);
		
		PORTF = 0x02; // backward
		_delay_ms(1000);
		
		PORTF = 0x03; // stop
		_delay_ms(1000);
    }
}
