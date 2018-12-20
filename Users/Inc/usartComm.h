
#ifndef  __USARTCOMM_H
#define  __USARTCOMM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
#define  DATABUF_MAX_SIZE   (15)
   
typedef struct
{
    uint8_t command;
    uint8_t data[DATABUF_MAX_SIZE];
    uint8_t dLen;
    uint8_t update;
}uartCommDataStruct_t;
   

int usartCommParaDataUpdate(uint8_t cmd, uint8_t* pWdata, uint8_t size);
void usartCommTimeoutCallback(void);
void usartCommSendCtrlInfo(void);

#ifdef __cplusplus
}
#endif

#endif
