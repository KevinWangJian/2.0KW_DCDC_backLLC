
#ifndef  __TIM3TIMEOUT_H
#define  __TIM3TIMEOUT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
void tim3TimeoutFuncConfig_LL(void);
int tim3TimeoutFunc_Start_LL(uint8_t delayCnt);
void tim3TimeoutFunc_Stop_LL(void);

#ifdef __cplusplus
}
#endif

#endif
