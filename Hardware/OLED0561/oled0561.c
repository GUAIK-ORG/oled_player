#include "oled0561.h"

#include "ASCII_8x16.h"
#include "CHS_16x16.h"
#include "PIC.h"

void OLED0561_Init(void) {
  OLED_DISPLAY_OFF();
  OLED_DISPLAY_CLEAR();
  OLED_DISPLAY_ON();
}

void OLED_DISPLAY_ON(void) {
  u8 buf[28] = {
      0xae,        // 0xae:关显示，0xaf:开显示
      0x00, 0x10,  // 开始地址（双字节）
      0xd5, 0x80,  // 显示时钟频率？
      0xa8, 0x3f,  // 复用率？
      0xd3, 0x00,  // 显示偏移？
      0xb0,        // 写入页位置（0xB0~7）
      0x40,        // 显示开始线
      0x8d, 0x14,  // VCC电源
      0xa1,        // 设置段重新映射？
      0xc8,        // COM输出方式？
      0xda, 0x12,  // COM输出方式？
      0x81, 0xff,  // 对比度，指令：0x81，数据：0~255（255最高）
      0xd9, 0xf1,  // 充电周期？
      0xdb, 0x30,  // VCC电压输出
      0x20, 0x00,  // 水平寻址设置
      0xa4,        // 0xa4:正常显示，0xa5:整体点亮
      0xa6,        // 0xa6:正常显示，0xa7:反色显示
      0xaf         // 0xae:关显示，0xaf:开显示
  };               //
  I2C_SEND_BUFFER(OLED0561_ADDR, COM, buf, 28);
}
void OLED_DISPLAY_OFF(void) {
  u8 buf[3] = {
      0xae,        // 0xae:关显示，0xaf:开显示
      0x8d, 0x10,  // VCC电源
  };               //
  I2C_SEND_BUFFER(OLED0561_ADDR, COM, buf, 3);
}

void OLED_DISPLAY_LIT(u8 x) {
  I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x81);
  I2C_SEND_BYTE(OLED0561_ADDR, COM, x);  //亮度值
}

void OLED_DISPLAY_CLEAR(void) {
  u8 j, t;
  for (t = 0xb0; t < 0xb8; t++) {             //设置起始页地址为0xB0
    I2C_SEND_BYTE(OLED0561_ADDR, COM, t);     //页地址（从0xB0到0xB7）
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x10);  //起始列地址的高4位
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x00);  //起始列地址的低4位
    for (j = 0; j < 132; j++) {               //整页内容填充
      I2C_SEND_BYTE(OLED0561_ADDR, DAT, 0x00);
    }
  }
}

void OLED_DISPLAY_8x16(u8 x, u8 y, u16 w) {
  u8 j, t, c = 0;
  // 因OLED屏的内置驱动芯片是从0x02列作为屏上最左一列，所以要加上偏移量
  y = y + 2;
  for (t = 0; t < 2; t++) {
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0xb0 + x);  //页地址（从0xB0到0xB7）
    I2C_SEND_BYTE(OLED0561_ADDR, COM, y / 16 + 0x10);  //起始列地址的高4位
    I2C_SEND_BYTE(OLED0561_ADDR, COM, y % 16);  //起始列地址的低4位
    for (j = 0; j < 8; j++) {                   //整页内容填充
      I2C_SEND_BYTE(OLED0561_ADDR, DAT,
                    ASCII_8x16[(w * 16) + c - 512]);  //为了和ASII表对应要减512
      c++;
    }
    x++;  //页地址加1
  }
}
void OLED_DISPLAY_8x16_BUFFER(u8 row, u8* str) {
  u8 r = 0;
  while (*str != '\0') {
    OLED_DISPLAY_8x16(row, r * 8, *str++);
    r++;
  }
}

void OLED_DISPLAY_16x16(u8 x, u8 y, u16 w) {
  u8 j, t, c = 0;
  for (t = 0; t < 2; t++) {
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0xb0 + x);  // 页地址（0xb0 到 0xb7）
    I2C_SEND_BYTE(OLED0561_ADDR, COM, y / 16 + 0x10);  // 起始列地址的高4位
    I2C_SEND_BYTE(OLED0561_ADDR, COM, y % 16);         // 起始列的低4位
    for (j = 0; j < 16; j++) {
      I2C_SEND_BYTE(OLED0561_ADDR, DAT, GB_16[(w * 32) + c]);
      c++;
    }
    x++;
  }
  I2C_SEND_BYTE(OLED0561_ADDR, COM, 0xAF);  // 开显示
}

void OLED_DISPLAY_PIC(void) {
  u8 m, i;
  for (m = 0; m < 8; m++) {
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0xb0 + m);
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x10);
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x02);
    for (i = 0; i < 128; i++) {
      I2C_SEND_BYTE(OLED0561_ADDR, DAT, PIC[m * 128 + i]);
    }
  }
}

void OLED_DISPLAY_VIDEO(u8* addr) {
  u8 m, i;
  for (m = 0; m < 8; m++) {
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0xb0 + m);
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x10);
    I2C_SEND_BYTE(OLED0561_ADDR, COM, 0x02);
    for (i = 0; i < 128; i++) {
      I2C_SEND_BYTE(OLED0561_ADDR, DAT, addr[m * 128 + i]);
    }
  }
}