
#ifndef  __USART_H
#define  __USART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
#define  USART_RX_INT_ENABLE    (1)             /* 串口接收中断使能 */
#define  USART_TX_INT_ENABLE    (0)             /* 串口发送中断禁止 */
   
void usartConfig_LL(void);
void usartSendData_LL(uint8_t data);
uint8_t usartReceiveData_LL(void);
void usartSendNBytesData(uint8_t* pTxData, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
