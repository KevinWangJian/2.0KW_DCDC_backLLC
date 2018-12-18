
#include "inputVolDetect.h"
#include "stm8s_it.h"

/*
 * @函数功能：判断通道1输出信号是否有效
 * @函数参数：无
 * @返回值：0, 有效。反映出硬件端有外部低压12V使能信号。
 *         -1, 无效。反映出硬件端没有外部低压12V使能信号。
 */
static int isChannelOneInputSig_Valid(void)
{
    if (GPIO_ReadInputPin(POWER_12VINPUT_GPIOPORT, POWER_12VINPUT_GPIOPIN) == RESET){
        return (0);
    }else{
        return (-1);
    }
}

/*
 * @函数功能：判断通道2输入信号是否有效
 * @函数参数：无
 * @返回值：0, 有效。反映出硬件端外部输入电池电压在9~16V之间。
 *         -1, 无效。反映出硬件端外部输入电池电压高于16V。
 */
static int isChannelTwoInputSig_Valid(void)
{
    if (GPIO_ReadInputPin(POWER_BATINPUT_GPIOPORT, POWER_BATINPUT_GPIOPIN) == SET){
        return (0);
    }
    else{
        return (-1);
    }
}

/*****************************************************************************
静态全局变量定义
*****************************************************************************/
static InputSignalPropertyTypeDef inputSig[INPUT_CHANNEL_SIZE] = 
{
  {
     .isInputSignalValidFunc = isChannelOneInputSig_Valid,
     .count = 0,
     .state = 0,
     .first = 0,
  },
  {
     .isInputSignalValidFunc = isChannelTwoInputSig_Valid,
     .count = 0,
     .state = 0,
     .first = 0,
  }
};

static InputFIFOTypeDef inputFifo = 
{
    .writePtr  = 0,
    .readPtr   = 0,
    .buffer[0] = Input_None,
    .buffer[1] = Input_None,
    .buffer[2] = Input_None,
    .buffer[3] = Input_None,
};

static volatile uint8_t scanCount = 0;


/*
* @函数功能：外部双通道输入信号硬件端口初始化为带上拉的输入模式
 * @函数参数：无
 * @返回值：无
 */
void inputSignalChannelInit_LL(void)
{
    GPIO_Init(POWER_12VINPUT_GPIOPORT, POWER_12VINPUT_GPIOPIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(POWER_BATINPUT_GPIOPORT, POWER_BATINPUT_GPIOPIN, GPIO_MODE_IN_PU_NO_IT);
}

/*
 * @函数功能：往输入信号写缓冲区中载入输入信号值.
 * @函数参数：data, 输入信号值.
 * @返回值：无
 */
static void inputSignalValue_WriteFIFO(uint8_t data)
{
    inputFifo.buffer[inputFifo.writePtr] = (InputSignalValueTypeDef)data;
    inputFifo.writePtr++;
    if (inputFifo.writePtr >= INPUT_FIFO_SIZE){
      inputFifo.writePtr = 0;
    }
}

/*
 * @函数功能：从缓冲区中读取信号值.
 * @函数参数：无
 * @返回值：0,无有效信号输入; 其它,输入的信号值.
 */
InputSignalValueTypeDef inputSignalValue_ReadFIFO(void)
{
    InputSignalValueTypeDef ret;
    
    if (inputFifo.readPtr == inputFifo.writePtr){
      return (Input_None);
    }else {
      ret = inputFifo.buffer[inputFifo.readPtr++];
      if (inputFifo.readPtr >= INPUT_FIFO_SIZE){
        inputFifo.readPtr = 0;
      }
      return (ret);
    }
}

/*
 * @函数功能：输入信号检测函数.
 * @函数参数：idx, 待检测的输入信号通道.
 * @返回值：无
 */
static void inputSignalDetection(uint8_t idx)
{
    InputSignalPropertyTypeDef *pInputSig = &inputSig[idx];
    
    if (pInputSig->isInputSignalValidFunc() == 0){                          /* 指定的通道输入信号有效吗? */
      if (pInputSig->count < INPUT_FILTER_TIME){
        pInputSig->count = INPUT_FILTER_TIME;
      }else if (pInputSig->count < (2 * INPUT_FILTER_TIME)){
        pInputSig->count++;
      }else {
        if ((pInputSig->state == 0) || (pInputSig->first == 0)){            /* 输入信号是无效状态吗? 或者是首次检测吗？*/
          inputSignalValue_WriteFIFO(2 * idx + 1);                          /* 将信号有效的值写入FIFO存储 */
          
          if (pInputSig->state == 0){
            pInputSig->state = 1;                                           /* 输入信号变更为有效态 */
          }
          
          if (pInputSig->first == 0){                                       /* 输入信号是首次检测吗? */
            pInputSig->first = 1;
          }
        }
      }
    }else {                                                                 /* 指定的通道输入信号无效吗？*/
      if (pInputSig->count > INPUT_FILTER_TIME){
        pInputSig->count = INPUT_FILTER_TIME;
      }else if (pInputSig->count != 0){
        pInputSig->count--;
      }else {                                                               
        if ((pInputSig->state == 1) || (pInputSig->first == 0)){            /* 输入信号是有效状态吗? 或者输入信号是首次检测吗？ */
          inputSignalValue_WriteFIFO(2 * idx + 2);                          /* 将信号有效的值写入FIFO存储 */
          
          if (pInputSig->state == 1){
            pInputSig->state = 0;                                           /* 输入信号变更为无效状态 */
          }
          
          if (pInputSig->first == 0){                                       /* 输入信号是首次检测吗? */
            pInputSig->first = 1;
          }
        }
      }
    }
}

/*
 * @函数功能：输入信号扫描检测函数.注意：该函数需要在系统"滴答"中断服务程序中周期调用.
 * @函数参数：无
 * @返回值：无
 */
void inputSignalScan(void)
{
    const uint8_t PERIOD = 10;
    uint8_t i;
    
    if (getInterruptIsrVectorNumber() == 23){
      if (++scanCount >= PERIOD){
        scanCount = 0;
        for (i = 0; i < INPUT_CHANNEL_SIZE; i++){
          inputSignalDetection(i);
        }
      }
    }
}

