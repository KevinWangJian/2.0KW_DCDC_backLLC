/**************************************************************************
模块名称: CRC16模块解析
模块功能: CRC校验码生成
输入参数: 
返回参数: 
**************************************************************************/
#ifndef __CRC16_H
#define __CRC16_H	

#include "main.h"


uint16_t crc16(uint8_t *puchMsg, uint16_t usDataLen);
uint8_t dataBufCrcCheck(uint8_t *pDataBuf, uint16_t usDataLen);



#endif


