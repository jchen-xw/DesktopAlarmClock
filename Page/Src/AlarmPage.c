/*
 * @Author: Jchen
 * @Date: 2024-10-25 13:59:11
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-26 17:15:57
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Src\AlarmPage.c
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
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core

#include "MenuPage.h"
#include "OLED.h"
#include "Time.h"
#include "driver_Key.h"
#include "AlarmPage.h"
#include "music.h"

extern TaskHandle_t xMenuPage_Handle;

TaskHandle_t xAlarmRingtone_Handle;

TaskStatus_t xAlarmRingtone_State;

/**
 * @description: 时间显示
 * @param {uint8_t} *WriteTimeArr
 * @return {*}
 * @Date: 2024-10-26 10:49:01
 * @Author: Jchen
 */
static void Current_Display( uint8_t *WriteTimeArr )
{
    OLED_ShowString( ALARM_DEFAULT_DISPLAY_X, ALARM_DEFAULT_DISPLAY_Y, "  :  :  " , OLED_8X16 );

    OLED_ShowNum( ALARM_HOUR_DEFAULT_DISPLAY_X, ALARM_HOUR_DEFAULT_DISPLAY_Y, WriteTimeArr[ 0 ], 2, OLED_8X16 );
    OLED_ShowNum( ALARM_MIN_DEFAULT_DISPLAY_X, ALARM_MIN_DEFAULT_DISPLAY_Y, WriteTimeArr[ 1 ], 2, OLED_8X16 );
    OLED_ShowNum( ALARM_SEC_DEFAULT_DISPLAY_X, ALARM_SEC_DEFAULT_DISPLAY_Y, WriteTimeArr[ 2 ], 2, OLED_8X16 );

    OLED_ShowString( A_DISPLAY_X, A_DISPLAY_Y, "A: Enable Alarm" , OLED_6X8 );
    OLED_ShowString( B_DISPLAY_X, B_DISPLAY_Y, "B: Disable Alarm" , OLED_6X8 );
}


/**
 * @description: 光标显示
 * @param {uint8_t} *Point
 * @return {*}
 * @Date: 2024-10-26 10:50:27
 * @Author: Jchen
 */
static void Select_Display( const uint8_t *Point )
{
    switch ( *Point )
    {
        case 1:
            OLED_ReverseArea( ALARM_HOUR_DEFAULT_DISPLAY_X, ALARM_HOUR_DEFAULT_DISPLAY_Y, ALARM_RECTANGLE_WIDE, ALARM_RECTANGLE_HIGH );
            break;
        
        case 2:
            OLED_ReverseArea( ALARM_MIN_DEFAULT_DISPLAY_X, ALARM_MIN_DEFAULT_DISPLAY_Y, ALARM_RECTANGLE_WIDE, ALARM_RECTANGLE_HIGH );
            break;
        
        case 3:
            OLED_ReverseArea( ALARM_SEC_DEFAULT_DISPLAY_X, ALARM_SEC_DEFAULT_DISPLAY_Y, ALARM_RECTANGLE_WIDE, ALARM_RECTANGLE_HIGH );
            break;

        default:
            break;
    }
}


/**
 * @description: 确定光标位置
 * @param {uint8_t} *Alarm_Key
 * @return {*}
 * @Date: 2024-10-26 13:21:09
 * @Author: Jchen
 */
static uint8_t Select_Option( const uint8_t *Alarm_Key )
{
    static uint8_t Point = 1;
    if ( ( *Alarm_Key == KEY_RIGHT_RIGHT_NUM ) && ( Point < 3 ) )
    {
        Point ++;
    }
    else if ( ( *Alarm_Key == KEY_RIGHT_LEFT_NUM ) && ( Point > 1 ) )
    {
        Point --;
    }
    if ( Point > 3 )
    {
        Point = 3;
    }

    return Point;
}


/**
 * @description: 光标位置显示
 * @param {uint8_t} *Alarm_Key
 * @param {uint8_t} *WriteTimeArr
 * @param {uint8_t} *Alarm_Point
 * @return {*}
 * @Date: 2024-10-26 13:22:45
 * @Author: Jchen
 */
