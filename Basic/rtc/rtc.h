#ifndef __RTC_H
#define __RTC_H

#include "sys/sys.h"

extern u16 ryear;
extern u8 rmon, rday, rhour, rmin, rsec, rweek;

u8 RTC_Get(void);             // 读出当前时间值
void RTC_First_Config(void);  // 首次启用RTC的设置
void RTC_Config(void);        // 实时时钟初始化
u8 Is_Leap_Year(u16 year);    // 判断是否是闰年
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);  // 写入时间
u8 RTC_Get_Week(u16 year, u8 month, u8 dat);  // 按年月日计算星期

#endif