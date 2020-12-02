/*
 * RGB_LED.c
 *
 * Created: 2020-11-26 오전 9:45:52
 * Author : DSM
 
 PB5 : RGB LED R
 PB6 : RGB LED G
 PB7 : RGB LED B
 */ 
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"

int state = 0, finish = 0;
char arr[10], i; // index

void pwm_init()
{
	// TCCR1n 레지스터의 설정은 OCR1A~C에 영향
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1) | (1 << WGM10); 
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	
	OCR1A = 0;
	OCR1B = 0;
	OCR1C = 100;
}
int toInt(char ch)
{
	return ch - '0';
}

int main(void)
{
	DDRB = 0xFF;
	pwm_init();
    uart_init(BAUDRATE(9600));
	sei(); // interrupt 활성화
	
    while (1) 
    {
		if(finish)
		{
			uart_string(arr);		
			int R = toInt(arr[0])*100 + toInt(arr[1])*10 + toInt(arr[2]);
			int G = toInt(arr[3])*100 + toInt(arr[4])*10 + toInt(arr[5]);
			int B = toInt(arr[6])*100 + toInt(arr[7])*10 + toInt(arr[8]);
			
			OCR1A = R;
			OCR1B = G;
			OCR1C = B;
			
			finish = 0;
		}
		_delay_ms(10);
    }
}

ISR(USART0_RX_vect)
{
	unsigned char buff = UDR0; // UDR0에 레지스터에 데이터가 저장이 된다.
	// S ___ ___ ___ F 
	// 문자열 포멧
	if(buff == 0x02)
	{
		state = 1;
		i=0;
		memset(arr, 0, 10);
		return;
	}
	else if(buff == 0x03)
	{
		finish = 1; // UART를 통해 문자열 입력 받기 완료
		state = 0;
	}
	
	if(state)
		arr[i++] = buff;
		
}