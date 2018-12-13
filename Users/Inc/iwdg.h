
#ifndef  __IWDG_H
#define  __IWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
void systemWatchDogInit_LL(void);
void feedWatchDog_LL(void);
   
#ifdef __cplusplus
}
#endif

#endif
