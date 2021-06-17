#ifndef SENSOR_H_
#define SENSOR_H_
#define F_CPU 16000000UL
#include <avr/delay.h>

#define DHT11_PIN 0
uint8_t c=0, I_RH, D_RH, I_Temp, D_Temp, CheckSum;

void Request()				// ATMega128로 스타트 펄스 전달 & 응답 과정
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	// PD6 LOW
	_delay_ms(20);
	PORTD |= (1<<DHT11_PIN);	// PD6 HIGH
}
void Response()				// 온습도 센서로부터 응답
{
	DDRD &= ~(1<<DHT11_PIN); // PD4 LOW
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data()
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0); //비트가 0인지 1인지 체크
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN)) //HIGH가 30ms보다 크면
		c = (c<<1)|(0x01);	 //HIGH 상태
		else
		c = (c<<1); //LOW 상태
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}
float temp_sensor_read() // 온도값 반환
{
	Request(); //시작 펄스 신호 보냄
	Response();	//센서로부터 응답 받음
	
	I_RH=Receive_data(); //습도의 정수 부분
	D_RH=Receive_data(); //습도의 실수 부분
	I_Temp=Receive_data(); //온도의 정수 부분
	D_Temp=Receive_data(); //온도의 실수 부분
	CheckSum=Receive_data(); //모든 세그먼트의 체크섬
	
	if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum);
	else
	{
		float temp = I_Temp + (D_Temp / 10.0);
		return temp;
	}
	return 0;
}
int humi_sensor_read() // 습도값 반환
{
	Request(); //시작 펄스 신호 보냄
	Response();	//센서로부터 응답 받음
	
	I_RH=Receive_data(); //습도의 정수 부분
	D_RH=Receive_data(); //습도의 실수 부분
	I_Temp=Receive_data(); //온도의 정수 부분
	D_Temp=Receive_data(); //온도의 실수 부분
	CheckSum=Receive_data(); //모든 세그먼트의 체크섬
	
	if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum);
	else
	{
		return I_RH;
	}
	return 0;
}

void ADC_set() // ADC 설정
{
	ADCSRA = (1 << ADEN); // ADC enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 16Mhz/128 = 125Khz
}
uint16_t ADC_read(uint8_t channel) // channel에 해당하는 ADC값 반환
{
	ADMUX = (1 << REFS0); // ADC0, 5V에 0~1023 설정
	ADMUX |= channel; // ADC핀 설정
	
	ADCSRA |= (1 << ADSC); // ADC 변환 시작
	while(ADCSRA & (1 << ADSC)); // ADC 변환 완료
	
	return ADCW; // Return converted value
}

#endif /* SENSOR_H_ */