
#include <stdlib.h>
#include "canRxFifo.h"


static CANMsgRxFiFoTypeDef canRxMsgBuffer = {
    .ptWrite = 0,
    .ptRead  = 0,
    .rxBuff[0].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[1].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[2].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[3].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[4].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[5].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[6].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[7].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[8].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
    .rxBuff[9].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
};

/*
 * @函数功能：向CAN接收报文缓冲区中装载新接收的CAN报文.
 * @函数参数：pWtMsg, 新接收的一帧CAN报文;
 * @返回值：0, 成功; -1, 失败;
 */
int writeCanRxMessageBuffer(CAN_MessageTypeDef* pWtMsg)
{
    uint8_t i;
    
    if (pWtMsg == NULL)return (-1);
    
    if (canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptWrite].frameType != (CAN_FrameTypeAndIDFormat_Def)0)
    {
        return (-1);
    }
    else
    {
        canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptWrite].frameId = pWtMsg->frameId;
        canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptWrite].dLc     = pWtMsg->dLc;
        
        for (i = 0; i < canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptWrite].dLc; i++)
        {
            canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptWrite].data[i] = pWtMsg->data[i];
        }
        
        canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptWrite].frameType = pWtMsg->frameType;
        
        canRxMsgBuffer.ptWrite++;
        if (canRxMsgBuffer.ptWrite >= CAN_RXMSG_FIFO_SIZE)canRxMsgBuffer.ptWrite = 0;
        
        return (0);
    }
}

/*
 * @函数功能：从CAN接收报文缓冲区中读取一帧CAN报文.
 * @函数参数：pRdMsg, 读出的一帧CAN报文装载到的缓冲区指针,用于存储读出的CAN报文;
 * @返回值：0, 成功; -1, 失败;
 */
int readCanRxMessageBuffer(CAN_MessageTypeDef* pRdMsg)
{
    uint8_t i;
    
    if (pRdMsg == NULL)return (-1);
    
    if (canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].frameType == (CAN_FrameTypeAndIDFormat_Def)0)
    {
        return (-1);
    }
    else
    {
        pRdMsg->frameId = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].frameId;
        canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].frameId = 0;
        
        pRdMsg->dLc = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].dLc;
        canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].dLc = 0;
        
        for (i = 0; i < (pRdMsg->dLc); i++)
        {
            pRdMsg->data[i] = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].data[i];
            canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].data[i] = 0;
        }
        
        pRdMsg->frameType = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].frameType;
        canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRead].frameType = (CAN_FrameTypeAndIDFormat_Def)0;
        
        canRxMsgBuffer.ptRead++;
        if (canRxMsgBuffer.ptRead >= CAN_RXMSG_FIFO_SIZE)canRxMsgBuffer.ptRead = 0;
        
        return (0);
    }
}


