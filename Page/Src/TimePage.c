/*
 * @Author: Jchen
 * @Date: 2024-10-25 13:59:09
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-26 10:36:00
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Src\TimePage.c
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

#include "MenuPage.h"
#include "OLED.h"
#include "Time.h"
#include "TimePage.h"
#include "driver_Key.h"

extern TaskHandle_t xMenuPage_Handle;


/**
 * @description: 默认显示内容
 * @param {uint8_t} *ReadTimeArr
 * @return {*}
 * @Date: 2024-10-25 20:56:46
 * @Author: Jchen
 */
static void Current_Display( const uint8_t *WriteTimeArr )
{
    OLED_ShowString( TIME_DEFAULT_DISPLAY_X, TIME_DEFAULT_DISPLAY_Y, "  :  :  " , OLED_8X16 );

    OLED_ShowNum( TIME_HOUR_DEFAULT_DISPLAY_X, TIME_HOUR_DEFAULT_DISPLAY_Y, WriteTimeArr[ 0 ], 2, OLED_8X16 );
    OLED_ShowNum( TIME_MIN_DEFAULT_DISPLAY_X, TIME_MIN_DEFAULT_DISPLAY_Y, WriteTimeArr[ 1 ], 2, OLED_8X16 );
    OLED_ShowNum( TIME_SEC_DEFAULT_DISPLAY_X, TIME_SEC_DEFAULT_DISPLAY_Y, WriteTimeArr[ 2 ], 2, OLED_8X16 );
}


/**
 * @description: 光标位置显示
 * @param {uint8_t} *Point
 * @return {*}
 * @Date: 2024-10-25 23:21:28
 * @Author: Jchen
 */
static void Select_Display( const uint8_t *Point )
{
    switch ( *Point )
    {
        case 1:
            OLED_ReverseArea( TIME_HOUR_DEFAULT_DISPLAY_X, TIME_HOUR_DEFAULT_DISPLAY_Y, TIME_RECTANGLE_WIDE, TIME_RECTANGLE_HIGH );
            break;
        
        case 2:
            OLED_ReverseArea( TIME_MIN_DEFAULT_DISPLAY_X, TIME_MIN_DEFAULT_DISPLAY_Y, TIME_RECTANGLE_WIDE, TIME_RECTANGLE_HIGH );
            break;
        
        case 3:
            OLED_ReverseArea( TIME_SEC_DEFAULT_DISPLAY_X, TIME_SEC_DEFAULT_DISPLAY_Y, TIME_RECTANGLE_WIDE, TIME_RECTANGLE_HIGH );
            break;
        
        default:
            break;
    }
}


/**
 * @description: 光标位置
 * @param {uint8_t} *TimeSet_Key
 * @return {*}
 * @Date: 2024-10-26 00:00:11
 * @Author: Jchen
 */
