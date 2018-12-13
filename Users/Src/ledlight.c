
#include "ledlight.h"
#include "systemStatus.h"
#include "stm8s_it.h"

/**
  * @brief led light initial.
  * @param None
  * @retval None
  */
void ledLightInit_LL(void)
{
    GPIO_Init(LEDLIGHT_PORT, LEDLIGHT_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}

/**
  * @brief led light on.
  * @param None
  * @retval None
  */
void ledLightOn_LL(void)
{
    GPIO_WriteHigh(LEDLIGHT_PORT, LEDLIGHT_PIN);
}

/**
  * @brief led light off.
  * @param None
  * @retval None
  */
void ledLightOff_LL(void)
{
    GPIO_WriteLow(LEDLIGHT_PORT, LEDLIGHT_PIN);
}

/**
  * @brief led light off.
  * @param None
  * @retval None
  */
void ledLightToggle_LL(void)
{
    GPIO_WriteReverse(LEDLIGHT_PORT, LEDLIGHT_PIN);
}

/*
 * @函数功能：LED指示灯闪烁显示.注意：此函数仅供TIMER4中断服务程序调用.
 * @函数参数：无
 * @返回值：无
 */
void ledLightDisplay(void)
{
    const uint16_t TIMEOUT = 500;
    systemWorkingStatusDef_t sta;
    static uint16_t cnt = 0;
    
    if (getInterruptIsrVectorNumber() == 23)                     /* 系统当前被TIM4中断触发了吗? */
    {
        sta = getSystemWorkingStatus();
        
        if (sta == Normal)
        {
            cnt++;
            if (cnt >= TIMEOUT)
            {
                cnt = 0;
                ledLightToggle_LL();
            }
        }
        else if (sta == Fault)
        {
            cnt = 0;
            ledLightOn_LL();
        }
    }
}



