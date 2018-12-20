
#include "tim3timeout.h"


static uint8_t timeoutTimer3Status = 0;


/*
 * @函数功能：TIM3超时定时器初始化,用于数据通信超时判断.
 * @函数参数：无
 * @返回值：无
 */
void tim3TimeoutFuncConfig_LL(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
    
    TIM3_DeInit();
    TIM3_TimeBaseInit(TIM3_PRESCALER_128, 125);
    TIM3_ARRPreloadConfig(DISABLE); 
    TIM3_UpdateDisableConfig(DISABLE); 
    TIM3_UpdateRequestConfig(TIM3_UPDATESOURCE_REGULAR);    
    TIM3_ClearFlag(TIM3_FLAG_UPDATE);
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
    TIM3_SetCounter(0);
}

/*
 * @函数功能：启动TIM3超时通信定时器开始计时,超时周期用户通过传参指定.
 * @函数参数：delayCnt,超时周期.毫秒级
 * @返回值：0,启动超时定时器成功; -1,启动超时定时器失败;
 */
int tim3TimeoutFunc_Start_LL(uint8_t delayCnt)
{
    uint16_t arr = 125 * delayCnt;
    
    if (arr >= 65500){
        timeoutTimer3Status = 0;
        return (-1);
    }
    TIM3_TimeBaseInit(TIM3_PRESCALER_128, arr);
    TIM3_SetCounter(0);
    TIM3_ClearFlag(TIM3_FLAG_UPDATE);
    TIM3_Cmd(ENABLE);
    
    timeoutTimer3Status = 0xff;
    
    return (0);
}

/*
 * @函数功能：关停TIM3超时定时器计时.
 * @函数参数：无
 * @返回值：无
 */
void tim3TimeoutFunc_Stop_LL(void)
{
    if (timeoutTimer3Status == 0xff) 
    {
        TIM3_Cmd(DISABLE);
        timeoutTimer3Status = 0;
    }
}
