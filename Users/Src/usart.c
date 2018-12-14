
#include <stdlib.h>
#include <string.h>
#include "usart.h"


static uint8_t usartTxBuffer[USART_TX_BUF_SIZE];
static uint8_t usartRxBuffer[USART_RX_BUF_SIZE];
static uint8_t usartRxBufferArray[USART_RX_MSG_SIZE][USART_RX_BUF_SIZE];

static USART_ParaTypeDef usartTransmitPara = 
{
    .pTxBuf = usartTxBuffer,
    .pRxBuf = usartRxBuffer,
    .pRxBufArray  = usartRxBufferArray,
    .txBufferSize = sizeof(usartTxBuffer) / sizeof(uint8_t),
    .rxBufferSize = sizeof(usartRxBuffer) / sizeof(uint8_t),
    .txWritePtr   = 0,
    .txReadPtr    = 0,
    .txCount      = 0,
    .rxWritePtr   = 0,
    .rxReadPtr    = 0,
    .rxCount      = 0,
    .rxFrameSize  = 0,
    .sendBeforeFunc = NULL,
    .sendOverFunc   = NULL,
};


/*
 * @函数功能：串口1配置初始化.
 * @函数参数：无
 * @返回值：无
 */
void usartConfig_LL(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
    
    UART1_DeInit();
    
    UART1_Init(9600,
               UART1_WORDLENGTH_8D,
               UART1_STOPBITS_1,
               UART1_PARITY_NO,
               UART1_SYNCMODE_CLOCK_DISABLE,
               UART1_MODE_TXRX_ENABLE);
    
#if (USART_RX_INT_ENABLE == 1)
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
#endif
    
    UART1_Cmd(ENABLE);
}

/*
 * @函数功能：串口发送单个字节数据.
 * @函数参数：data,待发送的单字节数据.
 * @返回值：无
 */
void usartSendData_LL(uint8_t data)
{
    UART1_SendData8(data);
    
#if (USART_TX_INT_ENABLE == 0)
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET){;}
#endif
}

/*
 * @函数功能：串口接收单个字节数据.
 * @函数参数：无
 * @返回值：串口实际接收到的单个字节数据.
 */
uint8_t usartReceiveData_LL(void)
{
    uint8_t rxData;
    
#if (USART_RX_INT_ENABLE == 0)
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET){;}
#endif
    
    rxData = UART1_ReceiveData8();
    
    return (rxData);
}

/*
 * @函数功能：串口发送指定长度的数据.
 * @函数参数：pTxData, 待发送的数据存储缓冲区;
 * @函数参数：length, 指定的数据长度;
 * @返回值：无
 */
void usartSendNBytesData(uint8_t* pTxData, uint16_t length)
{
    uint16_t i;
    volatile uint16_t count;
    USART_ParaTypeDef *pUsartPara = &usartTransmitPara;
    
    if (pTxData == NULL)return;
    
#if (USART_TX_INT_ENABLE == 0)
    if (pUsartPara->sendOverFunc != NULL)
    {
        pUsartPara->sendBeforeFunc();
    }
      
    for (i = 0; i < length; i++)
    {
        usartSendData_LL(pTxData[i]);
    }
    
    if (pUsartPara->sendOverFunc != NULL)
    {
        pUsartPara->sendOverFunc();
    }
#else
    if (pUsartPara->sendOverFunc != NULL)
    {
        pUsartPara->sendBeforeFunc();
    }
    
    for (i = 0; i < length; i++)
    {
        while (1)                                           /* 如果发送缓冲区满了,则等待发送缓冲区空 */
        {
            ENTER_CRITICAL();
            count = pUsartPara->txCount;
            EXIT_CRITICAL();
            
            if (count < pUsartPara->txBufferSize) 
            {
                break;
            }
        }
        
        pUsartPara->pTxBuf[pUsartPara->txWritePtr] = pTxData[i];
        
        ENTER_CRITICAL();
        
        pUsartPara->txWritePtr++;
        if (pUsartPara->txWritePtr >= pUsartPara->txBufferSize) 
        {
            pUsartPara->txWritePtr = 0;
        }
        
        pUsartPara->txCount++;
        if (pUsartPara->txCount >= pUsartPara->txBufferSize)
        {
            UART1_ITConfig(UART1_IT_TXE, ENABLE);
        }
        
        EXIT_CRITICAL();
    }
    
    if (length < pUsartPara->txBufferSize)
    {
        UART1_ITConfig(UART1_IT_TXE, ENABLE);
    }
#endif
}

