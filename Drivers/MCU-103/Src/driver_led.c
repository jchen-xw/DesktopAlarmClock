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
 * @Date: 2024-09-26 13:33:36
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-02 21:48:58
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Drivers\MCU-103\Src\driver_led.c
 */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include "driver_timer.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core

#include "gpio.h"
#include "OLED.h"
#include "driver_led.h"
#include "driver_Key.h"
#include "MenuPage.h"
#include "music.h"
#include "MenuPage.h"


/**********************************************************************
 * 函数名称： Led_Init
 * 功能描述： LED初始化函数, 在HAL的初始化代码里已经配置好了GPIO引脚, 
 *            所以本函数可以写为空
 *            但是为了不依赖于stm32cubemx, 此函数也实现了GPIO的配置
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/
int Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    return 0;
}

/**********************************************************************
 * 函数名称： Led_Control
 * 功能描述： LED控制函数
 * 输入参数： which-哪个LED, 在driver_led.h里定义, 比如LED_GREEN
 *            on-状态, 1-亮, 0-灭
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/
int Led_Control(int which, int on)
{
    if (on)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    return 0;
}

/**********************************************************************
 * 函数名称： Led_Test
 * 功能描述： Led测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/

void Led_Test( void *params )
{
    Led_Init();
    OLED_Init();

    
#if 0

#endif

    while (1)
    {
        Led_Control(LED_GREEN, 1);
        // mdelay(500);
        vTaskDelay( 500 );

        Led_Control(LED_GREEN, 0);
        // mdelay(500);
        vTaskDelay( 500 );
    }
}

