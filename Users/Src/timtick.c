

#include "timtick.h"

static volatile uint32_t systemTimTick = 0UL;
static volatile uint32_t uwTick = 0UL;

/**
  * @brief led light on.
  * @param None
  * @retval None
  */
void systemTimTickInit_LL(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125);             // 128分频,自动重装载值125. 16MHz/(128*125) = 1KHz
    TIM4_ARRPreloadConfig(DISABLE);                         // 不使能自动重装载,影子寄存器的值在写入ARR寄存器时立马更新
    TIM4_UpdateDisableConfig(DISABLE);                      // 计数器溢出时产生一个更新事件
    TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);    // 计数器溢出时立即发送一个中断请求
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_SetCounter(0);
    TIM4_Cmd(ENABLE);
}

/**
  * @brief led light on.
  * @param None
  * @retval None
  */
void uwTick_Increment(void)
{
    uwTick += 1;
    if (uwTick >= 0xffffffff)
    {
        uwTick = 0;
    }
}

/**
  * @brief 
  * @param 
  * @retval 
  */
uint32_t HAL_GetTick(void)
{
    return (uwTick);
}

/**
  * @brief led light on.
  * @param None
  * @retval None
  */
void timTick_Decrement(void)
{
    if (systemTimTick != 0)
    {
        systemTimTick--;
    }
}

/**
  * @brief led light on.
  * @param None
  * @retval None
  */
void systemDelayms(__IO uint32_t nCount)
{
    systemTimTick = nCount;
    
    while (systemTimTick != 0){;}
}

