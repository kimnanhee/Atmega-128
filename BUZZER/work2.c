/*
 * work2.c
 *
 * Created: 2020-10-06 오후 9:51:47
 *  Author: user
 */ 
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DO 0
#define RE 1
#define MI 2
#define FA 3
#define SOL 4
#define RA 5
#define SI 6
#define DDO 7
#define REST 8

#define EOS -1 // End Of Song 표시
#define ON 0 // 버저 ON
#define OFF 1 // 버저 OFF

#define N2 1250 // 2분 음표 연주 시간, 60*1000/96*2 (ms)
#define N02 (N2/2*3) // 점 2분 음표
#define N4 (N2/2) // 4분 음표 연주 시간, 60*1000/96 (ms)
#define N8 (N2/4) // 8분 음표 연주 시간, 60*1000/96/2 (ms)
#define N16 (N2/8) // 16분 음표 연주 시간, 60*1000/96/4 (ms)
#define N8N16 (N8+N16) // 점 8분 음표 연주 시간, N8+N16 (ms)
#define R 1 // 묵음 연주 시간, 1 (ms)

volatile int state, tone;

char f_table[] = {17, 43, 66, 77, 97, 114, 129, 137, 255}; // 도레미파솔라시도에 해당하는 TCNT0 값을 미리 계산한 값

int song[3][40] = { // 산토끼, 작은 별, 학교종 음계
	{DO,FA,SOL,RA,SOL,FA,SOL,RA,FA,SOL,FA,RE,DO,REST, DO,FA,SOL,RA,SOL,FA,SOL,RA,FA,SOL,RE,MI,FA,EOS},
	{DO,DO,SOL,SOL,RA,RA,SOL, FA,FA,MI,MI,RE,RE,DO,EOS},
	{SOL,SOL,RA,RA,SOL,SOL,MI, SOL,SOL,MI,MI,RE,EOS}}; 
		
int time[3][40] = { // 산토끼, 작은 별, 학교종 박자
	{N8,N8,N8,N8,N8,N8,N2,N4,N8,N8,N8,N8,N2,R, N8,N8,N8,N8,N8,N8,N2,N4,N8,N8,N8,N8,N2},
	{N4,N4,N4,N4,N4,N4,N2, N4,N4,N4,N4,N4,N4,N2},
	{N4,N4,N4,N4,N4,N4,N2, N4,N4,N4,N4,N02}}; 

char LED[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00}; //LED On Bits

ISR(TIMER0_OVF_vect) // Timer/Counter0 오버플로우 인터럽트 서비스 루틴
{
	TCNT0 = f_table[tone]; // TCNT0 초기화 (주어진 음을 만들기 위한)
	if (state == OFF) {
		PORTB |= 1 << 4; // 버저 포트 ON
		state = ON;
	}
	else {
		PORTB &= ~(1 << 4); // 버저 포트 OFF
		state = OFF;
	}
}

int main()
{
	int i=0;
	int mode = 0;
	DDRA = 0xff; // LED ON 출력
	DDRB |= 0x10; // 버저 연결 포트(PB4) 출력 설정
	DDRC = 0x00; // SW 입력
	TCCR0 = 0x03; // 프리스케일러 32분주
	TIMSK = 0x01; // 오버플로우 인터럽트 활성화, TOIE0 비트 세트
	TCNT0 = f_table[song[0][i]]; // TCNT0 초기화
	sei(); // 전역인터럽트 활성화

	while(1)
	{
		unsigned char key = (PINC & 0x07); // 3개의 스위치 중에 눌린 번호의 노래 재생
		mode = 0;
		
		if(key == 0x01) mode=1;
		else if(key == 0x02) mode=2;
		else if(key == 0x04) mode=3;
		
		if(mode != 0)
		{
			i=0; // 노래 반복 초기화
			while(song[mode-1][i] != EOS) // 노래 마지막 음인지 검사, 초판본 오류
			{
				tone = song[mode-1][i]; // 현재 음계
				PORTA = LED[tone]; // LED 표시
				_delay_ms(time[mode-1][i++]); // 한 계명당 지속 시간
			}
		}
		
	}
}
