/*
 * Ultrasonic.c
 *
 * Created: 2020-09-23 오전 9:50:07
 * Author : user
 
 ultrasonic
 trig : E6
 echo : E7
 
 buzzer
 +핀 : B0
 -핀 : GND
 */ 
#define TRIG 6
#define ECHO 7
#define SOUND_VELOCITY 340UL
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned int distance;
	unsigned int i;
	DDRB = 0xFF;
	DDRE = ((DDRE | (1<<TRIG)) & ~(1<<ECHO));
	
	while(1)
	{
		TCCR1B = 0x03;
		PORTE &= ~(1<<TRIG);
		_delay_us(10);
		PORTE |= (1<<TRIG);
		_delay_us(10);
		PORTE &= ~(1<<TRIG);
		while(!(PINE & (1<<ECHO)));
		TCNT1 = 0x0000;
		while(PINE & (1<<ECHO));
		TCCR1B = 0x00;
		distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000);

		//음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
		// 4/2 = 왕복거리
		if(distance < 300)
		{
			for(i=0; i<5; i++)
			{
				PORTB = 0x00;
				_delay_ms(1);
				PORTB = 0x01;
				_delay_ms(1);
			}
		} 
		else if(distance < 600)
		{
			for(i=0; i<5; i++)
			{
				PORTB = 0x00;
				_delay_ms(1);
				PORTB = 0x01;
				_delay_ms(1);
			}
			_delay_ms(100);
		}   
		else if(distance < 1000)
		{
			for(i=0; i<5; i++)
			{
				PORTB = 0x00;
				_delay_ms(1);
				PORTB = 0x01;
				_delay_ms(1);
			}
			_delay_ms(300);
		}
	}
}