
#ifndef  __CANRXFIFO_H
#define  __CANRXFIFO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
   
#define  CAN_RXMSG_FIFO_SIZE    (10)
   
typedef struct
{
    uint8_t ptWrite;
    uint8_t ptRead;
    CAN_MessageTypeDef rxBuff[CAN_RXMSG_FIFO_SIZE];
}CANMsgRxFiFoTypeDef;
   

int writeCanRxMessageBuffer(CAN_MessageTypeDef* pWtMsg);
int readCanRxMessageBuffer(CAN_MessageTypeDef* pRdMsg);
   
#ifdef __cplusplus
}
#endif

#endif

