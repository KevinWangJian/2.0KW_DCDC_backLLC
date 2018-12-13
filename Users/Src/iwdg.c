
#include "iwdg.h"

/*
 * @函数功能：
 * @函数参数：
 * @返回值：
 */
void systemWatchDogInit_LL(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);       /* 使能IWDG寄存器的写使能. */
    IWDG_SetPrescaler(IWDG_Prescaler_128);              /* 最小超时时间2ms. */
    IWDG_SetReload(200);                                /* 超时周期T = 200 * 2ms = 400ms. */
    IWDG_Enable();                                      /* 启动IWDG */
}

/*
 * @函数功能：
 * @函数参数：
 * @返回值：
 */
void feedWatchDog_LL(void)
{
    IWDG_ReloadCounter();
}

