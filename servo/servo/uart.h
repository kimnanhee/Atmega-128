#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart0_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1<<RXCIE0);
	sei();
} // 8bit, no parity, 1 stop bit, TX enable, RX ISR enable

void uart0_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	// wait for sending
	UDR0 = data; // send
}

void uart0_string(char* str)
{
	while (*str)
	uart0_write(*str++);
}

void uart1_init(unsigned int baud)
{
	UBRR1H = (unsigned char)(baud >> 8);
	UBRR1L = (unsigned char)baud;
	UCSR1B = (1 << TXEN1) | (1 << RXEN1) | (1<<RXCIE1);
} // 8bit, no parity, 1 stop bit, TX enable, RX ISR enable

void uart1_write(unsigned char data)
{
	while (!(UCSR1A & (1 << UDRE1)));
	// wait for sending
	UDR1 = data; // send
}

void uart1_string(char* str)
{
	while (*str)
	uart1_write(*str++);
}

#endif /* UART_H_ */