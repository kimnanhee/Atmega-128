/*
3개의 스위치와 8개의 LED를 사용 
1st SW - LED ON/OFF
2nd SW - Speed UP
3rd SW - Speed DOWN
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

int led_on=1; // LED 제어
int speed=500; // 속도 제어

void delay(int value) // delay 함수 
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
      PORTF=0x01;
      for(i=0; i<7; i++)
      {
         delay(speed);
         PORTF=((PORTF<<1) | 0x01);
      }
      led_on=0;
   }
   else // Sequential OFF
   {
      PORTF=0xFE;
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
      key = (PINE & 0b1110000);
      switch(key)
      {
        case 0b0110000: // 1sw SW
        LEDOnOff();
        break;
         
        case 0b10100000: // 2nd SW - speed up
        speed+=50;
        break;
         
        case 0b11000000: // 3rd SW - speed down
        speed-=50;
        if(speed <= 100) speed=100;
        break;
      }
   }
}
