### ATmega128과 PC간의 UART통신하기

---

ATmega128과 PC를 바로 연결해서 통신을 할 수 없으므로, 중간에 Arduino를 연결해서 통신을 한다.



준비물

```
PC
ATmega128
MCU를 제거한 Arduino
```

***꼭 Arduino의 MCU를 제거하고 통신을 해야한다.***



Arduino의 RX, TX핀에 ATmega 어떤 핀에 연결하는지에 따라서 UART number를 다르게 설정해주어야 한다.

| RX   | TX   |       |
| ---- | ---- | ----- |
| PE0  | PE1  | UART0 |
| PE2  | PE3  | UART1 |



- pc-to-pc
- pc-to-pc-ISR > 인터럽트를 사용해서 데이터 읽어오기