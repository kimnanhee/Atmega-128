#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char FND_SEGNP[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F}; // 0~9
unsigned char FND_SEGWP[10] = {0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0xA7,0xFF,0xEF}; // dot 0~9

unsigned char FND_SEGPOS[4] = {0x01,0x02,0x04,0x08};

void init_port()
{
   DDRA = 0xFF;
   DDRE = 0x0C;
   PORTE = 0x04;
   PORTA = 0x0F;
   DDRG = 0x00; // 스위치 연결 핀 G1~G3
}

void print_fnd(int num)
{
	if(num == 0)
	{
		int i;
		for(i=0; i<4; i++)
		{
			PORTA=FND_SEGPOS[i];
        	PORTE = 0x04;
        	PORTE = 0x00;

			PORTA = FND_SEGNP[0];
        	PORTE = 0x08;
        	PORTE = 0x00;
			_delay_us(2500);
		}
	}
	else
	{
		int i;
		for(i=0; i<4; i++)
		{
			PORTA=FND_SEGPOS[i];
        	PORTE = 0x04;
        	PORTE = 0x00;

			if(i==0) PORTA = FND_SEGNP[num/1000];
			else if(i==1) PORTA = FND_SEGWP[(num/100)%10]; // 점을 찍어서 정수 부분과 소수 부분 구분하기
			else if(i==2) PORTA = FND_SEGNP[(num/10)%10];
			else PORTA = FND_SEGNP[num%10];
        	PORTE = 0x08;
        	PORTE = 0x00;
			_delay_us(2500);
		}
	}
}

int main(void)
{
	init_port();
	char sw6 = 0x02, sw7 = 0x04, sw8 = 0x08;
	int state = 0; // 상태
	int num = 0; // 시간

	while(1)
	{
		if((PING & sw6) == 0x00) state = 1; // 숫자 증가
		if((PING & sw7) == 0x00) 
		{
			state = !state; // 숫자 증가 상태 변경
			while((PING & sw7) == 0x00);
		}
		if((PING & sw8) == 0x00)
		{
			num = 0; // 숫자 초기화
			state = 0; // 숫자 증가 정지
		}
		num += state; // 숫자가 증가 상태면 1을 더하고, 증가 정지 상태면 0을 더한다.
		print_fnd(num);
	}
}