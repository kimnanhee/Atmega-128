/*
 * pwm_motor.c
 *
 * Created: 2020-09-09 오전 10:36:46
 *  Author: user
 
 모터 연결 : PB5, PB6
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void DC_Motor(int speed)
{
	if(speed< 0) speed= 0;
	if(speed>100) speed=100;
	OCR1A=speed;
}

int main(void)
{
	int i;
	DDRB=0xFF;
	
	TCCR1A=0x82;
	TCCR1B=0x1A;
	OCR1A=0;
	OCR1B=0;
	ICR1=100;
	while(1)
	{
		for(i=0;i<=100;i++) {
			DC_Motor(i); _delay_ms(100);
		}
		for(i=100;i>=0;i--){
			DC_Motor(i); _delay_ms(100);
		}
	}
}
