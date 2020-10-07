/*
 * DHT.c
 *
 * Created: 2020-10-07 오전 9:03:08
 * Author : user
 
PA0~PA7 : LCD 데이터
PC0~PC2 : LCD 제어
PD6 : DHT11 데이터
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#define DHT11_PIN 6

uint8_t c=0, I_RH, D_RH, I_Temp, D_Temp, CheckSum;
void Request()				// ATMega128로 스타트 펄스 전달 & 응답 과정
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	// PD6 LOW
	_delay_ms(20);
	PORTD |= (1<<DHT11_PIN);	// PD6 HIGH
}
void Response()				// 온습도 센서로부터 응답
{
	DDRD &= ~(1<<DHT11_PIN); // PD4 LOW
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data()
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0); //비트가 0인지 1인지 체크
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN)) //HIGH가 30ms보다 크면
		c = (c<<1)|(0x01);	 //HIGH 상태
		else
		c = (c<<1); //LOW 상태
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}

int main(void)
{
	DDRA = 0xFF; // LCD 데이터 핀
	DDRC = 0xFF; // LCD 제어 핀
	
	LCD_Init();
	char buff[16];
	
    while (1) 
    {	
		Request();		 //시작 펄스 신호 보냄
		Response();		 //센서로부터 응답 받음
		
		I_RH=Receive_data();	 //습도의 정수 부분
		D_RH=Receive_data();	 //습도의 실수 부분
		I_Temp=Receive_data();	 //온도의 정수 부분
		D_Temp=Receive_data();	 //온도의 실수 부분
		CheckSum=Receive_data(); //모든 세그먼트의 체크섬
		
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			LCD_setcursor(0,0);
			LCD_wString("Error");
		}
		else
		{
			LCD_setcursor(0, 0);
			sprintf(buff, "temp : %d.%d C   ", I_Temp, D_Temp); // 온도
			LCD_wString(buff);
			
			LCD_setcursor(1, 0);
			sprintf(buff, "humi : %d.%d %%  ", I_RH, D_RH); // 습도
			LCD_wString(buff);
		}
		_delay_ms(3000); // 2초 이상 쉬어야 센서에서 맞는 값을 읽어올 수 있다
    }
}

