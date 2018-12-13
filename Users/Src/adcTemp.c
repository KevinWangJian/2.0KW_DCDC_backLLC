
#include "adcTemp.h"


/*
 * @函数功能：温度传感器采样端口的ADC初始化.
 * @函数参数：无
 * @返回值：无
 */
void temperatureSensorAdcInit_LL(void)
{
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
    ADC2_DeInit();
    ADC2_Init(ADC2_CONVERSIONMODE_SINGLE,
              ADC2_CHANNEL_0,
              ADC2_PRESSEL_FCPU_D8,
              ADC2_EXTTRIG_TIM,
              DISABLE,
              ADC2_ALIGN_RIGHT,
              ADC2_SCHMITTTRIG_CHANNEL0,
              DISABLE
              );
    
    ADC2_ITConfig(ENABLE);
    ADC2_Cmd(ENABLE);
    ADC2_StartConversion();
}
