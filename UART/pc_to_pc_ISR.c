// 인터럽트를 사용해서 pc에서 받은 데이터를 그대로 pc로 다시 전송하기

#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_init(unsigned int baud) // baudrate 속도 설정 
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}

void uart_write(unsigned char data) // uart로 데이터 전송
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

int main(void)
{
	uart_init(BAUDRATE(9600));
	sei();
	while(1);
}

ISR(USART0_RX_vect)
{
	unsigned char buff = UDR0; // UDR0에 레지스터에 데이터가 저장이 된다. 
	uart_write(buff - 'a' + 'A'); // 소문자->대문자로 전송 
}
