#ifndef  __CRC16_H
#define  __CRC16_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"


uint16_t calculateCRC16(uint8_t* pData, uint16_t size);
uint8_t dataBufCrcCheck(uint8_t *pDataBuf, uint16_t usDataLen);

#ifdef __cplusplus
}
#endif

#endif




