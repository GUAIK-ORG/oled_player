#include "flash.h"

#include <stm32f10x_flash.h>

void FLASH_W(u32 add, u16 dat) {
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR |
                  FLASH_FLAG_WRPRTERR);
  FLASH_ErasePage(add);
  FLASH_ProgramHalfWord(add, dat);
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR |
                  FLASH_FLAG_WRPRTERR);
  FLASH_Lock();
}

u16 FLASH_R(u32 add) { return *((u16*)add); }