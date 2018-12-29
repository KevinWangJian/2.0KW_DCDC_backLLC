
#ifndef  __USARTCOMM_H
#define  __USARTCOMM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
#define  DATABUF_MAX_SIZE   (30)
   
#define  SEND_INPUTVOLT_CMD		(0x10)
#define  SEND_OUTPUTVOLT_CMD	(0x11)
#define  SEND_TEMPERATURE_CMD	(0x12)
#define  START_STOP_MACHINE_CMD	(0x13)
#define  REGULATE_VOLT_CMD      (0x14)

#define  FRAME_TYPE_SEND		(0x00)
#define  FRAME_TYPE_RESPOND		(0xFF)
   
typedef struct
{
    uint8_t command;
    uint8_t data[DATABUF_MAX_SIZE];
    uint8_t dLen;
    uint8_t update;
}uartCommDataStruct_t;
   

int8_t getFrontBoostTemperature(void);
float getInputVoltageValue(void);
float getOutputVoltageValue(void);

int usartCommParaDataUpdate(uint8_t cmd, uint8_t* pWdata, uint8_t size);
void usartCommTimeoutCallback(void);
void usartCommSendCtrlInfo(void);
void usartCommReceivedFrameParsing(void);

#ifdef __cplusplus
}
#endif

#endif
