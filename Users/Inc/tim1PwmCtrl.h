

#ifndef __TIM1PWMCTRL_H
#define __TIM1PWMCTRL_H

#include "main.h"

#define  PWM_VOLTAGE_CHANNEL
#define  PWM_CURRENT_CHANNEL

#define  VOLTAGE_CHANNEL        (TIM1_CHANNEL_2)
#define  CURRENT_CHANNEL        (TIM1_CHANNEL_3)

#define  VOLTAGE_CH_GPIO_PORT   (GPIOC)
#define  CURRENT_CH_GPIO_PORT   (GPIOC)
#define  VOLTAGE_CH_PIN         (GPIO_PIN_2)
#define  CURRENT_CH_PIN         (GPIO_PIN_3)

#define  VOLTAGE_CH_IDLE_STATE  (0)
#define  CURRENT_CH_IDLE_STATE  (0)

void timer1PwmControlInit_LL(void);
void configVoltageChannel_DutyCycle_LL(float dutyCycle);
void configCurrentChannel_DutyCycle_LL(float dutyCycle);
float getVoltageChannel_DutyCycle(void);
float getCurrentChannel_DutyCycle(void);

#endif


