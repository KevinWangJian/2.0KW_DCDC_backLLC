
#ifndef  __USART_H
#define  __USART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
#define  USART_RX_INT_ENABLE    (1)             /* 串口接收中断使能 */
#define  USART_TX_INT_ENABLE    (0)             /* 串口发送中断使能 */
   
#define  USART_TX_BUF_SIZE      (18)
#define  USART_RX_BUF_SIZE      (18)
#define  USART_RX_MSG_SIZE      (10)
   
#define  USARTCOMM_VALID_FRAME_SIZE (4)         /* 串口通信每帧数据的最低有效长度 */
   
   
typedef struct
{
    uint8_t* pTxBuf;
    uint8_t* pRxBuf;
    uint8_t (*pRxBufArray)[USART_RX_BUF_SIZE];
    
    uint16_t txBufferSize;
    uint16_t rxBufferSize;
    
    volatile uint16_t txWritePtr;
    volatile uint16_t txReadPtr;
    volatile uint16_t txCount;
    
    volatile uint16_t rxWritePtr;
    volatile uint16_t rxReadPtr;
    volatile uint16_t rxCount;
    volatile uint16_t  rxFrameSize;
    
    void (*sendBeforeFunc)(void);
    void (*sendOverFunc)(void);
}USART_ParaTypeDef;


   
void usartConfig_LL(void);
void usartSendData_LL(uint8_t data);
uint8_t usartReceiveData_LL(void);
void usartCommSendData(uint8_t* pTxData, uint16_t length);
uint8_t usartCommReceiveData(uint8_t* pRxData);
void usartTxIRQ_Callback(void);
void usartRxIRQ_Callback(void);

#ifdef __cplusplus
}
#endif

#endif
