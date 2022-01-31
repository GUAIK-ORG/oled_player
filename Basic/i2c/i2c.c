#include "i2c.h"

#include <stm32f10x_i2c.h>
#include <stm32f10x_rcc.h>
#include <usart/usart.h>

void I2C_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(
      RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,
      ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  // 启动I2C功能
  GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(I2CPORT, &GPIO_InitStructure);
}

void I2C_Configuration(void) {
  I2C_InitTypeDef I2C_InitStructure;
  I2C_GPIO_Init();                                    // 先设置GPIO状态
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;          // 设置为I2C模式
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  // 占空比
  I2C_InitStructure.I2C_OwnAddress1 =
      HostAddress;  // 主机地址（从机不得用此地址）
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;  // 允许应答
  I2C_InitStructure.I2C_AcknowledgedAddress =
      I2C_AcknowledgedAddress_7bit;  // 7位地址模式
  I2C_InitStructure.I2C_ClockSpeed = BusSpeed;
  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
}
void I2C_SEND_BUFFER(u8 slaveAddr, u8 writeAddr, u8* pBuffer,
                     u16 numByteToWrite) {
  I2C_GenerateSTART(I2C1, ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    ;
  I2C_SendData(I2C1, writeAddr);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;
  for (u8 i = 0; i < numByteToWrite; i++) {
    I2C_SendData(I2C1, *(pBuffer + i));
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
      ;
  }
  I2C_GenerateSTOP(I2C1, ENABLE);
}
void I2C_SEND_BYTE(u8 slaveAddr, u8 writeAddr, u8 pBuffer) {
  I2C_GenerateSTART(I2C1, ENABLE);  //发送开始信号
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;  //等待完成
  I2C_Send7bitAddress(
      I2C1, slaveAddr,
      I2C_Direction_Transmitter);  //发送从器件地址及状态（写入）
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    ;                             //等待完成
  I2C_SendData(I2C1, writeAddr);  //发送从器件内部寄存器地址
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;                           //等待完成
  I2C_SendData(I2C1, pBuffer);  //发送要写入的内容
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;                              //等待完成
  I2C_GenerateSTOP(I2C1, ENABLE);  //发送结束信号
}
void I2C_READ_BUFFER(u8 slaveAddr, u8 readAddr, u8* pBuffer,
                     u16 numByteToRead) {
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    ;
  I2C_GenerateSTART(I2C1, ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    ;
  I2C_Cmd(I2C1, ENABLE);
  I2C_SendData(I2C1, readAddr);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;
  I2C_GenerateSTART(I2C1, ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    ;
  for (u16 i = 0; i < numByteToRead;) {
    if (i == numByteToRead - 1) {
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      I2C_GenerateSTOP(I2C1, ENABLE);
    }
    if (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
      *(pBuffer + i) = I2C_ReceiveData(I2C1);
      // i++必须放在这边，只有当成功读取到一条数据后才能读取下一个数据
      // 否则执行空循环
      i++;
    }
  }
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

u8 I2C_READ_BYTE(u8 slaveAddr, u8 readAddr) {
  u8 a;
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    ;
  I2C_GenerateSTART(I2C1, ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    ;
  I2C_Cmd(I2C1, ENABLE);
  I2C_SendData(I2C1, readAddr);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;
  I2C_GenerateSTART(I2C1, ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    ;
  I2C_AcknowledgeConfig(I2C1, DISABLE);  //最后有一个数据时关闭应答位
  I2C_GenerateSTOP(I2C1, ENABLE);        //最后一个数据时使能停止位
  a = I2C_ReceiveData(I2C1);
  return a;
}