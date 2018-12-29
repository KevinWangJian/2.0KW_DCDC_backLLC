
#include <string.h>
#include "Crc16.h"

/*
 * @函数功能：CRC16计算函数.注意：得出的CRC16结果数据是低位在前,高位在后.
 * @函数参数：*pData, 待计算CRC的缓冲数据序列; size, 缓冲数据序列长度;
 * @返回值：返回计算出来的CRC16结果数据.
 */
uint16_t calculateCRC16(uint8_t* pData, uint16_t size)
{
	uint16_t a, b, temp, CRC16, V;

	CRC16 = 0xFFFF;

	for (a = 0; a < size; a++)
	{
		CRC16 = *pData ^ CRC16;

		for (b = 0; b < 8; b++)
		{
			temp = CRC16 & 0x0001;

			CRC16 = CRC16 >> 1;

			if (temp) {
				CRC16 = CRC16 ^ 0xA001;
			}
		}

		pData++;
	}

	V = ((CRC16 & 0x00FF) << 8) + ((CRC16 & 0xFF00) >> 8);			/* 低8位在前,高8位在后.(小端模式) */

	return (V);
}

/*
 * @函数功能：CRC16计算函数.注意：得出的CRC16结果数据是低位在前,高位在后.
 * @函数参数：*pData, 待计算CRC的缓冲数据序列; size, 缓冲数据序列长度;
 * @返回值：返回计算出来的CRC16结果数据.
 */
uint8_t dataBufCrcCheck(uint8_t *pDataBuf, uint16_t usDataLen)
{
	uint16_t crcCode1 = 0, crcCode2 = 0;

	if (usDataLen < 2)return (0);

	crcCode1 = calculateCRC16(pDataBuf, (usDataLen - 2));
	memcpy(&crcCode2, &pDataBuf[usDataLen - 2], 2);

	return (crcCode1 == crcCode2);
}





