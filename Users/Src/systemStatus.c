
#include "systemStatus.h"


static uint16_t systemWorkStatus = 0x0000;


/*
 * @函数功能：获取当前系统工作状态
 * @函数参数：无
 * @返回值：系统当前工作状态值
 */
uint16_t getSystemWorkingStatus(void)
{
    return (systemWorkStatus);
}

/*
 * @函数功能：配置系统当前工作状态
 * @函数参数：data, 将要设置的系统工作状态值
 * @返回值：无
 */
void configSystemWorkingStatus(uint16_t data)
{
    ENTER_CRITICAL();           /* 进入临界区 */
    systemWorkStatus = data;
    EXIT_CRITICAL();            /* 退出临界区 */
}


