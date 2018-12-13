
#include "clock.h"

/**
  * @brief 
  * @param None
  * @retval None
  */
void systemClockInit_LL(void)
{
    CLK_DeInit();
    CLK_LSICmd(ENABLE);                                                   /* 打开片内低速RC(128KHz)时钟 */
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_HSICmd(ENABLE);                                             /* 打开片内高速RC(16MHz)时钟  */
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET){;}
    
//    CLK_HSECmd(ENABLE);
//    while (CLK_GetFlagStatus(CLK_FLAG_HSERDY) == RESET){;}
//    while (CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE) == ERROR){;}
    
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
}
