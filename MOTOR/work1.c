/*
 * work1.c
 *
 * Created: 2020-09-16 오전 9:19:28
 *  Author: user
 
 3개의 스위치로 DC 모터 컨트롤하기
 1 - speed up
 2 - speed down
 3 - start/stop
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void DC_Motor(int value)
{
	OCR1A = value;
}

int main(void)
{
	int speed=0;
	int motor=0;
	
	DDRD = 0x00;
	DDRE = 0xFF;
	PORTE = 0x01;
	
	DDRB = 0xFF;
	TCCR1A=0x82;
	TCCR1B=0x1A;
	OCR1A=0;
	OCR1B=0;
	ICR1=100;
	
	while(1)
	{
		unsigned char key;
		key = (PIND & 0x07); // 스위치 값 읽어오기
		switch(key)
		{
			case 0x03: // up
			speed+=20;
			if(speed >= 100) speed=100;
			_delay_ms(500);
			break;
			
			case 0x05: // down
			speed-=20;
			if(speed <= 0) speed=0;
			_delay_ms(500);
			break;
			
			case 0x06: // start/stop
			motor=!motor;
			_delay_ms(200);
			break;
		}
		if(motor) // motor start
		{
			DC_Motor(speed);
			PORTE = (0x01 << (speed/20+2));
		}
		else // motor stop
		{
			DC_Motor(0);
			PORTE=0x01 << 2;
		}
		_delay_ms(10);
	}
}
