/*
 * uart.h
 *
 * Created: 2020-12-02 오전 10:49:22
 *  Author: DSM
 */ 


#ifndef UART_H_
#define UART_H_

void uart_init(unsigned int baud) // baudrate 속도 설정
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // 7 bit : 인터럽트 활성화
}
void uart_write(unsigned char data) // uart로 데이터 전송
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void uart_string(char* str)
{
	while (*str)
	uart_write(*str++);
}


#endif /* UART_H_ */