static void ChangeNum( const uint8_t *Alarm_Key, uint8_t *WriteTimeArr, const uint8_t *Alarm_Point )
{
    if ( *Alarm_Point == 1 )
    {
        if ( *Alarm_Key == KEY_RIGHT_UP_NUM )
        {
            WriteTimeArr[ 0 ] ++;
            if ( WriteTimeArr[ 0 ] > 23 )
            {
                WriteTimeArr[ 0 ] = 0;
            }
            
        }
        else if ( *Alarm_Key == KEY_RIGHT_DOWN_NUM )
        {
             WriteTimeArr[ 0 ] --;
             if ( WriteTimeArr[ 0 ] == 0 )
            {
                WriteTimeArr[ 0 ] = 23;
            }
        }
    }
    else if ( *Alarm_Point == 2 )
    {
        if ( *Alarm_Key == KEY_RIGHT_UP_NUM )
        {
            WriteTimeArr[ 1 ] ++;
            if ( WriteTimeArr[ 1 ] > 59 )
            {
                WriteTimeArr[ 1 ] = 0;
            }
        }
        else if ( *Alarm_Key == KEY_RIGHT_DOWN_NUM )
        {
            WriteTimeArr[ 1 ] --;
            if ( WriteTimeArr[ 1 ] == 0 )
            {
                WriteTimeArr[ 1 ] = 59;
            }
        }
    }
    else if ( *Alarm_Point == 3 )
    {
        if ( *Alarm_Key == KEY_RIGHT_UP_NUM )
        {
            WriteTimeArr[ 2 ] ++;
            if ( WriteTimeArr[ 2 ] > 59 )
            {
                WriteTimeArr[ 2 ] = 0;
            }
        }
        else if ( *Alarm_Key == KEY_RIGHT_DOWN_NUM )
        {
            WriteTimeArr[ 2 ] --;
            if ( WriteTimeArr[ 2 ] == 0 )
            {
                WriteTimeArr[ 2 ] = 59;
            }
        }
    }
}


/**
 * @description: 设定好闹钟后对比系统时间和闹钟时间如果相同生成闹铃任务
 * @param {uint8_t} *AlarmTimeArr
 * @return {*}
 * @Date: 2024-10-26 17:13:43
 * @Author: Jchen
 */
static void Background_Alarm( const uint8_t *AlarmTimeArr )
{
    uint8_t ReadTimeArr[ 3 ];

    while ( 1 )
    {
        ReadTime( ReadTimeArr );

        if ( ( ReadTimeArr[ 0 ] == AlarmTimeArr[ 0 ] ) && ( ReadTimeArr[ 1 ] == AlarmTimeArr[ 1 ] ) && ( ReadTimeArr[ 2 ] == AlarmTimeArr[ 2 ] ) )
        {
            xTaskCreate( Play_AlarmRingtone, "PlayAlarmRingtone", ALARM_RINGTONE_STK_SIZE, NULL, osPriorityAboveNormal, &xAlarmRingtone_Handle );
        }
        vTaskDelay( 500 );
        }
}


/**
 * @description: 闹钟设置任务主体
 * @param {void} *params
 * @return {*}
 * @Date: 2024-10-26 13:23:18
 * @Author: Jchen
 */
void Alarm_Task( void *params )
{
    vTaskSuspend( xMenuPage_Handle );
    
    uint8_t Alarm_Key = KEY_LEFT_A_NUM;
    uint8_t Alarm_Last_Key;
    uint8_t Alarm_Point = 1;

    static uint8_t AlarmTimeArr[] = { 13, 14, 52 };

    while ( 1 )
    {
        OLED_Clear();

        /* 显示当前内容 */
        Current_Display( AlarmTimeArr );

        /* 选定数字显示 */
        Select_Display( &Alarm_Point );
        
        OLED_Update();

        Alarm_Last_Key = Alarm_Key;
		Alarm_Key = Get_Key();
		if ( ( Alarm_Last_Key != Alarm_Key ) && ( Alarm_Key != Key_NONE ) )
        {
            /* 上下左右按键。 */
            if ( ( Alarm_Key == KEY_RIGHT_RIGHT_NUM ) || ( Alarm_Key == KEY_RIGHT_LEFT_NUM ) )
            {
                Alarm_Point = Select_Option( &Alarm_Key );
            }
            else if ( ( Alarm_Key == KEY_RIGHT_UP_NUM ) || ( Alarm_Key == KEY_RIGHT_DOWN_NUM ) )
            {
                ChangeNum( &Alarm_Key, AlarmTimeArr, &Alarm_Point );
            }
            
            if ( Alarm_Key == KEY_LEFT_A_NUM )
            {
                OLED_Clear();
                OLED_Update();
                vTaskResume( xMenuPage_Handle );                    // 恢复菜单任务线程
                Background_Alarm( AlarmTimeArr );                   // 后台运行闹钟任务线程
            }
            else if ( Alarm_Key == KEY_LEFT_B_NUM )
            {
                vTaskResume( xMenuPage_Handle );                    // 恢复菜单任务线程
                vTaskDelete( NULL );                                // 删除闹钟任务线程
            }
        }
        vTaskDelay( 50 );
    }
    
}


