#ifndef __TOUCH_H
#define __TOUCH_H

#include <stm32f10x_gpio.h>

#define TOUCH_KEYPORT GPIOA

#define TOUCH_KEY_A GPIO_Pin_0
#define TOUCH_KEY_B GPIO_Pin_1
#define TOUCH_KEY_C GPIO_Pin_2
#define TOUCH_KEY_D GPIO_Pin_3

void TOUCH_KEY_Init(void);

#endif