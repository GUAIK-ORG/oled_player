#include "LM75A.h"

void LM75A_GetTemp(u8 *tempBuffer) {
  u8 buf[2];  //温度值储存
  u8 t = 0, a = 0;
  I2C_READ_BUFFER(LM75A_ADD, 0x00, buf,
                  2);  //读出温度值（器件地址，子地址，数据储存器，字节数）
  t = buf[0];         //处理温度整数部分，0~125度
  *tempBuffer = 0;    //温度值为正值
  if (t & 0x80) {     //判断温度是否是负（MSB表示温度符号）
    *tempBuffer = 1;  //温度值为负值
    t = ~t;
    t++;  //计算补码（原码取反后加1）
  }
  if (t & 0x01) {
    a = a + 1;
  }  //从高到低按位加入温度积加值（0~125）
  if (t & 0x02) {
    a = a + 2;
  }
  if (t & 0x04) {
    a = a + 4;
  }
  if (t & 0x08) {
    a = a + 8;
  }
  if (t & 0x10) {
    a = a + 16;
  }
  if (t & 0x20) {
    a = a + 32;
  }
  if (t & 0x40) {
    a = a + 64;
  }
  tempBuffer++;
  *tempBuffer = a;
  a = 0;
  t = buf[1];  //处理小数部分，取0.125精度的前2位（12、25、37、50、62、75、87）
  if (t & 0x20) {
    a = a + 12;
  }
  if (t & 0x40) {
    a = a + 25;
  }
  if (t & 0x80) {
    a = a + 50;
  }
  tempBuffer++;
  *tempBuffer = a;
}
void LM75A_POWERDOWN(void) { I2C_SEND_BYTE(LM75A_ADD, 0x01, 1); }