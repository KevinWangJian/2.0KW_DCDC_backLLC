
#include "iwdg.h"

/*
 * @函数功能：系统独立看门狗初始化.看门狗溢出周期400ms.
 * @函数参数：无
 * @返回值：无
 */
void systemWatchDogInit_LL(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);       /* 使能IWDG寄存器的写使能. */
    IWDG_SetPrescaler(IWDG_Prescaler_128);              /* 最小超时时间2ms. */
    IWDG_SetReload(200);                                /* 超时周期T = 200 * 2ms = 400ms. */
    IWDG_Enable();                                      /* 启动IWDG */
}

/*
 * @函数功能：系统喂狗函数.注意：如果使能了看门狗功能,该函数需要在用户主循环程序中有且仅调用一次.
 * @函数参数：无
 * @返回值：无
 */
void feedWatchDog_LL(void)
{
    IWDG_ReloadCounter();
}

