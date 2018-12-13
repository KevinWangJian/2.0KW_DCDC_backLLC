
#ifndef  __SYSTEMSTATUS_H
#define  __SYSTEMSTATUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
typedef enum
{
    Normal = 1,
    Fault,
}systemWorkingStatusDef_t;
   

systemWorkingStatusDef_t getSystemWorkingStatus(void);
void configSystemWorkingStatus(systemWorkingStatusDef_t data);
   
#ifdef __cplusplus
}
#endif

#endif
