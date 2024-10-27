/*
 * @Author: Jchen
 * @Date: 2024-10-27 16:58:12
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-27 17:15:01
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Src\InfoPage.c
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

#include "driver_Key.h"
#include "OLED.h"
#include "MenuPage.h"

extern TaskHandle_t xMenuPage_Handle;


void Info_Task( void *params )
{
    vTaskSuspend( xMenuPage_Handle );

    uint8_t Info_Key_Num = 0;

    OLED_Clear();
    OLED_ShowString( 19, 9, "---------------", OLED_6X8 );
    OLED_ShowString( 40, 17, "FreeRTOS", OLED_6X8 );
    OLED_ShowString( 31, 25, "Alarm Clock", OLED_6X8 );
    OLED_ShowString( 43, 36, "Author:", OLED_6X8 );
    OLED_ShowString( 49, 44, "ZSJTF", OLED_6X8 );
    OLED_ShowString( 19, 52, "--------------", OLED_6X8 );

    OLED_DrawRectangle( 0, 0, 128, 64, OLED_UNFILLED );
    OLED_DrawCircle( 0, 0, 6, OLED_UNFILLED );
    OLED_DrawCircle( 127, 0, 6, OLED_UNFILLED );
    OLED_DrawCircle( 0, 64, 6, OLED_UNFILLED );
    OLED_DrawCircle( 127, 64, 6, OLED_UNFILLED );

    OLED_Update();

    while ( 1 )
    {
        Info_Key_Num = Get_KeyNum();
        if ( Info_Key_Num == KEY_LEFT_B_NUM )
        {
            vTaskResume( xMenuPage_Handle );
            vTaskDelete( NULL );
        }
		vTaskDelay( 50 );
    }
}

