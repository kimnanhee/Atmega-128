/*
미리 10자리 숫자를 저장한뒤에 4×4키패드로 lcd 첫째줄에 10자리 숫자 입력하고 A~D중 하나를 엔터로 설정했을때 
저장된 10자리 숫자랑 같으면 lcd 2번째줄에 corret 아니면 error를 출력하는 문제입니다.
*/
#include <avr/io.h>
#include <string.h>
#include "ex.h"
#include "lcd.h"

char KeySet(void);

int main(void)
{
	char message_correct[10] = "correct  " ,message_error[10] = "error    ";
	char input[11], output[11] = "1122334455";
	memset(input, 0, sizeof(input));
	int cur = 0;
	
	InitLCD();
	ClearLCD();
	GotoXY(0, 0);
	PutString("ID:");
	
    while (1) 
    {
		char key = KeySet();
		if('0' <= key && key <= '9')
		{
			if(cur<11)
			{
				input[cur++] = key;
				GotoXY(3, 0);
				PutString(input);
				Delay_ms(500);	
			}
		}
		else if(key == 'D')
		{
			GotoXY(0, 1);
			if(strcmp(input, output) == 0) PutString(message_correct);
			else PutString(message_error);
			Delay_ms(2000);
			
			ClearLCD();
			memset(input, 0, sizeof(input));
			cur = 0;
			GotoXY(0, 0);
			PutString("ID:");
		}
    }
}

char KeySet(void)
{
	char KeyBuff = 'n';
	DDRE = 0x00; PORTE=0xff; // 키패드 입력
	
	DDRE = 0x01; PORTE &= ~0x01;
	Delay_us(5);
	if(!(PINE&0x10)) KeyBuff = '1';
	if(!(PINE&0x20)) KeyBuff = '2';
	if(!(PINE&0x40)) KeyBuff = '3';
	if(!(PINE&0x80)) KeyBuff = 'A';
	DDRE=0x00; PORTE = 0xff;                   // 1 2 3 A 행 검사 끝
	
	DDRE = 0x02; PORTE &= ~0x02;
	Delay_us(5);
	if(!(PINE&0x10)) KeyBuff = '4';
	if(!(PINE&0x20)) KeyBuff = '5';
	if(!(PINE&0x40)) KeyBuff = '6';
	if(!(PINE&0x80)) KeyBuff = 'B';
	DDRE = 0x00; PORTE = 0xff;                 // 4 5 6 B 행 검사 끝
	
	DDRE = 0x04; PORTE &= ~0x04;
	Delay_us(5);
	if(!(PINE&0x10)) KeyBuff = '7';
	if(!(PINE&0x20)) KeyBuff = '8';
	if(!(PINE&0x40)) KeyBuff = '9';
	if(!(PINE&0x80)) KeyBuff = 'C';
	DDRE = 0x00; PORTE = 0xff;                 // 7 8 9 C 행 검사 끝
	
	DDRE = 0x08; PORTE &= ~0x08;
	Delay_us(5);
	if(!(PINE&0x10)) KeyBuff = '*';
	if(!(PINE&0x20)) KeyBuff = '0';
	if(!(PINE&0x40)) KeyBuff = '#';
	if(!(PINE&0x80)) KeyBuff = 'D';
	DDRE = 0x00; PORTE = 0xff;                 // * 0 # D 행 검사 끝
	
	return KeyBuff;
}