
#include "analogRegulate.h"
#include "tim1PwmCtrl.h"


/*
 * @函数功能：输出电压调节,根据要输出的电压值控制PWM占空比输出
 * @函数参数：inputVol, 待输出的电压值,取值范围：9~15V.
 * @返回值：无
 */
void voltageParaRegulate(int inputVol)
{
    float vol;
    float x;
    
    if (inputVol < 9)
    {
        vol = 9.0;                          /* 要求输出的电压低于9V的情况强制输出9V */
    }
    else if (inputVol > 15)
    {
        vol = 15.0;                         /* 要求输出的电压高于15V的情况强制输出15V */
    }
    else
    {
        vol = (float)inputVol;
    }
    
    x = (15.356 - vol) / 8.3743;
    
    x = x * 100.0;
    x = 100.0 - x;                          /* 注意：在当前硬件电路和PWM输出配置的情况下,占空比需要取反. */     
    
    configVoltageChannel_DutyCycle_LL(x);   /* 调用配置电压输出通道PWM占空比. */
}

/*
 * @函数功能：输出电流调节,根据要输出的电流值控制PWM占空比输出
 * @函数参数：inputCur, 待输出的电流值.取值范围：待定.
 * @返回值：无
 */
void currentParaRegulate(int inputCur)
{
}