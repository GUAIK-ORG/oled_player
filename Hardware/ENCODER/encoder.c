#include "encoder.h"

#include <stm32f10x_rcc.h>

u8 KUP;  // 旋钮锁死标志 （1为锁死）
u16 cou;

void ENCODER_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(
      RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,
      ENABLE);
  GPIO_InitStructure.GPIO_Pin = ENCODER_L | ENCODER_D;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(ENCODER_PORT_A, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = ENCODER_R;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(ENCODER_PORT_B, &GPIO_InitStructure);
}

u8 ENCODER_READ(void) {
  u8 a;
  u8 kt;
  a = 0;
  if (GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)) KUP = 0;
  if (!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L) && KUP == 0) {
    delay_us(100);
    kt = GPIO_ReadInputDataBit(ENCODER_PORT_B, ENCODER_R);
    delay_ms(3);
    if (!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)) {
      if (kt == 0) {
        a = 1;
      } else {
        a = 2;
      }
      cou = 0;
      while (!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L) && cou < 60000) {
        cou++;
        KUP = 1;
        delay_us(20);
      }
    }
  }
  if (!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_D) && KUP == 0) {
    delay_ms(20);
    if (!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_D)) {
      a = 3;
    }
  }
  return a;
}

/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
