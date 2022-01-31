#ifndef __I2C_H
#define __I2C_H

#include <stm32f10x_gpio.h>

#define I2CPORT GPIOB
#define I2C_SCL GPIO_Pin_6
#define I2C_SDA GPIO_Pin_7

#define HostAddress 0xc0  // 总线主机器件地址
#define BusSpeed 200000   // 总监速度（不高于400000）

void I2C_Configuration(void);
void I2C_SEND_BUFFER(u8 slaveAddr, u8 writeAddr, u8* pBuffer,
                     u16 numByteToWrite);
void I2C_SEND_BYTE(u8 slaveAddr, u8 writeAddr, u8 pBuffer);
void I2C_READ_BUFFER(u8 slaveAddr, u8 readAddr, u8* pBuffer, u16 numByteToRead);
u8 I2C_READ_BYTE(u8 slaveAddr, u8 readAddr);

#endif