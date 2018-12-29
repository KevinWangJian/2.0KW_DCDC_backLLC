
#include "clock.h"

/*
 * @函数功能：硬件系统时钟初始化.注意：该函数用于配置系统各时钟源和频率,需要在main函数第一步调用.
 * @函数参数：无
 * @返回值：无
 */
void systemClockInit_LL(void)
{
    CLK_DeInit();
    CLK_LSICmd(ENABLE);                                             /* 打开片内低速RC(128KHz)时钟 */
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_HSICmd(ENABLE);                                             /* 打开片内高速RC(16MHz)时钟  */
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET){;}

#if (SYSTEM_HSECLOCK_ENABLE == 1)
    CLK_HSECmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSERDY) == RESET){;}
    while (CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE) == ERROR){;}
#endif

    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
}
