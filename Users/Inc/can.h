
#ifndef  __CAN_H
#define  __CAN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define  CAN_BAUDRATE       (1000)            /* baudrate = 125kbps/250kbps/500kbps/1000kbps */
#define  CANTX_INT_ENABLE   (0)
#define  CANRX_INT_ENABLE   (1)
   
typedef enum
{
	DataFrameWithStandardId = 1,
	RemoteFrameWithStandardId,
	DataFrameWithExtendedId,
	RemoteFrameWithExtendedId,
}CAN_FrameTypeAndIDFormat_Def;

typedef struct
{
	uint8_t  data[8];
	uint8_t  dLc;
    uint32_t frameId;
	CAN_FrameTypeAndIDFormat_Def frameType;
}CAN_MessageTypeDef;


void canPeripheralInit_LL(void);
int canSendMessage_LL(CAN_MessageTypeDef* pSmsg);
int canReceiveMessage_LL(CAN_MessageTypeDef* pRmsg);

#ifdef __cplusplus
}
#endif

#endif