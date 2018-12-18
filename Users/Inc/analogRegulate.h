
#ifndef  __ANALOGREGULATE_H
#define  __ANALOGREGULATE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
void voltageParaRegulate(int inputVol);
void currentParaRegulate(int inputCur);
   
#ifdef __cplusplus
}
#endif

#endif
