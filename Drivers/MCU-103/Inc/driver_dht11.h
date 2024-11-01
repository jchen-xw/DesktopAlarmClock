#ifndef _DRIVER_DHT11_H
#define _DRIVER_DHT11_H

#include "stdint.h"

/**********************************************************************
 * 函数名称： DHT11_Init
 * 功能描述： DHT11的初始化函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void DHT11_Init(void);

/**********************************************************************
 * 函数名称： DHT11_Read
 * 功能描述： 读取DHT11的温度/湿度
 * 输入参数： 无
 * 输出参数： hum  - 用于保存湿度值
 *            temp - 用于保存温度值
 * 返 回 值： 0 - 成功, (-1) - 失败
 ***********************************************************************/
int8_t DHT11_Read(uint8_t *hum, uint8_t *temp);

/**********************************************************************
 * 函数名称： DHT11_Test
 * 功能描述： DHT11测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 ***********************************************************************/
void DHT11_Test(void);


#endif /* _DRIVER_DHT11_H */

