
#ifndef  __CANSOFTFIFO_H
#define  __CANSOFTFIFO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
   
#define  CAN_RXMSG_FIFO_SIZE    (10)
#define  CAN_TXMSG_FIFO_SIZE    (10)
   
typedef struct
{
    uint8_t ptWrite;
    uint8_t ptRead;
    CAN_MessageTypeDef rxBuff[CAN_RXMSG_FIFO_SIZE];
}CANMsgRxFiFoTypeDef;

typedef struct
{
    uint8_t ptWrite;
    uint8_t ptRead;
    CAN_MessageTypeDef txBuff[CAN_TXMSG_FIFO_SIZE];
}CANMsgTxFiFoTypeDef;
   
int writeCanTxMessageBuffer(CAN_MessageTypeDef* pWtMsg);
int readCanTxMessageBuffer(CAN_MessageTypeDef* pRdMsg);
int writeCanRxMessageBuffer(CAN_MessageTypeDef* pWtMsg);
int readCanRxMessageBuffer(CAN_MessageTypeDef* pRdMsg);
   
#ifdef __cplusplus
}
#endif

#endif

