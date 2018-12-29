
#include <string.h>
#include "canComm.h"
#include "canSoftFifo.h"
#include "Crc16.h"
#include "escf.h"
#include "usart.h"
#include "usartComm.h"


static bool dcWorkStatusShowFlag = FALSE;



/*
 * @函数功能：CAN通信接收的数据帧解析处理.
 * @函数参数：无
 * @返回值：无
 */
void canCommReceivedFrameParsing(void)
{
    CAN_MessageTypeDef canRxMsg;
    uint8_t buf[15], Len;
    uint8_t es_buf[30], esLen;
    uint16_t crc;
    
    if (readCanRxMessageBuffer(&canRxMsg) == 0) 
    {
        writeCanTxMessageBuffer(&canRxMsg);                             /* 应答接收到的CAN数据帧. */

		if (canRxMsg.frameType == DataFrameWithExtendedId)
		{
			if (canRxMsg.frameId == 0x0C467E21)
			{
				Len = 0;
				buf[Len++] = 6;
				buf[Len++] = START_STOP_MACHINE_CMD;
				buf[Len++] = FRAME_TYPE_SEND;
				buf[Len++] = canRxMsg.data[0];
				crc = crc16(buf, Len);
				memcpy(&buf[Len], &crc, sizeof(uint16_t));
				Len += sizeof(uint16_t);

	            esLen = escfEncode(buf, Len, es_buf, sizeof(es_buf));       		/* 调用"转义"序列编码函数将待发送数据进行转义操作. */
	            es_buf[esLen + 1] = 0xF2;                                   		/* 开始添加转义操作的帧头和帧尾符. */
	            for (uint8_t i = esLen; i > 0; i--) 
				{
	                es_buf[i] = es_buf[i - 1];
	            }
	            es_buf[0] = 0xF1;
	            esLen += 2;    				

				usartCommParaDataUpdate(START_STOP_MACHINE_CMD, es_buf, esLen);   	/* 加载串口待发送的数据. */
			}
			else if (canRxMsg.frameId == 0x0C467E22)								/* 额外添加的测试ID,用于改变Boost端电压. */
			{
				Len = 0;
				buf[Len++] = 7;
				buf[Len++] = REGULATE_VOLT_CMD;
				buf[Len++] = FRAME_TYPE_SEND;
				memcpy(&buf[Len], &canRxMsg.data[0], 2);
				Len += 2;
				crc = crc16(buf, Len);
				memcpy(&buf[Len], &crc, sizeof(uint16_t));
				Len += sizeof(uint16_t);

	            esLen = escfEncode(buf, Len, es_buf, sizeof(es_buf));       		/* 调用"转义"序列编码函数将待发送数据进行转义操作. */
	            es_buf[esLen + 1] = 0xF2;                                   		/* 开始添加转义操作的帧头和帧尾符. */
	            for (uint8_t i = esLen; i > 0; i--) 
				{
	                es_buf[i] = es_buf[i - 1];
	            }
	            es_buf[0] = 0xF1;
	            esLen += 2;    				

				usartCommParaDataUpdate(REGULATE_VOLT_CMD, es_buf, esLen);   		/* 加载串口待发送的数据. */
			}
		}
    }
}

/*
 * @函数功能：CAN通信发送系统信息数据.
 * @函数参数：无
 * @返回值：无
 */
void canCommSendSystemInfo(void)
{
	CAN_MessageTypeDef canTxMsg;

	if (dcWorkStatusShowFlag == TRUE)
	{
		dcWorkStatusShowFlag = FALSE;

		canTxMsg.frameType = DataFrameWithStandardId;
		canTxMsg.frameId   = 0X123;
		canTxMsg.dLc       = 1;
		canTxMsg.data[0]   = (uint8_t)getFrontBoostTemperature();

		writeCanTxMessageBuffer(&canTxMsg); 
	}
}

/*
 * @函数功能：CAN通信发送系统信息数据.
 * @函数参数：无
 * @返回值：无
 */
void systemWorkStatusShowCallback(void)
{
	static uint16_t dcStaCnt = 0;

	dcStaCnt++;
	if (dcStaCnt >= 500)
	{
		dcStaCnt = 0;
		
		dcWorkStatusShowFlag = TRUE;
	}
}

