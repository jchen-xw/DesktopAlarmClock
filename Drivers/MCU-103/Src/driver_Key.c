/*
 * @Author: Jchen
 * @Date: 2024-09-27 14:40:02
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-23 14:12:43
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Drivers\MCU-103\Src\driver_Key.c
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
#include <string.h>

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "tim.h"

#include "OLED.h"
#include "driver_Key.h"
#include "gpio.h"

/* 为什么没用队列输入呢，因为只用按键输入，如果多设备输入可以考虑使用队列集来进行输入 */

/* 缓冲区: 用来保存解析出来的按键,可以防止丢失 */
static uint8_t g_KeysBuf[ BUF_LEN ] = { 0, 0 };

// static uint8_t Key_Jitter[ 8 ];

//static QueueHandle_t g_xQueueKey; /* 按键队列 */

//static QueueHandle_t g_xQueues[10];
//static int g_queue_cnt = 0;

uint8_t KeyVal;

// #define NEXT_POS(x) ((x+1) % BUF_LEN)

/**
 * @description: 返回按键驱动程序的队列句柄
 * @return {*}
 * @Date: 2024-09-29 17:19:24
 * @Author: Jchen
 */
//QueueHandle_t GetQueueKey(void)
//{
//	return g_xQueueKey;
//}

//void RegisterQueueHandle( QueueHandle_t queueHandle )
//{
//	if ( g_queue_cnt < 10 )
//	{
//		g_xQueues[ g_queue_cnt ] = queueHandle;
//		g_queue_cnt ++;
//	}
//}

// buffer 
/**
 * @description: 缓冲区是否有数据
 * @return {*}1为有数据，0为没有数据
 * @Date: 2024-09-29 16:45:37
 * @Author: Jchen
 */
static uint8_t isKeysBufEmpty(void)
{
	return g_KeysBuf[ 0 ];
}

/**
 * @description: 把按键数据写入缓冲区
 * @param {unsigned char} key
 * @return {*}
 * @Date: 2024-09-29 16:46:43
 * @Author: Jchen
 */
static void PutKeyToBuf( uint8_t key )
{
    if ( key != 0 )
    {
        g_KeysBuf[ 1 ] = key;
        g_KeysBuf[ 0 ] = 1;
    }
}

/**
 * @description: 从环形缓冲区读取按键数据
 * @return {*}
 * @Date: 2024-09-29 16:46:29
 * @Author: Jchen
 */
static uint8_t GetKeyFromBuf(void)
{
	uint8_t key = 0;
	if ( isKeysBufEmpty() )
	{
		key = g_KeysBuf[ 1 ];
		g_KeysBuf[ 0 ] = 0;
        return key;
	}
	else
    {
        return 0;
    }
}

/**
 * @description: 使用缓冲区读取按键
 * @return {*}
 * @Date: 2024-10-05 22:10:15
 * @Author: Jchen
 */
uint8_t Get_KeyNum( void )
{
    return GetKeyFromBuf();
}

/**
 * @description: 调用按键数值写入队列
 * @param {uint8_t} *KeyVal
 * @return {*}
 * @Date: 2024-09-29 16:13:46
 * @Author: Jchen
 */
//static void DispatchKey( uint8_t *KeyVal )
//{
//    uint8_t i;
//    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//    for ( i = 0; i < g_queue_cnt; i++ )
//    {
//        xQueueSendFromISR(g_xQueues[i], KeyVal, &xHigherPriorityTaskWoken);
//    }
//    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//}

#if 0
/**
 * @description: 判断按键是否抖动
 * @return {*}1 - 不抖动； 0 - 抖动
 * @Date: 2024-09-29 15:35:14
 * @Author: Jchen
 */
static int isKeyJitter(void)
{
    volatile uint8_t i;
    for ( i = 1; i < 8; i++ )
    {
        if ( Key_Jitter[ i ] != Key_Jitter[ i - 1 ] )
        {
            return 0;
        }
    }
    return 1;
}
#endif

/**
 * @description: 获取电平变化
 * @return {*}
 * @Date: 2024-09-29 23:28:06
 * @Author: Jchen
 */
uint8_t Get_Key( void )
{
    uint8_t Key_Num = 0;
    if ( HAL_GPIO_ReadPin( KEY_PA4_GPIO_Port, KEY_PA4_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_RIGHT_UP_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PA5_GPIO_Port, KEY_PA5_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_RIGHT_DOWN_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PA6_GPIO_Port, KEY_PA6_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_RIGHT_RIGHT_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PA7_GPIO_Port, KEY_PA7_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_RIGHT_LEFT_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PB12_GPIO_Port, KEY_PB12_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_LEFT_A_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PB13_GPIO_Port, KEY_PB13_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_LEFT_B_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PB0_GPIO_Port, KEY_PB0_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_DOWN_START_NUM;
    }
    else if ( HAL_GPIO_ReadPin( KEY_PB1_GPIO_Port, KEY_PB1_Pin ) == GPIO_PIN_RESET )
    {  
		Key_Num = KEY_DOWN_SET_NUM;
    }
	else
	{
		Key_Num = 0;
	}
    return Key_Num;
}


/**
 * @description: 中断触发扫描案件状态
 * @return {*}
 * @Date: 2024-09-30 10:29:16
 * @Author: Jchen
 */
void Scan_Key( void )
{
    
    PutKeyToBuf( Get_Key() );
}

/**
 * @description: 初始化按键
 * @return {*}
 * @Date: 2024-09-28 20:43:05
 * @Author: Jchen
 */
void Key_Init(void)
{
    // g_xQueueKey = xQueueCreate( Key_QUEUE_LEN, sizeof( uint8_t ) );
    // RegisterQueueHandle( g_xQueueKey );
}

