#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int timer_0 = 0, timer_1 = 0;
int cnt_0 = 0, cnt_1 = 0;

int main(void)
{
	DDRA = 0xFF; // led
	PORTA = 0x00;
	
	DDRB = 0xFF; // timer
	PORTB = 0x00;
	
	TCCR0 = 5; TCNT0 = 131; TIMSK = 1; // timer 0 setting
	TCCR1B = 3; TCNT1 = 53036; TIMSK |= 4; // timer 1 setting 
	
	sei(); // timer overflow enterrupt enble
	
    while (1) 
    {
		PORTA = ((0x01) << cnt_0) | (0x80 >> cnt_1);
		
		if(timer_0 >= 1000) // 1 * 1000 = 1000
		{
			cnt_0++;
			cnt_0 %= 4;
			timer_0 = 0;
		}
		if(timer_1 = 400) // 50 * 400 = 2000
		{
			cnt_1++;
			cnt_1 %= 4;
			timer_1 = 0;
		}
    }
}
ISR(TIMER0_OVF_vect)
{
	timer_0++;
}
ISR(TIMER1_OVF_vect)
{
	timer_1++;
}