/*
 * @函数功能：串口发送中断回调函数
 * @函数参数：无
 * @返回值：无
 */
void usartTxIRQ_Callback(void)
{
    USART_ParaTypeDef *pUsartPara = &usartTransmitPara;
    
    if (UART1_GetITStatus(UART1_IT_TXE) != RESET) 
    {
        if (pUsartPara->txCount == 0) 
        {
            UART1_ITConfig(UART1_IT_TXE, DISABLE);
            UART1_ITConfig(UART1_IT_TC, ENABLE);
        }
        else 
        {
            usartSendData_LL(pUsartPara->pTxBuf[pUsartPara->txReadPtr]);
            
            pUsartPara->txReadPtr++;
            if (pUsartPara->txReadPtr >= pUsartPara->txBufferSize) 
            {
                pUsartPara->txReadPtr = 0;
            }
            
            pUsartPara->txCount--;
        }
    }
    else if (UART1_GetITStatus(UART1_IT_TC) != RESET) 
    {
        if (pUsartPara->txCount == 0)
        {
            UART1_ITConfig(UART1_IT_TC, DISABLE);
            
            if (pUsartPara->sendOverFunc != NULL)
            {
                pUsartPara->sendOverFunc();
            }
        }
        else
        {
            usartSendData_LL(pUsartPara->pTxBuf[pUsartPara->txReadPtr]);
            
            pUsartPara->txReadPtr++;
            if (pUsartPara->txReadPtr >= pUsartPara->txBufferSize) 
            {
                pUsartPara->txReadPtr = 0;
            }
            
            pUsartPara->txCount--;           
        }
    }
}

/*
 * @函数功能：串口接收中断回调函数
 * @函数参数：无
 * @返回值：无
 */
void usartRxIRQ_Callback(void)
{
    uint8_t rxData, length;
    USART_ParaTypeDef *pUsartPara = &usartTransmitPara;
    static uint8_t commSemMutex = 0;
    
    if ((UART1_GetITStatus(UART1_IT_RXNE) == SET) || \
        (UART1_GetITStatus(UART1_IT_OR) == SET))
    {
        rxData = usartReceiveData_LL();
        
        if (rxData == 0xF1)                 /* 接收到"转义序列"的起始符. */
        {
          /* TODO: post一个"开始接收"信号量,告知应用程序开始接收数据. */
            if (commSemMutex == 0)
            {
                pUsartPara->pRxBuf[pUsartPara->rxFrameSize++] = rxData;
                commSemMutex = 1;
            }
        }
        else if (rxData == 0xF2)            /* 接收到"转义序列"的结束符. */
        {
          /* TODO: post一个"结束接收"信号量,告知应用程序结束接收数据. */
            if ((commSemMutex == 1) && (pUsartPara->rxFrameSize >= 5))
            {
                pUsartPara->pRxBuf[pUsartPara->rxFrameSize++] = rxData;
                
                length = (uint8_t)pUsartPara->rxFrameSize;
                pUsartPara->pRxBufArray[pUsartPara->rxWritePtr][0] = length;
                memcpy(&pUsartPara->pRxBufArray[pUsartPara->rxWritePtr][1], &pUsartPara->pRxBuf[0], length);
                
                pUsartPara->rxWritePtr++;
                if (pUsartPara->rxWritePtr >= pUsartPara->rxBufferSize)
                {
                    pUsartPara->rxWritePtr = 0;
                }
                
                if (pUsartPara->rxCount < pUsartPara->rxBufferSize)
                {
                    pUsartPara->rxCount++;
                }
                
                pUsartPara->rxFrameSize = 0;
                commSemMutex = 0;
            }
            else
            {
                pUsartPara->rxFrameSize = 0;
                commSemMutex = 0;
            }
        }
        else
        {
            if (commSemMutex == 1)
            {
                pUsartPara->pRxBuf[pUsartPara->rxFrameSize++] = rxData;
                
                if (pUsartPara->rxFrameSize > pUsartPara->rxBufferSize)
                {
                    pUsartPara->rxFrameSize = 0;
                    commSemMutex = 0;
                }
            }
        }
    }
}


