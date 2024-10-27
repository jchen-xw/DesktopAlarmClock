/*
 * @Author: Jchen
 * @Date: 2024-10-23 10:40:41
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-27 00:09:13
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Src\HomePage.c
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

#include "OLED.h"
#include "HomePage.h"
#include "Time.h"
#include "driver_dht11.h"
#include "driver_Key.h"
#include "MenuPage.h"
#include "AlarmPage.h"


//static TimerHandle_t g_TimerDHT11;

TaskHandle_t xMenuPage_Handle;
TaskHandle_t xAlarm_Handle;
TaskHandle_t xDinoGame_Handle;
TaskHandle_t xStarWar_Handler;

TaskStatus_t xMenuPage_State;
TaskStatus_t xAlarm_State;
TaskStatus_t xDinoGame_State;
TaskStatus_t xStarWar_State;

extern TaskHandle_t xAlarmRingtone_Handle;
extern TaskHandle_t xMusic_Handle;

extern TaskStatus_t xAlarmRingtone_State;
extern TaskStatus_t xMusic_State;


void HomePage_Init( void )
{
    OLED_Init();
}


/**
 * @description: 读取温湿度数据
 * @param {uint8_t} *humi
 * @param {uint8_t} *temp
 * @return {*}
 * @Date: 2024-10-24 14:32:56
 * @Author: Jchen
 */
static void DHT11AndLightSenser_Display( uint8_t *humi, uint8_t *temp )
{
	int8_t err;
    /* 读取DHT11的温湿度 */
    /* 暂停调度器 */
    vTaskSuspendAll();
    err = DHT11_Read( humi, temp );
    /* 恢复调度器 */
    xTaskResumeAll();
	
	if( err == -1 )
	{
		*temp = 0;
		*humi = 0;
	}
}

#if 1
/**
 * @description: 任务运行状态显示
 * @return {*}
 * @Date: 2024-10-24 14:34:08
 * @Author: Jchen
 */
void TaskRunState( void )
{
    /* 获取任务状态 */
    vTaskGetInfo( xAlarm_Handle, &xAlarm_State, pdFALSE, eInvalid );
    vTaskGetInfo( xMusic_Handle, &xMusic_State, pdFALSE, eInvalid );
    vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );
    vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );

    /* 任务运行状态显示 */
    if ( ( xDinoGame_State.eCurrentState == eSuspended ) || ( xDinoGame_State.eCurrentState == eBlocked ) )
    {
        OLED_ShowImage( GAME1_RUN_ICON_POSITION_X, GAME1_RUN_ICON_POSITION_Y, 16, 16, DINO_JUMP[7] );
    }
    if ( ( xStarWar_State.eCurrentState == eSuspended ) || ( xStarWar_State.eCurrentState == eBlocked ) )
    {
        OLED_ShowImage( GAME2_RUN_ICON_POSITION_X, GAME2_RUN_ICON_POSITION_Y, 16, 16, dioda16 );
    }
    if ( ( xAlarm_State.eCurrentState == eSuspended ) || ( xAlarm_State.eCurrentState == eBlocked ) )
    {
        OLED_ShowImage( ALARM_RUN_ICON_POSITION_X, ALARM_RUN_ICON_POSITION_Y, 16, 16, AlarmRun );
    }
    if ( ( xMusic_State.eCurrentState == eBlocked ) || ( xMusic_State.eCurrentState == eSuspended ) )
    {
        OLED_ShowImage( MUSIC_RUN_ICON_POSITION_X, MUSIC_RUN_ICON_POSITION_Y, 16, 16, MusicRun );
    }

}
#endif

/**
 * @description: 时间显示
 * @return {*}
 * @Date: 2024-10-23 16:58:02
 * @Author: Jchen
 */
void TimeDisplay( void )
{
    uint8_t TimeArr[ 3 ];
    static uint8_t count = 1;

    /* 显示时间冒分割号 */
    if ( count == 1 )
    {
        OLED_ShowString( POSITION_TIME_X, POSITION_TIME_Y, "  :  :  ", OLED_8X16 );
        count = 0;
    }
    else
    {
        OLED_ShowString( POSITION_TIME_X, POSITION_TIME_Y, "        ", OLED_8X16 );
        count ++;
    }

    ReadTime( TimeArr );
    /* 显示时间数字 */
    OLED_ShowNum( POSITION_HOUR_X, POSITION_HOUR_Y, TimeArr[ 0 ], POSITION_TIME_LEN, OLED_8X16 );
    OLED_ShowNum( POSITION_MIN_X, POSITION_MIN_Y, TimeArr[ 1 ], POSITION_TIME_LEN, OLED_8X16 );
    OLED_ShowNum( POSITION_SEC_X, POSITION_SEC_Y, TimeArr[ 2 ], POSITION_TIME_LEN, OLED_8X16 );
}

/**
 * @description: 显示温湿度、光照度数据
 * @param {uint8_t} *temp
 * @param {uint8_t} *humi
 * @param {uint8_t} *LightIntensity
 * @return {*}
 * @Date: 2024-10-24 10:52:08
 * @Author: Jchen
 */
static void Data_Show( uint8_t *temp, uint8_t *humi )
{
    OLED_ShowString( TEMP_HUMI_LOGO_POSITION_X, TEMP_HUMI_LOGO_POSITION_Y, " T:00C   H:000% ", OLED_8X16 );
    
    OLED_ShowNum( TEMP_NUM_POSITION_X, TEMP_NUM_POSITION_Y, *humi, TEMP_NUM_LEN, OLED_8X16 );
    OLED_ShowNum( HUMI_NUM_POSITION_X, HUMI_NUM_POSITION_Y, *temp, HUMI_NUM_LEN, OLED_8X16 );
    
}


void HomePage_Task( void *params )
{
    uint8_t temp, humi;
    uint8_t count = 5;
    uint8_t Home_Key, Home_Last_Key;

    DHT11_Init();

    /* 创建定时器 */
    //g_TimerDHT11 = xTimerCreate( "DHT11AndLightSenser_timer", 2000, pdTRUE, NULL, DHT11AndLightSenserTimer_Func );
    /* 启动定时器 */
    //xTimerStart( g_TimerDHT11, portMAX_DELAY );

    while ( 1 )
    {
        OLED_Clear();

        /* 信息显示 */
        TimeDisplay();

        /* 任务运行状态显示 */
        TaskRunState();

        /* 每5秒更新一次温湿度数据 */
        if ( count == 5 )
        {
            count = 0;
            DHT11AndLightSenser_Display( &temp, &humi );
        }
        
		/* 显示数据 */
        Data_Show( &temp, &humi );
        
        count ++;
        OLED_Update();

        /* 打开菜单 */
        Home_Last_Key = Home_Key;
        Home_Key = Get_KeyNum();

        if ( Home_Last_Key != Home_Key )
        {
            if ( Home_Key == KEY_LEFT_A_NUM )
            {
                vTaskGetInfo( xMenuPage_Handle, &xMenuPage_State, pdFALSE, eInvalid );
                
                if ( xMenuPage_State.eCurrentState == eSuspended )
                {
                    OLED_Clear();
                    OLED_Update();
                    vTaskResume( xMenuPage_Handle );
                    vTaskSuspend( NULL );
                }
                else
                {
                    xTaskCreate( MenuPage_Task, "MenuPageTask", MENU_STK_SIZE, NULL, osPriorityNormal, &xMenuPage_Handle ); 
                }
            }
        }
        vTaskDelay( 500 );
    }
    
}





