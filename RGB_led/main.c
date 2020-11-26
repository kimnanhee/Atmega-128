/*
 * RGB_LED.c
 *
 * Created: 2020-11-26 오전 9:45:52
 * Author : DSM
 */ 
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

int state=0;
int arr[10], i;

void uart_init(unsigned int baud) // baudrate 속도 설정
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
}

int main(void)
{
    uart_init(BAUDRATE(9600));
	sei();
    while (1) 
    {
		
    }
}

ISR(USART0_RX_vect)
{
	unsigned char buff = UDR0; // UDR0에 레지스터에 데이터가 저장이 된다.
	if(buff == 'S') 
	{
		state = 1;
		i=0;
		memset(arr, 0, 10);
		break;
	}
	else if(buff == 'F') state = 0;
	
	if(state)
		arr[i++] = atoi(buff);
		
}