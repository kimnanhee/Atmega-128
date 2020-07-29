/*
8���� led�� ������ ���������� �����Ű�ٰ�
��ư�� ���� ���� �����ִ� led�� ��ȣ�� fnd�� ����Ѵ�.
5���� ��ȣ�� �� ���߸� ����!!, �ƴҽÿ��� �ݺ��Ѵ�.
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.h"

unsigned char seg[10] = // FND 0~9������
{0xC0, 0xF9, 0xA4, 0xB0, 0x99,
0x92, 0x82, 0xF8, 0x80, 0x90};

int num; // �������� ���� LED ��ȣ
char print_data[16]; // LCD�� ����� ����
int print_arr[5]={0, 0, 0, 0, 0}; // LED ��ȣ 5�� ����
int i_num=0;

int main(void)
{
	DDRA = 0xFF; // LCD ������ ��
    DDRC = 0xFF; // LCD ���� ��
	DDRE = 0xFF; // LED ���� ��
	DDRB = 0xFF; // FND ���� ��
	
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
		PORTE = buff; // LED ���
		if(buff==0x80) {
			buff=0x01;
			num=0;
		}
		else {
			buff=buff<<1;
			num++;
		}
		_delay_ms(250); // 0.25�� ����
		
		if((PIND & 0x01)==0x00)
		{
			PORTB = ~(seg[num]); // fnd�� LED ��ȣ ���
			
			print_arr[i_num++]=num; // LED ��ȣ�� ������ �迭�� num�� ����
			
			sprintf(print_data, " - %d %d %d %d %d - ", print_arr[0], print_arr[1], print_arr[2], print_arr[3], print_arr[4]);
			LCD_setcursor(0, 0);
			LCD_wString(print_data);
			
			if(i_num>4)
			{
				if(print_arr[0]==print_arr[1] && print_arr[1]==print_arr[2] && print_arr[2]==print_arr[3] && print_arr[3]==print_arr[4]) // 5���� �� ������ ��
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
