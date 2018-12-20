
#include <stdlib.h>
#include "usartComm.h"
#include "usart.h"
#include "tim3timeout.h"
#include "Crc16.h"
#include "escf.h"
#include "systemStatus.h"



static uartCommDataStruct_t uartCommDataPara = {
    .command = 0,
    .dLen    = 0,
    .update  = 0,
};

static int8_t maxRetryTimes = 0;
static int8_t retryDelayTime = 0;
static int8_t keepingRetryTimes = 0;

static int8_t alwaysRetryFlag = 0;
static int8_t retryAgainFlag  = 0;


/*
 * @函数功能：串口通信
 * @函数参数：无
 * @返回值：无
 */
int usartCommParaDataUpdate(uint8_t cmd, uint8_t* pWdata, uint8_t size)
{
    if (pWdata == NULL)return (-1);
    if (size > DATABUF_MAX_SIZE)return (-1);
    if (uartCommDataPara.update != 0)return (-1);
    
    uartCommDataPara.command = cmd;
    uartCommDataPara.dLen    = size;
    for (uint8_t i = 0; i < size; i++) 
    {
        uartCommDataPara.data[i] = pWdata[i];
    }
    uartCommDataPara.update = 0xff;
    
    return (0);
}

/*
 * @函数功能：串口通信
 * @函数参数：无
 * @返回值：无
 */
void usartCommTimeoutCallback(void)
{
    if (uartCommDataPara.update) 
    {
        if (maxRetryTimes == -1) 
        {
            alwaysRetryFlag = 0xff;
        } 
        else if (keepingRetryTimes < (maxRetryTimes - 1)) 
        {
            keepingRetryTimes += 1;
            retryAgainFlag = 0xff;
        } 
        else 
        {
            tim3TimeoutFunc_Stop_LL();
            uartCommDataPara.update = 0;
        }
    }
}

/*
 * @函数功能：串口通信
 * @函数参数：无
 * @返回值：无
 */
void usartCommSendCtrlInfo(void)
{
    uint8_t rxBuf[20], rxLength;
    uint8_t esBuf[10], esLen;
    int8_t usartCommStatus;
    const int8_t SUCESS = 0;
    const uint8_t MAX_COUNT = 3;
    static uint8_t errCount = 0;
    uint16_t sysSta;
    
    if (uartCommDataPara.update) 
    {
        usartCommStatus   = -1;
        rxLength          = 0;
        alwaysRetryFlag   = 0;
        retryAgainFlag    = 0;
        keepingRetryTimes = 0;
        maxRetryTimes     = 3;                                                          /* 没收到应答信号情况下重发3次; -1, 没收到应答信号一直重发; */
        retryDelayTime    = 10;                                                         /* 每次发送数据后最大等待超时时间10ms. */
        
        usartCommSendData(uartCommDataPara.data, uartCommDataPara.dLen);
        tim3TimeoutFunc_Start_LL(retryDelayTime);
        
        while ((rxLength == 0) && (uartCommDataPara.update != 0)) 
        {
            rxLength = usartCommReceiveData(rxBuf);
            
            if (alwaysRetryFlag != 0)
            {
                alwaysRetryFlag = 0;
                usartCommSendData(uartCommDataPara.data, uartCommDataPara.dLen);
                tim3TimeoutFunc_Start_LL(retryDelayTime);                
            }
            
            if (retryAgainFlag != 0) 
            {
                retryAgainFlag = 0;
                usartCommSendData(uartCommDataPara.data, uartCommDataPara.dLen);
                tim3TimeoutFunc_Start_LL(retryDelayTime);               
            }
        }
        
        if (rxLength != 0)                                                              /* 收到了串口应答数据帧.   */
        {                                                                               
            tim3TimeoutFunc_Stop_LL();                                                  /* 关闭超时定时器计数器. */
            
            esLen = (uint8_t)escfDecode(rxBuf, rxLength, esBuf, sizeof(esBuf));
            for (uint8_t i = 0; i < esLen; i++) 
            {
                esBuf[i] = esBuf[i + 1];
            }
            esLen = esLen - 2;
            
            if (esBuf[0] == uartCommDataPara.command) 
            {
                if ((dataBufCrcCheck(esBuf, esLen) != 0) && (esBuf[1] == SUCESS))       /* 接收的应答数据帧正确  */
                {     
                    usartCommStatus = 0;                                                /* USART通信正常. */
                } 
                else 
                {                                                                       /* 接收的应答数据帧错误  */
                    errCount++;
                    if (errCount >= MAX_COUNT){
                        usartCommStatus = 1;                                            /* USART通信错误. */
                        errCount = 0;
                    }
                }
            } 
            else 
            {
                errCount++;
                if (errCount >= MAX_COUNT){
                    usartCommStatus = 1;                                                 /* USART通信错误. */
                    errCount = 0;
                }
            }
        } 
        else if (uartCommDataPara.update == 0)                                          /* 没有收到串口应答数据帧且多次超时. */
        {                                      
            usartCommStatus = 2;                                                        /* USART通信超时. */
        }
        
        switch (usartCommStatus)
        {
            case 0: {                                                                   /* 通信正常. */
              uartCommDataPara.update = 0;
              
              sysSta  = getSystemWorkingStatus();
              sysSta &= ~(1 << UsartComm_TimeOut_Error);
              sysSta &= ~(1 << UsartComm_Data_Error);
              configSystemWorkingStatus(sysSta);
            } break;                                                                    
            
            case 1: {                                                                   /* 通信错误. */
              uartCommDataPara.update = 0; 
              
              sysSta  = getSystemWorkingStatus();
              sysSta |= (1 << UsartComm_Data_Error);
              configSystemWorkingStatus(sysSta);
            } break;                                                                    
            
            case 2: {                                                                   /* 通信超时. */
              uartCommDataPara.update = 0; 
              
              sysSta  = getSystemWorkingStatus();
              sysSta |= (1 << UsartComm_TimeOut_Error);
              configSystemWorkingStatus(sysSta);
            } break;                                                                    
            
            default: break;
        }
    }
}

