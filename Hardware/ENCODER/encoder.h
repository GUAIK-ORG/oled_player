#ifndef __ENCODER_H
#define __ENCODER_H

#include <delay/delay.h>
#include <stm32f10x_gpio.h>

#define ENCODER_PORT_A GPIOA
#define ENCODER_L GPIO_Pin_6
#define ENCODER_D GPIO_Pin_7

#define ENCODER_PORT_B GPIOB
#define ENCODER_R GPIO_Pin_2

void ENCODER_Init(void);  // 初始化
u8 ENCODER_READ(void);

#endif