#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DATA_PORT         PORTC
#define LCD_DATA_PORT_DDR      DDRC
#define LCD_CONTROL_PORT      PORTB
#define LCD_CONTROL_PORT_DDR   DDRB
#define LCD_E               7
#define LCD_RW               6
#define LCD_RS               5
#define BACK_LIGHT            4

void EnablePulse(void) {
	
	LCD_CONTROL_PORT &= ~(1<<LCD_E);
	LCD_CONTROL_PORT |= 1<<LCD_E;
	_delay_us(40);
	LCD_CONTROL_PORT &= ~(1<<LCD_E);
}

void FunctionSet(void) {
	
	LCD_CONTROL_PORT &= ~((1<<LCD_RW) | (1<<LCD_RS));
	
	LCD_DATA_PORT = 0x38;
	EnablePulse();
}

void ClearLCD(void) {
	
	LCD_CONTROL_PORT &= ~((1<<LCD_RW) | (1<<LCD_RS));
	
	LCD_DATA_PORT = 0x01;
	EnablePulse();
	_delay_ms(1.5);
}

void Cursor(char CursorMode, char BlinkingMode) {
	unsigned char Tmp;
	
	LCD_CONTROL_PORT &= ~((1<<LCD_RW) | (1<<LCD_RS));
	
	Tmp = (CursorMode<<1) | BlinkingMode;
	LCD_DATA_PORT = Tmp | 0x0C;
	EnablePulse();
	_delay_ms(1.5);
}

void InitLCD(void) {
	
	LCD_DATA_PORT_DDR = 0xFF;
	LCD_CONTROL_PORT_DDR = (1<<LCD_E) | (1<<LCD_RW) | (1<<LCD_RS);
	
	LCD_CONTROL_PORT &= ~((1<<LCD_RW) | (1<<LCD_RS));
	
	_delay_ms(15);
	
	LCD_DATA_PORT = 0x30;
	EnablePulse();
	
	_delay_ms(5);
	
	LCD_DATA_PORT = 0x30;
	EnablePulse();
	
	_delay_ms(0.1);
	
	LCD_DATA_PORT = 0x30;
	EnablePulse();
	
	_delay_ms(1.5);
	
	LCD_DATA_PORT = 0x38;
	EnablePulse();
	
	LCD_DATA_PORT = 0x08;
	EnablePulse();
	
	LCD_DATA_PORT = 0x06;
	EnablePulse();
	
	LCD_DATA_PORT = 0x0C;
	EnablePulse();
}

void GotoXY(unsigned char X, unsigned char Y) {
	
	LCD_CONTROL_PORT &= ~((1<<LCD_RW) | (1<<LCD_RS));
	
	LCD_DATA_PORT = 0X40*Y+X+0X80;
	EnablePulse();
}

void PutCharacter(unsigned char Character) {
	
	LCD_CONTROL_PORT |= 1<<LCD_RS;
	LCD_CONTROL_PORT &= ~(1<<LCD_RW);
	
	LCD_DATA_PORT = Character;
	EnablePulse();
}

void PutString(unsigned char *StrPtr) {
	
	while(*StrPtr) PutCharacter(*StrPtr++);
}

void PutString_ROM(unsigned char const *StrPtr) {
	
	while(*StrPtr) PutCharacter(*StrPtr++);
}