
#ifndef  __INPUTVOLDETECT_H
#define  __INPUTVOLDETECT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
#define  INPUT_CHANNEL_SIZE     (2)             /* 需要检测的输入信号通道数 */
   
#define  INPUT_FILTER_TIME      (5)             /* 检测输入信号滤波时间5*10ms(扫描基准时间),只有连续50ms检测到状态不变才认为有效 */


typedef struct
{
   int (*isInputSignalValidFunc)(void);          /* 函数指针,检测输入信号为低电平有效  */             
   uint8_t count;                                /* 滤波计数器  */ 
   uint8_t state;                                /* 输入信号当前状态.0,输入信号是无效态; 1,输入信号是有效态;  */
   uint8_t first;                                /* 输入信号首次检测. */
}InputSignalPropertyTypeDef;
   
typedef enum
{
   Input_None = 0,
   
   Power_Input12V_Valid,
   Power_Input12V_Invalid,
   
   Power_InputBAT_Valid,
   Power_InputBAT_Invalid,
}InputSignalValueTypeDef;

#define  INPUT_FIFO_SIZE    (4)

typedef struct
{
   InputSignalValueTypeDef buffer[INPUT_FIFO_SIZE];  /* 输入信号有效值存储缓冲区 */
   uint8_t writePtr;                                 /* 缓冲区写指针 */
   uint8_t readPtr;                                  /* 缓冲区读指针 */
}InputFIFOTypeDef;
   

#define  POWER_12VINPUT_GPIOPORT     (GPIOD)
#define  POWER_12VINPUT_GPIOPIN      (GPIO_PIN_3)

#define  POWER_BATINPUT_GPIOPORT     (GPIOD)
#define  POWER_BATINPUT_GPIOPIN      (GPIO_PIN_7)


void inputSignalChannelInit_LL(void);
InputSignalValueTypeDef inputSignalValue_ReadFIFO(void);
void inputSignalScan(void);

   
#ifdef __cplusplus
}
#endif

#endif
