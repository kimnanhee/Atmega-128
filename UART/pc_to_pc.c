// pc���� ���� �����͸� �״�� pc�� �ٽ� �����ϱ�

#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)

#include <avr/io.h>
#include <util/delay.h>

void uart_init(unsigned int baud) // baudrate �ӵ� ���� 
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
}

void uart_write(unsigned char data) // uart�� ������ ���� 
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

unsigned char uart_read() // ������ �о���� 
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0; // UDR0�� �������Ϳ� �����Ͱ� ������ �ȴ�. 
}

int main(void)
{
	uart_init(BAUDRATE(9600));
	while(1)
	{
		unsigned char ch = uart_read(); // ������ �о���� 
		if ('a' <= ch && ch <= 'z')
			uart_write(ch - 'a' + 'A'); // �ҹ���->�빮�ڷ� ���� 
	}
}
