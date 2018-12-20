
#ifndef  __SYSTEMSTATUS_H
#define  __SYSTEMSTATUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
typedef enum
{
    UsartComm_TimeOut_Error = 0,
    UsartComm_Data_Error = 1,
    
}systemErrorTypeDef_t;
   

uint16_t getSystemWorkingStatus(void);
void configSystemWorkingStatus(uint16_t data);
   
#ifdef __cplusplus
}
#endif

#endif
