
#include <stdlib.h>
#include "escf.h"

/*****************************************************************************************
 * @brief A function which does escaping for byte arraies, that is, all bytes with values
          of 0xF0, 0xF1 and 0xF2 will be taken over by combinations of 2 bytes, i.e, the
          byte 0xF0 will be turned into 0xF0+0x00, the byte 0xF1  will be turned into
          0xF0+0x01 and the byte 0xF2 will be turned into 0xF0+0x02.

 * @param raw: the start address of the given byte array to be escaped.
          nraw: the length of the 'raw' in unit of byte.
          escf: the start address of the buffer to contain the result.
          nescf: the maximum length of the 'escf' in unit of byte.

 * @returns -1:    failed.
            other: the length of the encoded array.
*****************************************************************************************/

// 0xf1 ---- 0xf0 0x01 
// 0xf0 ---- 0xf0 0x00
// 0xf2 ---- 0xf0 0x02
// 0x21 0x12 0xf2 0x9d --- 0xf1 0x21 0x12 0xf0 0x02 0x9d 0xf2
int escfEncode(uint8_t *raw, int nraw, uint8_t *escf, int nescf) {
	int i, nret = 0, len = 0;
	int ret_val = -1;
	uint8_t c;
	
	if ((raw != NULL) && (escf != NULL) && (nraw >= 0) && (nescf >= nraw)) {
		for (i = 0; i < nraw; i++) {
			c = *raw++;
			
			if ((c >= 0xF0) && (c <= 0xF2)) {
				if ((nret += 2) <= nescf) {
					*escf++ = 0xF0;
					*escf++ = c - 0xF0;
					len += 2;
				} else {
					break;
				}
			} else {
				if ((nret += 1) <= nescf) {
					*escf++ = c;
					len += 1;
				} else {
					break;
				}
			}
		}
		
		if (i == nraw) {
			ret_val = len;
		}
	}
	
	return ret_val;
}

/*****************************************************************************************
 * @brief Inverse function of the 'escfEncode'.

 * @param escf: the start address of the given escaped array to be decoded.
          nescf: the length of the 'escf' in unit of byte.
          raw: the start address of the buffer to contain the result.
          nraw: the maximum length of the 'raw' in unit of byte.

 * @returns -1:    failed.
            other: the length of the decoded array.
*****************************************************************************************/
int escfDecode(uint8_t *escf, int nescf, uint8_t *raw, int nraw) {
	int i, nret = 0;
	int ret_val = -1;
	uint8_t c;
	
	enum {
		DONE,
		ESCAPING,
		BAD,
	} fsm = DONE;
	
	if ((raw != NULL) && (escf != NULL) && (nescf >= 0) && (nraw >= nescf)) {
		for (i = 0; i < nescf; i++) {
			c = *escf++;
			
			switch (fsm) {
				case DONE:
					if (c == 0xF0) {
						fsm = ESCAPING;
					} else {
						*raw++ = c;
						nret += 1;
					}
					break;
				
				case ESCAPING:
					if (c <= 2) {
						*raw++ = c + 0xF0;
						nret += 1;
						
						fsm = DONE;
					} else {
						fsm = BAD;
					}
					break;
				
				default:
					fsm = BAD;
					break;
			}
			
			if (fsm == BAD) {
				break;
			}
		}
		
		if (fsm == DONE) {
			ret_val = nret;
		}
	}
	
	return ret_val;
}


