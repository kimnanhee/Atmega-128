/*
 * work1.c
 *
 * Created: 2020-09-16 오전 9:19:28
 *  Author: user
 
 3개의 스위치로 DC 모터 컨트롤하기
 1 - speed up     D0
 2 - speed down   D1
 3 - start/stop   D2
 
 모터
 B5, B6
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void DC_Motor(int value)
{
	OCR1A = value; // B5 : PWM으로 모터 제어
}

int main(void)
{
	int speed=0;
	int motor=0;
	
	DDRD = 0x00; // 스위치 연결 핀
	DDRE = 0xFF; // LED 연결 핀
	PORTE = 0x01;
	
	DDRB = 0xFF; // B포트에 모터를 연결
	TCCR1A=0x82;
	TCCR1B=0x1A;
	OCR1A=0; // B5
	OCR1B=0; // B6
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
			PORTE = (0x01 << (speed/20+2)); // LED 출력
		}
		else // motor stop
		{
			DC_Motor(0);
			PORTE=0x01 << 2; // LED 출력
		}
		_delay_ms(10);
	}
}
