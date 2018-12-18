
#include <string.h>
#include "canComm.h"
#include "canSoftFifo.h"
#include "Crc16.h"
#include "escf.h"
#include "usart.h"


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
    uint16_t crc;
    float duty;
    
    if (readCanRxMessageBuffer(&canRxMsg) == 0) {
        duty = (float)canRxMsg.data[0];
        
        Len = 0;
        buf[Len++] = 0x01;
        memcpy(&buf[Len], &duty, sizeof(float));
        Len += sizeof(float);
        
        crc = crc16(buf, Len);
        memcpy(&buf[Len], &crc, sizeof(uint16_t));
        Len += sizeof(uint16_t);
        
        esLen = escfEncode(buf, Len, es_buf, sizeof(es_buf));
        
        usartCommSendData(es_buf, esLen);
        
        writeCanTxMessageBuffer(&canRxMsg);
    }
}


