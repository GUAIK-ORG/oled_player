
#include "ENCODER/encoder.h"
#include "LM75A/LM75A.h"
#include "OLED0561/oled0561.h"
#include "TM1640/TM1640.h"
#include "delay/delay.h"
#include "i2c/i2c.h"
#include "rtc/rtc.h"
#include "sys/sys.h"
#include "usart/usart.h"

void main(void) {
  u8 buffer[3];
  delay_ms(100);        //上电时等待其他器件就绪
  RCC_Configuration();  //系统时钟初始化
  USART1_Init(115200);
  I2C_Configuration();  // I2C初始化

  OLED0561_Init();  // OLED初始化
  OLED_DISPLAY_LIT(100);
  OLED_DISPLAY_PIC();

  u8 buf[1024];
  u16 idx = 0;
  while (1) {
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
      u8 a = USART_ReceiveData(USART1);
      buf[idx++] = a;
      if (idx == 1024) {
        OLED_DISPLAY_VIDEO(buf);
        idx = 0;
      }
    }
  }
}