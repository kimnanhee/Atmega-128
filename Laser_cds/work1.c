/*
 * laser.c
 *
 * Created: 2020-11-11 오전 9:11:02
 * Author : DSM
 
 회로에 전원이 인가되면 Green led ON
 3초후 레이저 전원이 켜지고 Blue led ON, 
 장애물이 감지되면 Red led ON
 
  PF0 : cds
  PD4 : laser module sensor pin
  PD5 : RGB led B
  PD6 : RGB led G
  PD7 : RGB led R
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void init_adc()
{
	ADMUX = 0x40; // 기준 전압 선택, ADC0
	ADCSRA = 0x87; // ADC동작 허용, 128분주비 설정
}
unsigned short read_adc()
{
	unsigned char low, high;
	unsigned short value;
	
	ADCSRA |= 0x40;
	while((ADCSRA & 0x10) != 0x10); // ADc변환이 될 때까지 기다리기
	
	low = ADCL;
	high = ADCH;
	value = (high << 5) | low;
	
	return value;
}

int main(void)
{
	DDRD = 0xFF;
	
	PORTD = 0x40; // Green led on
	_delay_ms(3000);
	PORTD = 0x10; // laser on
	PORTD |= 0x80; // Blue led on
	
    init_adc();
    while (1) 
    {
		PORTD = 0x10;
		int value = read_adc();
		if(value < 50) // 장애물이 감지 되었을 경우
		{
			PORTD |= 0x80; // Red led on
		}
		else
		{
			PORTD |= 0x20; // Blue led on
		}
		_delay_ms(100);
    }
}