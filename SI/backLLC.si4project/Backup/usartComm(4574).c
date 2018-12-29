
#include <stdlib.h>
#include <string.h>
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

static int8_t usartCommTimerStart = 0;

static int8_t frontBoostTemp = 0;
static float  inputVoltage = 0.0;
static float  outputVoltage = 0.0;


/*
 * @函数功能：串口通信
 * @函数参数：无
 * @返回值：无
 */

int8_t getFrontBoostTemperature(void)
{
	return (frontBoostTemp);
}

float getInputVoltageValue(void)
{
    return (inputVoltage);
}

float getOutputVoltageValue(void)
{
    return (outputVoltage);
}
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
    if (usartCommTimerStart == -1) 
    {
    	usartCommTimerStart = 0;
		
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
    uint8_t rxBuf[25], rxLength;
    uint8_t esBuf[15], esLen;
    int8_t usartCommStatus;
    const int8_t SUCESS = 0;
    const uint8_t MAX_COUNT = 3;
    static uint8_t errCount = 0;
    uint16_t sysSta;
    
    if (uartCommDataPara.update != 0) 
    {
        usartCommStatus   = -1;
        rxLength          = 0;
        alwaysRetryFlag   = 0;
        retryAgainFlag    = 0;
        keepingRetryTimes = 0;
        maxRetryTimes     = 3;                                                          /* 没收到应答信号情况下重发3次; -1, 没收到应答信号一直重发; */
        retryDelayTime    = 20;                                                         /* 每次发送数据后最大等待超时时间20ms. */
        
        usartCommSendData(uartCommDataPara.data, uartCommDataPara.dLen);
        tim3TimeoutFunc_Start_LL(retryDelayTime);
        usartCommTimerStart = -1;
		
        while ((rxLength == 0) && (uartCommDataPara.update != 0)) 
        {
            rxLength = usartCommReceiveData(rxBuf);
            
            if (alwaysRetryFlag != 0)
            {
                alwaysRetryFlag = 0;
                usartCommSendData(uartCommDataPara.data, uartCommDataPara.dLen);
                tim3TimeoutFunc_Start_LL(retryDelayTime);
				usartCommTimerStart = -1;
            }
            
            if (retryAgainFlag != 0) 
            {
                retryAgainFlag = 0;
                usartCommSendData(uartCommDataPara.data, uartCommDataPara.dLen);
                tim3TimeoutFunc_Start_LL(retryDelayTime);
				usartCommTimerStart = -1;
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
            
            if (esBuf[1] == uartCommDataPara.command) 
            {
                if ((dataBufCrcCheck(esBuf, esLen) != 0) && (esBuf[3] == SUCESS))       /* 接收的应答数据帧正确  */
                {     
                    usartCommStatus = 0;                                                /* USART通信正常. */
                } 
                else 
                {                                                                       /* 接收的应答数据帧错误  */
                    errCount++;
                    if (errCount >= MAX_COUNT)
					{
                        usartCommStatus = 1;                                            /* USART通信错误. */
                        errCount = 0;
                    }
                }
            } 
            else 
            {
                errCount++;
                if (errCount >= MAX_COUNT)
				{
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
              sysSta  = getSystemWorkingStatus();
              sysSta &= ~(1 << UsartComm_TimeOut_Error);
              sysSta &= ~(1 << UsartComm_Data_Error);
              configSystemWorkingStatus(sysSta);
            } break;                                                                    
            
            case 1: {                                                                   /* 通信错误. */
              sysSta  = getSystemWorkingStatus();
              sysSta |= (1 << UsartComm_Data_Error);
              configSystemWorkingStatus(sysSta);
            } break;                                                                    
            
            case 2: {                                                                   /* 通信超时. */
              sysSta  = getSystemWorkingStatus();
              sysSta |= (1 << UsartComm_TimeOut_Error);
              configSystemWorkingStatus(sysSta);
            } break;                                                                    
            
            default: break;
        }

		uartCommDataPara.update = 0;
    }
}

/*
 * @函数功能：串口通信
 * @函数参数：无
 * @返回值：无
 */
void usartCommReceivedFrameParsing(void)
{
    uint8_t  Buf[30], Len;
    uint8_t  esBuf[15], esLen;
    uint8_t  error, retVal = 0xff;
    uint16_t crc;
    
    Len = usartCommReceiveData(Buf);
    
    if (Len != 0) 
	{
        esLen = (uint8_t)escfDecode(Buf, Len, esBuf, sizeof(esBuf));			/* "转义序列"解码. */
        for (uint8_t i = 0; i < esLen; i++) 
		{
            esBuf[i] = esBuf[i + 1];
        }
        esLen = esLen - 2;
		
		error = 0;
		
		if (esBuf[2] == FRAME_TYPE_SEND)									/* 接收到的数据帧是前级Boost控制部分主动发送的. */
		{
			if (esBuf[0] != esLen)error = 1;								/* 接收到的数据长度不正确. */
			if (dataBufCrcCheck(esBuf, esLen) == 0)error = 1;				/* 接收到的数据CRC校验不正确. */
			
			if (esBuf[1] == SEND_INPUTVOLT_CMD)								/* 收到的是上传输入电压命令. */
			{
				if (error == 0)
				{
                    /* TODO: Add code here to do... */
                    memcpy(&inputVoltage, &esBuf[3], sizeof(float));
                  
					retVal = 0;
				}
				else 
				{
					retVal = 0xff;
				}
			}
			else if (esBuf[1] == SEND_OUTPUTVOLT_CMD)						/* 收到的是上传输出电压命令. */
			{
				if (error == 0)
				{
					/* TODO: Add code here to do... */
                    memcpy(&outputVoltage, &esBuf[3], sizeof(float));
					
					retVal = 0;
				}
				else 
				{
					retVal = 0xff;
				}
			}
            else if (esBuf[1] == SEND_TEMPERATURE_CMD)                      /* 接收到的是上传温度命令. */
            {
				if (error == 0)
				{
					/* TODO: Add code here to do... */
					frontBoostTemp = esBuf[3];
					
					retVal = 0;
				}
				else 
				{
					retVal = 0xff;
				}                
            }
            else
            {
                return;
            }
			
			esLen = 0;
			esBuf[esLen++] = 0x06;												/* 帧长度6字节. */
			esLen++;															/* esBuf[1]即为接收到的Cmd,数据内容不变,无需修改. */
			esBuf[esLen++] = FRAME_TYPE_RESPOND;								/* 帧类型是应答帧. */
			esBuf[esLen++] = retVal;											/* 接收结果. */
			crc = calculateCRC16(esBuf, esLen);
			memcpy(&esBuf[esLen], &crc, sizeof(uint16_t));						/* CRC16校验码. */
			esLen += sizeof(uint16_t);
			
			Len = escfEncode(esBuf, esLen, Buf, sizeof(Buf));					/* 串口发送数据帧之前做"转义序列"编码. */
			Buf[Len + 1] = 0xF2;
			for (uint8_t i = Len; i > 0; i--)
			{
				Buf[i] = Buf[i - 1];
			}
			Buf[0] = 0xF1;
			Len += 2;															/* "转义序列"编码完成. */
			
			usartCommSendData(Buf, Len);                                        /* 调用串口数据发送驱动函数. */
		}
    }
}

