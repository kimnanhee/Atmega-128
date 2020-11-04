#ifndef UART_H_
#define UART_H_


#define BAUDRATE(x) ((F_CPU/16/x)-1)

void uart_init(unsigned int baud) // baudrate 속도 설정
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
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