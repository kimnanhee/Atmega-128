/*
A : LCD data
C : LCD control
PB5 : servo
PD0 : DHT11
PD1 : sw
PE0~2 : LED
PF0 : VR
*/
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU/16/x)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "sensor.h"
#include "servo.h"

int state = 0;

int main(void)
{
	DDRA = 0xFF; // LCD data
	DDRC = 0xFF; // LCD control
	DDRB = 0xFF; // servo
	DDRD = 0x00; // DHT11, SW
	DDRE = 0x00; // LED
	DDRF = 0x00; // VR
	
	LCD_Init();
	SERVO_init();
	
	EICRA |= (1 << ISC11) | (1 << ISC10);
	EIMSK |= 1 << INT1;
	sei();
	
	char buff[16];
	
    while (1) 
    {
		
		float temp = temp_sensor_read();
		_delay_ms(3000);
		int humi = humi_sensor_read();
		_delay_ms(3000);
		LCD_setcursor(1, 0);
		sprintf(buff, "%d.%d'C  %d%%  ", (int)temp, (int)(temp*10)%10, humi);
		LCD_wString(buff);
		
		if(temp >= 30) PORTE = (0x01 << 0);
		else if(temp >= 10) PORTE = (0x01 << 1);
		else PORTE = (0x01 << 2);
		
		ADC_set();
		int value = ADC_read(0);
		int set_value = 0 + (int)((value*30)/1024);
		LCD_setcursor(0, 0);
		sprintf(buff, "set temp : %d'C  ", set_value);
		LCD_wString(buff);	
		
		if(temp >= set_value) 
		{
			state = 1;
			SERVO_play(state);
		}
		else
		{
			state = 0;
			SERVO_play(state);
		}
    }
}

ISR(INT1_vect)
{
	state = !state;
	SERVO_play(state);
}