static uint8_t Select_Option( const uint8_t *TimeSet_Key )
{
    static uint8_t Point = 1;
    if ( ( *TimeSet_Key == KEY_RIGHT_RIGHT_NUM ) && ( Point < 3 ) )
    {
        Point ++;
    }
    else if ( ( *TimeSet_Key == KEY_RIGHT_LEFT_NUM ) && ( Point > 1 ) )
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
 * @description: 按位置加减时分秒数字
 * @param {uint8_t} *TimeSet_Key
 * @param {uint8_t} *WriteTimeArr
 * @param {uint8_t} *TimeSet_Point
 * @return {*}
 * @Date: 2024-10-26 00:04:30
 * @Author: Jchen
 */
static void ChangeNum( const uint8_t *TimeSet_Key, uint8_t *WriteTimeArr, const uint8_t *TimeSet_Point )
{
    if ( *TimeSet_Point == 1 )
    {
        if ( *TimeSet_Key == KEY_RIGHT_UP_NUM )
        {
            WriteTimeArr[ 0 ] ++;
            if ( WriteTimeArr[ 0 ] > 23 )
            {
                WriteTimeArr[ 0 ] = 0;
            }
            
        }
        else if ( *TimeSet_Key == KEY_RIGHT_DOWN_NUM )
        {
             WriteTimeArr[ 0 ] --;
             if ( WriteTimeArr[ 0 ] == 0 )
            {
                WriteTimeArr[ 0 ] = 23;
            }
        }
    }
    else if ( *TimeSet_Point == 2 )
    {
        if ( *TimeSet_Key == KEY_RIGHT_UP_NUM )
        {
            WriteTimeArr[ 1 ] ++;
            if ( WriteTimeArr[ 1 ] > 59 )
            {
                WriteTimeArr[ 1 ] = 0;
            }
        }
        else if ( *TimeSet_Key == KEY_RIGHT_DOWN_NUM )
        {
            WriteTimeArr[ 1 ] --;
            if ( WriteTimeArr[ 1 ] == 0 )
            {
                WriteTimeArr[ 1 ] = 59;
            }
        }
    }
    else if ( *TimeSet_Point == 3 )
    {
        if ( *TimeSet_Key == KEY_RIGHT_UP_NUM )
        {
            WriteTimeArr[ 2 ] ++;
            if ( WriteTimeArr[ 2 ] > 59 )
            {
                WriteTimeArr[ 2 ] = 0;
            }
        }
        else if ( *TimeSet_Key == KEY_RIGHT_DOWN_NUM )
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
 * @description: 程序主体
 * @param {void} *params
 * @return {*}
 * @Date: 2024-10-26 00:18:27
 * @Author: Jchen
 */
void TimeSet_Task( void *params )
{
    vTaskSuspend( xMenuPage_Handle );

    uint8_t ReadTimeArr[ 3 ];
    uint8_t WriteTimeArr[ 3 ];

    uint8_t TimeSet_Key = KEY_LEFT_A_NUM;
    uint8_t TimeSet_Last_Key;
    uint8_t TimeSet_Point = 1;

    uint8_t Cnt = 0;

    ReadTime( ReadTimeArr );
    WriteTimeArr[ 0 ] = ReadTimeArr[ 0 ];
    WriteTimeArr[ 1 ] = ReadTimeArr[ 1 ];
    WriteTimeArr[ 2 ] = ReadTimeArr[ 2 ];

    while ( 1 )
    {
        OLED_Clear();

        /* 显示当前内容 */
        Current_Display( WriteTimeArr );

        /* 选定光标闪烁 */
        if ( Cnt > 59 )
        {
            Cnt = 0;
        }
        
        if ( Cnt > 29 )
        {
            /* 选定数字显示 */
            Select_Display( &TimeSet_Point );
        }

        Cnt ++;
        
        OLED_Update();

        /* 判断重复按键 */
        TimeSet_Last_Key = TimeSet_Key;
		TimeSet_Key = Get_Key();
		if ( ( TimeSet_Last_Key != TimeSet_Key ) && ( TimeSet_Key != Key_NONE ) )
        {

            /* 上下左右按键。 */
            if ( ( TimeSet_Key == KEY_RIGHT_RIGHT_NUM ) || ( TimeSet_Key == KEY_RIGHT_LEFT_NUM ) )
            {
                TimeSet_Point = Select_Option( &TimeSet_Key );
            }
            else if ( ( TimeSet_Key == KEY_RIGHT_UP_NUM ) || ( TimeSet_Key == KEY_RIGHT_DOWN_NUM ) )
            {
                ChangeNum( &TimeSet_Key, WriteTimeArr, &TimeSet_Point );
            }

            if ( TimeSet_Key == KEY_LEFT_A_NUM )
            {
                Write_Time( WriteTimeArr );
                vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
                vTaskDelete( NULL );                            // 删除时间设置任务线程
            }
            else if ( TimeSet_Key == KEY_LEFT_B_NUM )
            {
                vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
                vTaskDelete( NULL );                            // 删除时间设置任务线程
            }
            vTaskDelay( 50 );
        }
    }
    
}

