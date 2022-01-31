#include "buzzer.h"

#include <delay/delay.h>
#include <stm32f10x_gpio.h>

void BUZZER_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = BUZZER;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZERPORT, &GPIO_InitStructure);

  GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)1);
}

void BUZZER_BEEP1(void) {
  for (u16 i = 0; i < 200; i++) {
    GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)0);
    delay_us(500);
    GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)1);
    delay_us(500);
  }
}

void MIDI_Play(void) {
  u16 music[] = {784, 200, 784, 200, 784, 200, 880, 400, 659, 200, 784,
                 200, 698, 200, 698, 400, 587, 200, 659, 200, 698, 400,
                 587, 200, 988, 200, 880, 200, 988, 200, 784, 400};
                 
  for (u16 i = 0; i < 16; i++) {
    for (u16 e = 0; e < music[i * 2] * music[i * 2 + 1] / 1000; e++) {
      GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)0);
      delay_us(500000 / music[i * 2]);
      GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)1);
      delay_us(500000 / music[i * 2]);
    }
    delay_ms(100);
  }
}