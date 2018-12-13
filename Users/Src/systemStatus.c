
#include "systemStatus.h"


static systemWorkingStatusDef_t sysWorkSta = Normal;


/*
 * @函数功能：获取当前系统工作状态
 * @函数参数：无
 * @返回值：系统当前工作状态值
 */
systemWorkingStatusDef_t getSystemWorkingStatus(void)
{
    return (sysWorkSta);
}

/*
 * @函数功能：配置系统当前工作状态
 * @函数参数：data, 将要设置的系统工作状态值
 * @返回值：无
 */
void configSystemWorkingStatus(systemWorkingStatusDef_t data)
{
    disableInterrupts();        /* 关闭中断,下面的程序进入临界区 */
    sysWorkSta = data;
    enableInterrupts();         /* 重新开启中断,退出临界区 */
}


