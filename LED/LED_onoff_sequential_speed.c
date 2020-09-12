/*
3개의 스위치와 8개의 LED를 사용 
1st SW - LED ON/OFF
2nd SW - Speed UP
3rd SW - Speed DOWN

PIN 입력
SW push - 0
 
PORT 출력
0 - LED OFF
1 - LED ON
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

int led_on=1; // LED 제어
int speed=500; // 속도 제어

void delay(int value) // 사용자 delay 함수 
{
   while(value > 0)
   {
      value--;
      _delay_ms(1);
   }
}
void LEDOnOff()
{
   int i;
   if(led_on) // Sequential ON
   {
      PORTF=0x01; // 0000 0001
      for(i=0; i<7; i++)
      {
        delay(speed);
        PORTF=((PORTF<<1) | 0x01);
      }
      led_on=0;
   }
   else // Sequential OFF
   {
      PORTF=0xFE; // 1111 1110
      for(i=0; i<7; i++)
      {
        delay(speed);
        PORTF=((PORTF<<1) & 0xFF);
      }
      led_on=1;
   }
}
int main(void)
{
   DDRF = 0xff; // OUTPUT setting - LED
   DDRE = 0x00; // INPUT setting - SW
   
   unsigned char key;

   while(1)
   {
      key = (PINE & 0xE0); // 1110 0000
      switch(key)
      {
    	// 1sw SW
        case 0x60: // 0110 0000
        LEDOnOff();
        break;
        
        // 2nd SW - speed up
        case 0xA0: // 1010 0000
        speed+=50;
        break;
        
        // 3rd SW - speed down
        case 0xC0: // 1100 0000 
        speed-=50;
        if(speed <= 100) speed=100;
        break;
      }
      delay(10);
   }
}
