// ���ͷ�Ʈ�� ����ؼ� pc���� ���� �����͸� �״�� pc�� �ٽ� �����ϱ�

#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_init(unsigned int baud) // baudrate �ӵ� ���� 
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}

void uart_write(unsigned char data) // uart�� ������ ����
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
	unsigned char buff = UDR0; // UDR0�� �������Ϳ� �����Ͱ� ������ �ȴ�. 
	uart_write(buff - 'a' + 'A'); // �ҹ���->�빮�ڷ� ���� 
}
