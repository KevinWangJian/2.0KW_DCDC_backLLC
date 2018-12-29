
#include "ledlight.h"
#include "systemStatus.h"
#include "stm8s_it.h"

/*
 * @函数功能：led灯控制IO口初始化并默认输出熄灭led灯.
 * @函数参数：无
 * @返回值：无
 */
void ledLightInit_LL(void)
{
    GPIO_Init(LEDLIGHT_PORT, LEDLIGHT_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}

/*
 * @函数功能：点亮LED灯.
 * @函数参数：无
 * @返回值：无
 */
void ledLightOn_LL(void)
{
    GPIO_WriteHigh(LEDLIGHT_PORT, LEDLIGHT_PIN);
}

/*
 * @函数功能：熄灭led灯.
 * @函数参数：无
 * @返回值：无
 */
void ledLightOff_LL(void)
{
    GPIO_WriteLow(LEDLIGHT_PORT, LEDLIGHT_PIN);
}

/*
 * @函数功能：led灯取反.
 * @函数参数：无
 * @返回值：无
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
    const uint16_t TIMEOUT = 350;
    static uint16_t cnt = 0;
    uint16_t sta;
    
    if (getInterruptIsrVectorNumber() == 23)                     /* 系统当前被TIM4中断触发了吗? */
    {
        sta = getSystemWorkingStatus();
        
        if (sta == 0)
        {
            cnt++;
            if (cnt >= TIMEOUT)
            {
                cnt = 0;
                ledLightToggle_LL();
            }
        }
        else
        {
            cnt = 0;
            ledLightOn_LL();
        }
    }
}



