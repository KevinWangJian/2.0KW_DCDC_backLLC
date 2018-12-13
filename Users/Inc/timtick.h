
#ifndef __TIMTICK_H
#define __TIMTICK_H

#include "main.h"


void systemTimTickInit_LL(void);
void uwTick_Increment(void);
uint32_t HAL_GetTick(void);
void timTick_Decrement(void);
void systemDelayms(uint32_t nCount);

#endif




