
#include <string.h>
#include "canComm.h"
#include "canSoftFifo.h"
#include "Crc16.h"
#include "escf.h"
#include "usart.h"
#include "usartComm.h"

/*
 * @函数功能：CAN通信接收的数据帧解析处理.
 * @函数参数：无
 * @返回值：无
 */
void canCommReceivedFrameParsing(void)
{
    CAN_MessageTypeDef canRxMsg;
    uint8_t buf[10], Len;
    uint8_t es_buf[20], esLen;
    uint16_t crc, recData;
    float duty;
    
    if (readCanRxMessageBuffer(&canRxMsg) == 0) 
    {
        writeCanTxMessageBuffer(&canRxMsg);                         /* 应答接收到的CAN数据帧. */
        
        recData = ((uint16_t)canRxMsg.data[0] << 8) + canRxMsg.data[1];
        duty = (float)recData / 10.0;
        
        Len = 0;
        buf[Len++] = 0x01;
        memcpy(&buf[Len], &duty, sizeof(float));                    /* 注意：STM8在IAR环境下采用大端模式. */
        Len += sizeof(float);
        crc = crc16(buf, Len);
        memcpy(&buf[Len], &crc, sizeof(uint16_t));                  /* 注意：STM8在IAR环境下采用大端模式. */
        Len += sizeof(uint16_t);
        
        esLen = escfEncode(buf, Len, es_buf, sizeof(es_buf));       /* 调用"转义"序列编码函数将待发送数据进行转义操作. */
        es_buf[esLen + 1] = 0xF2;                                   /* 开始添加转义操作的帧头和帧尾符. */
        for (uint8_t i = esLen; i > 0; i--) {
            es_buf[i] = es_buf[i - 1];
        }
        es_buf[0] = 0xF1;
        esLen += 2;                                                 

        usartCommParaDataUpdate(buf[0], es_buf, esLen);             /* 加载串口待发送的数据. */
    }
}


