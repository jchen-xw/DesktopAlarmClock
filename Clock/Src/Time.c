/*
 * @Author: Jchen
 * @Date: 2024-10-23 13:46:16
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-26 23:44:31
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Clock\Src\Time.c
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


#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "OLED.h"

static uint8_t Hour = 12;
static uint8_t Min = 18;
static uint8_t Sec = 36;
static uint8_t Msec = 0;
static uint16_t ReadMsec = 0;

static uint8_t Timer_Flag = 0;

/**
 * @description: TIM2执行函数，每次中断毫秒数+10 // Msec的1为10毫秒
 * @return {*}
 * @Date: 2024-10-23 14:28:00
 * @Author: Jchen
 */
void Get_Time( void )
{
    Msec ++;
    if ( Msec >= 100 )
    {
        Msec = 0;
        Sec ++;
    }

    if ( Sec >= 60 )
    {
        Sec = 0;
        Min ++;
    }

    if ( Min >= 60 )
    {
        Min = 0;
        Hour ++;
    }

    if ( Min >= 60 )
    {
        Min = 0;
        Hour ++;
    }

    if ( Hour == 24 )
    {
        Hour = 0;
    }

    if ( Timer_Flag == 1 )
    {
        ReadMsec ++;
    }
    
}


/**
 * @description: 读取时间
 * @param {uint8_t} *TimeArr
 * @return {*}
 * @Date: 2024-10-23 14:43:31
 * @Author: Jchen
 */
void ReadTime( uint8_t *TimeArr )
{
    TimeArr[ 0 ] = Hour;
    TimeArr[ 1 ] = Min;
    TimeArr[ 2 ] = Sec;
}


void Timer_ReadTime( uint8_t *TimerArr, uint8_t *Timing_Flag )
{
    static uint8_t Read_Flag = 0;
    if ( Read_Flag == 0 )
    {
        Timer_Flag = 1;
        Read_Flag = 1;
    }

    TimerArr[ 2 ] = ReadMsec % 100;
    TimerArr[ 1 ] = ( ReadMsec / 100 ) % 60;
    TimerArr[ 0 ] = ( ReadMsec / 100 / 60 ) % 60;

    if ( *Timing_Flag == 0 )
    {
        Read_Flag = 0;
        Timer_Flag = 0;
        ReadMsec = 0;
    }
}


/**
 * @description: 时间设置写入时间
 * @param {uint8_t} *Write_TimeArr
 * @return {*}
 * @Date: 2024-10-26 20:02:05
 * @Author: Jchen
 */
void Write_Time( const uint8_t *Write_TimeArr )
{
    Hour = Write_TimeArr[ 0 ];
    Min  = Write_TimeArr[ 1 ];
    Sec  = Write_TimeArr[ 2 ];
}

void DateAndTime_Init( void )
{

}


