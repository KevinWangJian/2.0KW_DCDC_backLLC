

#ifndef _ESCF_H_
#define _ESCF_H_

#include "main.h"

/* Exported types ------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

int escfEncode(uint8_t *raw, int nraw, uint8_t *escf, int nescf);
int escfDecode(uint8_t *escf, int nescf, uint8_t *raw, int nraw);


#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/


