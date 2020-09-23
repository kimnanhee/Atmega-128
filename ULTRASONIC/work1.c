/*
 * work1.c
 *
 * Created: 2020-09-23 오전 10:06:20
 *  Author: user
 
 전원을 인가하면 1초마다 초음파 센서의 값을 읽기
 출력에 해당되는 LED ON
 측정값을 PC로 전송
 
 LED : PORTB
 
 Ultrasonic
 trig : E6
 echo : E7
 
 Arduino
 RX : E0
 TX : E1
 */ 
#define TRIG 6
#define ECHO 7
#define SOUND_VELOCITY 340UL
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
void uart_string(char* str)
{
	while (*str)
	uart_write(*str++);
}

int main(void)
{
	DDRB = 0xFF; // LED 출력핀
	
	unsigned int distance;
	DDRE = ((DDRE | (1<<TRIG)) & ~(1<<ECHO));
	
	uart_init(BAUDRATE(9600));
	sei();
	while(1)
	{
		TCCR1B = 0x03;
		PORTE &= ~(1<<TRIG);
		_delay_us(10);
		PORTE |= (1<<TRIG);
		_delay_us(10);
		PORTE &= ~(1<<TRIG);
		while(!(PINE & (1<<ECHO)));
		TCNT1 = 0x0000;
		while(PINE & (1<<ECHO));
		TCCR1B = 0x00;
		distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000);

		//음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
		// 4/2 = 왕복거리
		if(distance <= 30) {
			PORTB = 0x01;
			uart_string("30mm down   ");
		}
		else if(distance <= 70) {
			PORTB = 0x02;
			uart_string("70mm down   ");
		}
		else if(distance <= 100) {
			PORTB = 0x04;
			uart_string("100mm down   ");
		}
		else if(distance <= 130) {
			PORTB = 0x08;
			uart_string("130mm down   ");
		}
		else if(distance <= 200) {
			PORTB = 0x10;
			uart_string("200mm down   ");
		}
		else {
			PORTB = 0x20;
			uart_string("200mm up   ");
		}
		
		_delay_ms(1000); // 1초 쉬기
	}
}