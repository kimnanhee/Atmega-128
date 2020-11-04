/*
 * Ball_sw.c
 * Created: 2020-11-04 오전 9:27:26
 * Author : DSM
 
 ball switch가 한번 접촉될 때마다 PB0 led on,
 ir sensor에 한번 인식될 때마다 PB1 led on.
 인식 상태를 uart로 보내줘서 PyQt에서 인식된 횟수를 확인한다.
 
 ball switch : PD4
 ir sensor : PD5
 */ 
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

int main(void)
{
	DDRD = 0x00; // ball switch
	DDRB = 0xFF; // LED
	PORTB = 0x00; // LED OFF
	uart_init(BAUDRATE(9600));
		
	int sw_state = 0, ir_state = 0; // 스위치, 적외선 인식 상태
	
	while (1) 
    {
		char key = PIND; // D포트 값 읽어오기
		if((key & 0x10) == 0x10) // ball switch
		{
			PORTB |= 0x01;
			sw_state = 1;
		}
		else sw_state = 0;
		if((key & 0x20) != 0x20) // ir sensor
		{
			PORTB |= 0x02;
			ir_state = 1;
		}
		else ir_state = 0;
		
		if(sw_state == 1 || ir_state == 1) // 하나라도 인식이 되어서 LED에 불이 켜졌을 경우
		{
			_delay_ms(250);
			PORTB = 0x00;
		}
		else _delay_ms(200);
		
		char buff[10];
		sprintf(buff, "%d", (sw_state*10 + ir_state)); // 시리얼 통신으로 보내주기
		uart_string(buff);
    }
	return 0;
}

