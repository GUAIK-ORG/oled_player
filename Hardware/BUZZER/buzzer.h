#ifndef __BUZZER_H
#define __BUZZER_H

#include <sys/sys.h>

#include "stm32f10x_gpio.h"

#define BUZZERPORT GPIOB
#define BUZZER GPIO_Pin_5

void BUZZER_Init(void);
void BUZZER_BEEP1(void);
void MIDI_Play(void);

#endif