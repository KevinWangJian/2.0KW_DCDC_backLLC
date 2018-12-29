
#ifndef  __CANCOMM_H
#define  __CANCOMM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
void canCommReceivedFrameParsing(void);
void canCommSendSystemInfo(void);
void systemWorkStatusShowCallback(void);


#ifdef __cplusplus
}
#endif

#endif

