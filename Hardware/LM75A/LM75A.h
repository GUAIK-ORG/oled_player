#ifndef __LM75A_H
#define __LM75A_H

#include <i2c/i2c.h>

#define LM75A_ADD 0x9E

void LM75A_GetTemp(u8 *tempBuffer);  // 读温度
void LM75A_POWERDOWN(void);          // 掉电模式

#endif
