

#include "timtick.h"

static volatile uint32_t systemTimTick = 0UL;
static volatile uint32_t uwTick = 0UL;

/*
 * @函数功能：系统"滴答"定时器初始化.中断周期:1ms
 * @函数参数：无
 * @返回值：无
 */
void systemTimTickInit_LL(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, (125 - 1));       // 128分频,自动重装载值125. 16MHz/(128*125) = 1KHz
    TIM4_ARRPreloadConfig(DISABLE);                         // 不使能自动重装载,影子寄存器的值在写入ARR寄存器时立马更新
    TIM4_UpdateDisableConfig(DISABLE);                      // 计数器溢出时产生一个更新事件
    TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);    // 计数器溢出时立即发送一个中断请求
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_SetCounter(0);
    TIM4_Cmd(ENABLE);
}

/*
 * @函数功能：系统Tick计数器全局变量递增.
 * @函数参数：无
 * @返回值：无
 */
void uwTick_Increment(void)
{
    uwTick += 1;
    if (uwTick >= 0xffffffff)
    {
        uwTick = 0;
    }
}

/*
 * @函数功能：获取系统Tick计数器全局变量当前值.
 * @函数参数：无
 * @返回值：无
 */
uint32_t HAL_GetTick(void)
{
    return (uwTick);
}

/*
 * @函数功能：判断当前TimTick是否为非0值.若是,递减.
 * @函数参数：无
 * @返回值：无
 */
void timTick_Decrement(void)
{
    if (systemTimTick != 0)
    {
        systemTimTick--;
    }
}

/*
 * @函数功能：系统延时函数.毫秒级
 * @函数参数：nCount, 延时的时间值.以ms为单位.
 * @返回值：无
 */
void systemDelayms(__IO uint32_t nCount)
{
    systemTimTick = nCount;
    
    while (systemTimTick != 0){;}
}

