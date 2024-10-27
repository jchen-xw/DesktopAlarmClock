/*
 * @Author: Jchen
 * @Date: 2024-10-25 13:59:11
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-26 23:42:24
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Src\TimerPage.c
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
#include "driver_Key.h"
#include "TimerPage.h"

extern TaskHandle_t xMenuPage_Handle;


void Current_Display( const uint8_t *TimerArr, const uint8_t *Result_1_Arr, const uint8_t *Result_2_Arr )
{
    OLED_ShowString( TIME_DISPLAY_X, TIME_DISPLAY_Y, "  :  :  " , OLED_8X16 );
    OLED_ShowString( TIMER_RESULT_1_X, TIMER_RESULT_1_Y, "  :  :  " , OLED_6X8 );
    OLED_ShowString( TIMER_RESULT_2_X, TIMER_RESULT_2_Y, "  :  :  " , OLED_6X8 );

    OLED_ShowNum( TIMER_MIN_DISPLAY_X, TIMER_MIN_DISPLAY_Y, TimerArr[ 0 ], 2, OLED_8X16 );
    OLED_ShowNum( TIMER_SEC_DISPLAY_X, TIMER_SEC_DISPLAY_Y, TimerArr[ 1 ], 2, OLED_8X16 );
    OLED_ShowNum( TIMER_MSEC_DISPLAY_X, TIMER_MSEC_DISPLAY_Y, TimerArr[ 2 ], 2, OLED_8X16 );

    OLED_ShowNum( RESULT_1_MIN_X, RESULT_1_MIN_Y, Result_1_Arr[ 0 ], 2, OLED_6X8 );
    OLED_ShowNum( RESULT_1_SEC_X, RESULT_1_SEC_Y, Result_1_Arr[ 1 ], 2, OLED_6X8 );
    OLED_ShowNum( RESULT_1_MSEC_X, RESULT_1_MSEC_Y, Result_1_Arr[ 2 ], 2, OLED_6X8 );

    OLED_ShowNum( RESULT_2_HOUR_X, RESULT_2_HOUR_Y, Result_2_Arr[ 0 ], 2, OLED_6X8 );
    OLED_ShowNum( RESULT_2_MIN_X, RESULT_2_MIN_Y, Result_2_Arr[ 1 ], 2, OLED_6X8 );
    OLED_ShowNum( RESULT_2_SEC_X, RESULT_2_SEC_Y, Result_2_Arr[ 2 ], 2, OLED_6X8 );
}


void Timer_Task( void *params )
{
    vTaskSuspend( xMenuPage_Handle );

    uint8_t TimerArr[] = { 0, 0, 0, 0 };
    uint8_t Result_1_Arr[] = { 0, 0, 0, 0 };
    uint8_t Result_2_Arr[] = { 0, 0, 0, 0 };

    uint8_t Timer_Key = KEY_LEFT_A_NUM;
    uint8_t Timer_Last_Key;
    uint8_t Timing_Flag = 0;

    while ( 1 )
    {
        OLED_Clear();

        /* 显示当前内容 */
        Current_Display( TimerArr, Result_1_Arr, Result_2_Arr );

        /* 获取时间 */
        if ( Timing_Flag == 1 )
        {
            /* 有问题 */
            Timer_ReadTime( TimerArr, &Timing_Flag );
        }
        
        OLED_Update();


        Timer_Last_Key = Timer_Key;
		Timer_Key = Get_Key();
		if ( ( Timer_Last_Key != Timer_Key ) && ( Timer_Key != Key_NONE ) )
        {
            /* 计时开始，分别固定3个结果 */
            if ( Timer_Key == KEY_LEFT_A_NUM )
            {
                /* 开始计时 */
                if ( ( TimerArr[ 0 ] == 0 ) && ( TimerArr[ 1 ] == 0 ) && ( TimerArr[ 2 ] == 0 ) )
                {
                    Timing_Flag = 1;
                    Timer_ReadTime( TimerArr, &Timing_Flag );
                }
                else if ( ( Result_1_Arr[ 0 ] == 0 ) && ( Result_1_Arr[ 1 ] == 0 ) && ( Result_1_Arr[ 2 ] == 0 ) )
                {
                    Result_1_Arr[ 0 ] = TimerArr[ 0 ];
                    Result_1_Arr[ 1 ] = TimerArr[ 1 ];
                    Result_1_Arr[ 2 ] = TimerArr[ 2 ];
                }
                else if ( ( Result_2_Arr[ 0 ] == 0 ) && ( Result_2_Arr[ 1 ] == 0 ) && ( Result_2_Arr[ 2 ] == 0 ) )
                {
                    Result_2_Arr[ 0 ] = TimerArr[ 0 ];
                    Result_2_Arr[ 1 ] = TimerArr[ 1 ];
                    Result_2_Arr[ 2 ] = TimerArr[ 2 ];
                }
                else
                {
                    /* 结束计时，固定第三个结果 */
                    Timing_Flag = 0;
                    Timer_ReadTime( TimerArr, &Timing_Flag );
                }
            }
            else if ( Timer_Key == KEY_DOWN_START_NUM )
            {
                TimerArr[ 0 ] = TimerArr[ 1 ] = TimerArr[ 2 ] = 0;
                Result_1_Arr[ 0 ] = Result_1_Arr[ 1 ] = Result_1_Arr[ 2 ] = 0;
                Result_2_Arr[ 0 ] = Result_2_Arr[ 1 ] = Result_2_Arr[ 2 ] = 0;
            }
            /* 嗯 */
            else if ( Timer_Key == KEY_LEFT_B_NUM )
            {
                vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
                vTaskDelete( NULL );                            // 删除秒表任务线程
            }
            vTaskDelay( 10 );
        }
    }
}


