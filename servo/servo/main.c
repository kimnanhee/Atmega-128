#include "uart.h"

int motor_state = 0;

int main(void)
{	
	int i;
	DDRD = 0xFF; // motor data port
	
	uart0_init(BAUDRATE(9600));	// Bluetooth connection
	
    while (1)
    {
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

ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	
	if(buf == '0') motor_state = 0;
	else if(buf == '1') motor_state = 1;
}