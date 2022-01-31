#include "delay.h"

#define AHB_INPUT 72

void delay_us(u32 uS)
{
	SysTick->LOAD = AHB_INPUT * uS;
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x00000005;
	while (!(SysTick->CTRL & 0x00010000))
		;
	SysTick->CTRL = 0x00000004;
}

void delay_ms(u16 ms)
{
	while (ms-- != 0)
	{
		delay_us(1000);
	}
}

void delay_s(u16 s)
{
	while (s-- != 0)
	{
		delay_ms(1000);
	}
}
