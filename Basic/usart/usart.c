#include "usart.h"

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

#if 1
/* IAR编译器重定向 */
#pragma import(__use_no_semihosting)
struct __FILE {
  int handle;
};
FILE __stdout;
_sys_exit(int x) { x = x; }
_ttywrch(int ch) { ch = ch; }

int fputc(int ch, FILE* f) {
  while ((USART_N->SR & 0x40) == 0)
    ;
  USART_N->DR = (u8)ch;
  return ch;
}

/* GCC编译器重定向 */
int _write(int fd, char* pBuffer, int size) {
  for (int i = 0; i < size; i++) {
    while ((USART_N->SR & 0x40) == 0)
      ;
    USART_N->DR = *((u8*)pBuffer + i);
  }
  return size;
}
#endif

#if EN_USART1
u8 USART1_RX_BUF[USART1_REC_LEN];
// 接收状态
// bit15 接收完成标志
// bit14 接收到0x0d
// bit13~0 接收到的有效字节数
u16 USART1_RX_STA = 0;

/*
USART1专用printf函数
当同时开启2个以上串口时，printf函数只能用户其中之一，其他串口要创建独立的printf函数
调用方法：USART1_printf("123"); // 向USART1发送字符串123
*/
void USART1_printf(char* fmt, ...) {
  char buffer[USART1_REC_LEN + 1];
  u8 i = 0;
  va_list arg_ptr;
  va_start(arg_ptr, fmt);
  vsnprintf(buffer, USART1_REC_LEN + 1, fmt, arg_ptr);
  while ((i < USART1_REC_LEN) && (i < strlen(buffer))) {
    USART_SendData(USART1, (u8)buffer[i++]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
      ;
  }
  va_end(arg_ptr);
}

void USART1_Init(u32 bound) {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void) {
  u8 Res;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    Res = USART_ReceiveData(USART1);
    if ((USART1_RX_STA & 0x8000) == 0) {
      if (USART1_RX_STA & 0x4000) {
        if (Res != 0x0a)
          USART1_RX_STA = 0;
        else
          USART1_RX_STA |= 0x8000;
      } else {
        if (Res == 0x0d)
          USART1_RX_STA |= 0x4000;
        else {
          USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = Res;
          USART1_RX_STA++;
          if (USART1_RX_STA > (USART1_REC_LEN - 1)) USART1_RX_STA = 0;
        }
      }
    }
  }
}
#endif