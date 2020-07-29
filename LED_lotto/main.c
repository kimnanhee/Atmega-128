/*
8개의 led를 빠르게 순차적으로 점등시키다가
버튼이 눌린 순간 켜져있는 led의 번호를 fnd에 출력한다.
5개의 번호를 다 맞추면 성공!!, 아닐시에는 반복한다.
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.h"

unsigned char seg[10] = // FND 0~9데이터
{0xC0, 0xF9, 0xA4, 0xB0, 0x99,
0x92, 0x82, 0xF8, 0x80, 0x90};

int num; // 랜덤으로 눌린 LED 번호
char print_data[16]; // LCD에 출력할 내용
int print_arr[5]={0, 0, 0, 0, 0}; // LED 번호 5개 저장
int i_num=0;

int main(void)
{
	DDRA = 0xFF; // LCD 데이터 핀
    DDRC = 0xFF; // LCD 제어 핀
	DDRE = 0xFF; // LED 연결 핀
	DDRB = 0xFF; // FND 연결 핀
	
	LCD_Init();
	
	unsigned char buff=0x01;
	num = 0;
	LCD_setcursor(0, 0);
	LCD_wString("Start lotto");
	_delay_ms(2000);
	
	sprintf(print_data, " - %d %d %d %d %d - ", 0, 0, 0, 0, 0);
	LCD_setcursor(0, 0);
	LCD_wString(print_data);
	
    while (1) 
    {
		PORTE = buff; // LED 출력
		if(buff==0x80) {
			buff=0x01;
			num=0;
		}
		else {
			buff=buff<<1;
			num++;
		}
		_delay_ms(250); // 0.25초 지연
		
		if((PIND & 0x01)==0x00)
		{
			PORTB = ~(seg[num]); // fnd에 LED 번호 출력
			
			print_arr[i_num++]=num; // LED 번호를 저장할 배열에 num값 저장
			
			sprintf(print_data, " - %d %d %d %d %d - ", print_arr[0], print_arr[1], print_arr[2], print_arr[3], print_arr[4]);
			LCD_setcursor(0, 0);
			LCD_wString(print_data);
			
			if(i_num>4)
			{
				if(print_arr[0]==print_arr[1] && print_arr[1]==print_arr[2] && print_arr[2]==print_arr[3] && print_arr[3]==print_arr[4]) // 5개를 다 맞췄을 때
				{
					LCD_setcursor(1, 0);
					LCD_wString("success");
				}
				else
				{
					LCD_setcursor(1, 0);
					LCD_wString("fail");
				}
				_delay_ms(2000);
				print_arr[0] = print_arr[1] = print_arr[2] = print_arr[3] = print_arr[4] = 0;
				sprintf(print_data, " - %d %d %d %d %d - ", print_arr[0], print_arr[1], print_arr[2], print_arr[3], print_arr[4]);
				LCD_setcursor(0, 0);
				LCD_wString(print_data);
				
				LCD_setcursor(1, 0);
				LCD_wString("          ");
				i_num=0;
			}
			_delay_ms(1000);
		}
    }
}
