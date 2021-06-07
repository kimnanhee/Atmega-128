#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

volatile unsigned char flag; // 블루투스 변수
char data[10];
int data_state = 0, motor_state = 0;

char rx_char(void)
{
	while((UCSR0A & 0x80) == 0);
	return UDR0;
}

void tx_char(char tx_char)
{
	while((UCSR0A & 0x20)==0);
	UDR0 = tx_char; // 송신받을 값
}

int main(void)
{	
	int i;
	DDRD = 0xFF; // motor data port
	
	char txt;
	UCSR0A = 0x00;
	UCSR0B = 0x98;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 103;
	DDRB=0xF0;
	DDRE = 0x08;
	
    while (1) 
    {
		txt = rx_char();
		if(txt == 0x02) data_state = 1;
		else if(txt == 0x03)
		{
			if(data[0] == 0x10) // rotate motor
			{
				
			}
			else if(data[1] == 0x11	) // stop motor
			{
				
			}
			memset(data, 0, sizeof(data));
			data_state = 0;
		}
		else
		{
			if(data_state) strcat(data, txt);
		}
		tx_char(txt);
		
		if(motor_state)
		{
			for(i=0; i<4; i++)
			{
				PORTD = (1 << i);
				_delay_ms(10);
			}
		}
    }
}