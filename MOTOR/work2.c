/*
 * work2.c
 *
 * Created: 2020-09-16 오전 10:09:41
 *  Author: user
 */ 
#define F_CPU 16000000
#define BAUDRATE(x) ((F_CPU/16/x)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}
void uart_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

unsigned char uart_read()
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uart_string(char* str)
{
	while (*str)
		uart_write(*str++);	
}

void DC_Motor(int value)
{
	OCR1A = value;
}

int i=0, state=0, finish=0;
char arr[10]={0};

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
	PORTB = 0;
	uart_init(BAUDRATE(9600));
	sei();
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
		
		if(finish)
		{
			if(!strcmp(arr, "0")) speed=0;
			else if(!strcmp(arr, "20")) speed=20;
			else if(!strcmp(arr, "40")) speed=40;
			else if(!strcmp(arr, "60")) speed=60;
			else if(!strcmp(arr, "80")) speed=80;
			else if(!strcmp(arr, "100")) speed=100;
			i=0;
			uart_string(arr);
			memset(arr, 0, 10);
			
			finish=0;
		}
		if(motor) // motor start
		{
			DC_Motor(speed);
			PORTE = (0x01 << (speed/20+2));
			uart_write(speed/20+'0');
			uart_write('s');
			uart_write(' ');
		}
		else // motor stop
		{
			DC_Motor(0);
			PORTE=0x01 << 2;
		}
		_delay_ms(100);
	}
}

ISR(USART0_RX_vect) // uart로 받은 데이터가 있을 때 실행되는 함수
{
	unsigned char buff = UDR0;
	
	if(buff == 0x02) 
	{state=1; return;}
	else if(buff == 0x03) state=0, finish=1;
	
	if(state) arr[i++]=buff;
};