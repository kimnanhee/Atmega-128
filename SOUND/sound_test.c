/*
 * sound.c
 *
 * Created: 2020-10-21 오전 10:00:24
 * Author : user
 
 F0 : 사운드 센서
 D0 : 스위치
 B0~B3 : LED
 
 사운드 센서와 스위치를 연결해서, 센서가 인식되면 LED에 2진수 형태로 출력
 스위치가 눌리면 LED RESET
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF; // led
	PORTB = 0x00;
	DDRF = 0x00; // sound sensor
	DDRD = 0x00; // sw
	
	int cnt=0;
	while (1) 
    {
		char key = PINF & 0x01;
		char sw = PIND & 0x01;
		 
		if(sw == 0x00) // 버튼이 눌렸을 때
		{
			cnt=0;
			_delay_ms(100);
		}
		
		if(key == 0x00) // 소리가 감지되었을 떄
		{
			cnt++;
			if(cnt>15) cnt=0;
			_delay_ms(50);
		}
		PORTB = cnt;
    }
}

