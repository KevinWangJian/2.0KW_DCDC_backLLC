
#include "tim1PwmCtrl.h"


static float voltageDutyCycle = 0.0;
static float currentDutyCycle = 0.0;


/**
  * @brief Timer1定时器的通道2和通道3的输出比较模式初始化.默认产生占空比
  *        为83.3%的PWM波形.
  * @param  无
  * @retval 无
  */
void timer1PwmControlInit_LL(void)
{
    uint16_t voltageChannel_CCR;
    uint16_t currentChannel_CCR;
    
    voltageDutyCycle = 83.3;
    currentDutyCycle = 83.3;
    
    voltageChannel_CCR = (uint16_t)(voltageDutyCycle * 10.0);
    currentChannel_CCR = (uint16_t)(currentDutyCycle * 10.0);
      
#ifdef PWM_VOLTAGE_CHANNEL
#if (VOLTAGE_CH_IDLE_STATE == 0)
    GPIO_Init(VOLTAGE_CH_GPIO_PORT, VOLTAGE_CH_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
#else 
    GPIO_Init(VOLTAGE_CH_GPIO_PORT, VOLTAGE_CH_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
#endif
#endif
    
#ifdef PWM_CURRENT_CHANNEL
#if (CURRENT_CH_IDLE_STATE == 0)
    GPIO_Init(CURRENT_CH_GPIO_PORT, CURRENT_CH_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
#else
    GPIO_Init(CURRENT_CH_GPIO_PORT, CURRENT_CH_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
#endif
#endif
    
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
    
    /*TIM1 Frequency = TIM1 counter clock/(ARR + 1) */
    TIM1_TimeBaseInit(15, 
                      TIM1_COUNTERMODE_UP,
                      999,
                      0);                                           /* TIM1 Frequency=16M/16/(999+1)=1KHz */
    
    TIM1_ARRPreloadConfig(DISABLE);                                 /* 不使能自动重装载,影子寄存器的值在写入ARR寄存器时立马更新 */
    
#ifdef PWM_VOLTAGE_CHANNEL
    TIM1_OC2Init(TIM1_OCMODE_PWM1, 
                 TIM1_OUTPUTSTATE_ENABLE,                           /* PWM输出打开 */
                 TIM1_OUTPUTNSTATE_DISABLE,                         /* PWM反向输出关闭 */
                 voltageChannel_CCR,                                /* CCR寄存器赋值,该值决定占空比 */
                 TIM1_OCPOLARITY_LOW,                               /* COUNT计数器值小于CCR时PWM输出低 */
                 TIM1_OCNPOLARITY_HIGH,                             /* PWM反向输出高.此处反向输出禁止,该值无影响 */
                 TIM1_OCIDLESTATE_SET,                              /* PWM空闲状态输出低 */
                 TIM1_OCNIDLESTATE_RESET);                          /* PWM反向空闲状态输出高 */
    
    TIM1_CCxCmd(VOLTAGE_CHANNEL, ENABLE);
    TIM1_OC2PreloadConfig(ENABLE);
#endif
    
#ifdef PWM_CURRENT_CHANNEL
    TIM1_OC3Init(TIM1_OCMODE_PWM1, 
                 TIM1_OUTPUTSTATE_ENABLE, 
                 TIM1_OUTPUTNSTATE_DISABLE,
                 currentChannel_CCR,
                 TIM1_OCPOLARITY_LOW,
                 TIM1_OCNPOLARITY_HIGH,
                 TIM1_OCIDLESTATE_SET,
                 TIM1_OCNIDLESTATE_RESET);                          
    
    TIM1_CCxCmd(CURRENT_CHANNEL, ENABLE);
    TIM1_OC3PreloadConfig(ENABLE);
#endif
    
    TIM1_CtrlPWMOutputs(ENABLE);
    
    TIM1_Cmd(ENABLE);
}

/**
  * @brief 获取电流控制通道的PWM当前输出占空比.
  * @param  无
  * @retval 电流通道当前PWM占空比值.
  */
void configVoltageChannel_DutyCycle_LL(float dutyCycle)
{
    uint16_t ccr_val;
    
    ccr_val = (uint16_t )(dutyCycle * 10.0);
    
    TIM1_SetCompare2(ccr_val);
}

/**
  * @brief 获取电流控制通道的PWM当前输出占空比.
  * @param  无
  * @retval 电流通道当前PWM占空比值.
  */
void configCurrentChannel_DutyCycle_LL(float dutyCycle)
{
    uint16_t ccr_val;
    
    ccr_val = (uint16_t )(dutyCycle * 10.0);
    
    TIM1_SetCompare3(ccr_val);
}

/**
  * @brief 获取电压控制通道的PWM当前输出占空比.
  * @param  无
  * @retval 电压通道当前PWM占空比值.
  */
float getVoltageChannel_DutyCycle(void)
{
    return (voltageDutyCycle);
}

/**
  * @brief 获取电流控制通道的PWM当前输出占空比.
  * @param  无
  * @retval 电流通道当前PWM占空比值.
  */
float getCurrentChannel_DutyCycle(void)
{
    return (currentDutyCycle);
}

