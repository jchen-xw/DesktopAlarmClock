/*
 * @Author: Jchen
 * @Date: 2024-10-24 09:39:17
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-24 13:51:24
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Drivers\MCU-103\Src\driver_Light_Senser.c
 */
/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑     永不宕机     永无BUG
 */

/*
 * @Author: Jchen
 * @Date: 2024-10-24 09:39:17
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-24 09:41:20
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Drivers\MCU-103\Src\driver_Light_Senser.c
 */
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "adc.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "driver_timer.h"
#include "driver_Light_Senser.h"


extern ADC_HandleTypeDef hadc1;
static ADC_HandleTypeDef *g_HADC_LightSensor = &hadc1;

/**
 * @description: 执行ADC自动自校准
 * @return {*}
 * @Date: 2024-10-24 09:56:19
 * @Author: Jchen
 */
void LightSenser_Init( void )
{
    /* 执行ADC自动自校准 */
    HAL_ADCEx_Calibration_Start( g_HADC_LightSensor );
}


/**
 * @description: 读取光敏电阻的ADC值
 * @return {*} 0 - 失败；其他-正常值
 * @Date: 2024-10-24 10:04:55
 * @Author: Jchen
 */
static int16_t LightSenser_Value( void )
{
    HAL_ADC_Start( g_HADC_LightSensor );
    if ( HAL_ADC_PollForConversion( g_HADC_LightSensor, LIGHT_SENSOR_ADC_TIMEOUT ) == HAL_OK )
    {
        return HAL_ADC_GetValue( g_HADC_LightSensor );
    }
    else
    {
        return -1;
    }
}

void LightSenser_GetValue( uint8_t *LightValue )
{
    uint16_t ADC_Value;
    if ( LightSenser_Value() != -1 )
    {
        ADC_Value = LightSenser_Value();
        *LightValue = 100 - ( ADC_Value / 4095 * 100 );
    }
    else
    {
        *LightValue = 101;
    }
}

