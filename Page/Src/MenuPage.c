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
 * @Date: 2024-10-23 10:40:55
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-23 10:48:37
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Src\MenuPage.c
 */

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core

#include "MenuPage.h"
#include "OLED.h"
#include "driver_Key.h"
#include "driver_passive_buzzer.h"
#include "TimePage.h"
#include "TimerPage.h"
#include "AlarmPage.h"
#include "music.h"
#include "DinoGame.h"
#include "StarWar.h"
#include "InfoPage.h"

extern TaskHandle_t xHomePageTaskHandle;
extern TaskHandle_t xMenuPage_Handle;
extern TaskHandle_t xAlarm_Handle;
extern TaskHandle_t xDinoGame_Handle;
extern TaskHandle_t xStarWar_Handler;

TaskHandle_t xTimeSet_Handle;
TaskHandle_t xMusic_Handle;
TaskHandle_t xTimer_Handle;
TaskHandle_t TimeSet_Handle;
TaskHandle_t xInfoPage_Handle;

extern TaskStatus_t xAlarm_State;
extern TaskStatus_t xDinoGame_State;
extern TaskStatus_t xStarWar_State;

TaskStatus_t xMusic_State;


/* 全局参数 */
static uint8_t MenuPage_key = 0;
static uint8_t MenuPage_Last_key = 0;


/**
 * @description: 默认显示
 * @return {*}
 * @Date: 2024-10-24 21:04:26
 * @Author: Jchen
 */
static void Default_Display( uint8_t *Menu_Point )
{
    /* 标题 */
    OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

    switch ( *Menu_Point )
    {
        case 1:
            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y, MENU_1_TITLE_CONTENT, OLED_6X8 );
            OLED_ShowImage( OPTION_ICON_POSITION_X, OPTION_ICON_POSITION_Y, 31, 31, SetTime );
            break;

        case 2:
            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y, MENU_2_TITLE_CONTENT, OLED_6X8 );
            OLED_ShowImage( OPTION_ICON_POSITION_X, OPTION_ICON_POSITION_Y, 31, 31, Alarm );
            break;
        
        case 3:
            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y, MENU_3_TITLE_CONTENT, OLED_6X8 );
            OLED_ShowImage( OPTION_ICON_POSITION_X, OPTION_ICON_POSITION_Y, 31, 31, Music );
            break;
        
        case 4:
            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y, MENU_4_TITLE_CONTENT, OLED_6X8 );
            OLED_ShowImage( OPTION_ICON_POSITION_X, OPTION_ICON_POSITION_Y, 31, 31, Game );
            break;
        
        case 5:
            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y, MENU_5_TITLE_CONTENT, OLED_6X8 );
            OLED_ShowImage( OPTION_ICON_POSITION_X, OPTION_ICON_POSITION_Y, 31, 31, Stopwatch );
            break;
        
        case 6:
            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y, MENU_6_TITLE_CONTENT, OLED_6X8 );
            OLED_ShowImage( OPTION_ICON_POSITION_X, OPTION_ICON_POSITION_Y, 31, 31, setting );
            break;
		default:
			break;
    }
}


/**
 * @description: 菜单动画-右
 * @param {uint8_t} *Point
 * @return {*}
 * @Date: 2024-10-25 13:21:10
 * @Author: Jchen
 */
static void Menu_RightMove( uint8_t *Point )
{
    uint8_t i, j;

   if ( *Point == 2 )
   {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_2_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_ICON_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, SetTime );
            OLED_ShowImage( OPTION_NEXT_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Alarm );

            OLED_Update();
            vTaskDelay( 20 );
        }
   }
    else if ( *Point == 3 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_3_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_ICON_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Alarm );
            OLED_ShowImage( OPTION_NEXT_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Music );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
    else if ( *Point == 4 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_4_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_ICON_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Music );
            OLED_ShowImage( OPTION_NEXT_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Game );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
    else if ( *Point == 5 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_5_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_ICON_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Game );
            OLED_ShowImage( OPTION_NEXT_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Stopwatch );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
    else if ( *Point == 6 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_6_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_ICON_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, Stopwatch );
            OLED_ShowImage( OPTION_NEXT_POSITION_X - i, OPTION_ICON_POSITION_Y, 31, 31, setting );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
}


/**
 * @description: 菜单动画-左
 * @param {uint8_t} *Point
 * @return {*}
 * @Date: 2024-10-25 13:21:22
 * @Author: Jchen
 */
static void Menu_LefttMove( uint8_t *Point )
{
    uint8_t i, j;

   if ( *Point == 1 )
   {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_1_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_PREVIOUS_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, SetTime );
            OLED_ShowImage( OPTION_ICON_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Alarm );

            OLED_Update();
            vTaskDelay( 20 );
        }
   }
    else if ( *Point == 2 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_2_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_PREVIOUS_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Alarm );
            OLED_ShowImage( OPTION_ICON_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Music );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
    else if ( *Point == 3 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_3_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_PREVIOUS_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Music );
            OLED_ShowImage( OPTION_ICON_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Game );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
    else if ( *Point == 4 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_4_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_PREVIOUS_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Game );
            OLED_ShowImage( OPTION_ICON_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Stopwatch );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }
    else if ( *Point == 5 )
    {
        for ( i = 0, j = 0; i < 81; i += 8, j++ )
        {
            OLED_Clear();

            OLED_ShowString( MENU_TITLE_POSITION_X, MENU_TITLE_POSITION_Y, MENU_TITLE_CONTENT, OLED_8X16 );

            OLED_ShowString( OPTION_TITLE_POSITION_X, OPTION_TITLE_POSITION_Y + 10 - j, MENU_5_TITLE_CONTENT, OLED_6X8 );

            OLED_ShowImage( OPTION_PREVIOUS_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, Stopwatch );
            OLED_ShowImage( OPTION_ICON_POSITION_X + i, OPTION_ICON_POSITION_Y, 31, 31, setting );

            OLED_Update();
            vTaskDelay( 20 );
        }
    }

}


/**
 * @description: 菜单选项移动
 * @return {*}
 * @Date: 2024-10-25 13:40:42
 * @Author: Jchen
 */
static uint8_t Select_Option( void )
{
	static uint8_t Point = 1;
    if ( ( MenuPage_key == KEY_RIGHT_RIGHT_NUM ) && ( Point < 6 ) )
    {
        Point ++;
        Menu_RightMove( &Point );
    }
    else if ( ( MenuPage_key == KEY_RIGHT_LEFT_NUM ) && ( Point > 1 ) )
    {
        Point --;
        Menu_LefttMove( &Point );
    }
    
    if ( Point > 6 )
    {
        Point = 6;
    }

    return Point;
}


#if 1       // 游戏子菜单
/**
 * @description: 游戏子菜单默认显示
 * @return {*}
 * @Date: 2024-10-25 15:44:30
 * @Author: Jchen
 */
static void Default_Display_Game( void )
{
    /* 游戏图标 */
    OLED_ShowImage( GAME1_ICON_POSITION_X, GAME1_ICON_POSITION_Y, GAME_ICON_LENGTH, GAME_ICON_LENGTH, GAME1_ICON );
    OLED_ShowImage( GAME2_ICON_POSITION_X, GAME2_ICON_POSITION_Y, GAME_ICON_LENGTH, GAME_ICON_LENGTH, GAME2_ICON );

    /* 图标框显示 */
    OLED_DrawRectangle( SQUARE_1_POSITION_X, SQUARE_1_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH, OLED_UNFILLED );
    OLED_DrawRectangle( SQUARE_2_POSITION_X, SQUARE_2_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH, OLED_UNFILLED );
}


/**
 * @description: 选定游戏显示
 * @param {uint8_t} *GameMenu_Point
 * @return {*}
 * @Date: 2024-10-25 15:47:29
 * @Author: Jchen
 */
static void Select_Game( const uint8_t *GameMenu_Point )
{
    switch ( *GameMenu_Point )
    {
        case 1:
            OLED_ReverseArea( SQUARE_1_POSITION_X, SQUARE_1_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH );
            break;
        
        case 2:
            OLED_ReverseArea( SQUARE_2_POSITION_X, SQUARE_2_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH );
            break;

        default:
            break;
    }
}


/**
 * @description: 开启游戏
 * @param {uint8_t} *GameMenu_Point
 * @return {*}
 * @Date: 2024-10-25 16:37:18
 * @Author: Jchen
 */
static void OpenGame( const uint8_t *GameMenu_Point )
{
    if ( *GameMenu_Point == 1 )
    {
        vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );

        if ( xDinoGame_State.eCurrentState == eSuspended )
        {
            OLED_Clear();
            OLED_Update();
            vTaskResume( xDinoGame_Handle );
            vTaskSuspend( NULL );
         }
        else
        {
            xTaskCreate( DinoGame_Task, "DinoGameTask", DINOGAME_STK_SIZE, NULL, osPriorityNormal, &xDinoGame_Handle ); 
        }
    }
    else if ( *GameMenu_Point == 2 )
    {
        vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );

        if ( xStarWar_State.eCurrentState == eSuspended )
        {
            OLED_Clear();
            OLED_Update();
            vTaskResume( xStarWar_Handler );
            vTaskSuspend( NULL );
        }
        else
        {												 
            xTaskCreate( StarWar_Task, "StarWarTask", STARWAR_STK_SIZE, NULL, osPriorityNormal, &xStarWar_Handler ); 
        }
    }
}


/**
 * @description: 关闭运行游戏
 * @param {uint8_t} *GameMenu_Point
 * @return {*}
 * @Date: 2024-10-25 16:23:31
 * @Author: Jchen
 */
static void CloseGame( const uint8_t *GameMenu_Point )
{
    if ( *GameMenu_Point == 1 )
    {
        vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );

        if ( ( xDinoGame_State.eCurrentState == eSuspended ) || ( xDinoGame_State.eCurrentState == eBlocked ) )
            {
                vTaskDelete( xDinoGame_Handle );
                xDinoGame_Handle = NULL;
            }
    }
    else if ( *GameMenu_Point == 2 )
    {
        vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );

        if ( ( xStarWar_State.eCurrentState == eSuspended ) || ( xStarWar_State.eCurrentState == eBlocked ) )
        {
            vTaskDelete( xStarWar_Handler );
            xStarWar_Handler = NULL;
        }
    }
}

/**
 * @description: 游戏运行状态显示
 * @param {uint8_t} *GameMenu_Point
 * @return {*}
 * @Date: 2024-10-25 16:24:14
 * @Author: Jchen
 */
static void GameRunState( const uint8_t *GameMenu_Point )
{
    /* 获取任务状态 */
    vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );
    vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );

    /* 游戏运行状态显示 */
    if ( ( xDinoGame_State.eCurrentState == eSuspended ) || ( xDinoGame_State.eCurrentState == eBlocked ) )
    {
        OLED_DrawRectangle( GAME1_ICON_POSITION_X - 3, GAME1_ICON_POSITION_Y - 3, GAME_ICON_LENGTH + 6, GAME_ICON_LENGTH + 6, OLED_UNFILLED );

        if ( *GameMenu_Point == 1 )
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }

    if ( ( xStarWar_State.eCurrentState == eSuspended ) || ( xStarWar_State.eCurrentState == eBlocked ) )
    {
        OLED_DrawRectangle( GAME2_ICON_POSITION_X - 3, GAME2_ICON_POSITION_Y - 3, GAME_ICON_LENGTH + 6, GAME_ICON_LENGTH + 6, OLED_UNFILLED );

        if ( *GameMenu_Point == 2 )
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
}

/**
 * @description: 游戏子菜单
 * @return {*}
 * @Date: 2024-10-25 14:21:18
 * @Author: Jchen
 */
static void Open_GameMenu( void )
{
    uint8_t GameMenu_Key = KEY_LEFT_A_NUM;
    uint8_t GameMenu_Last_Key;
    uint8_t GameMenu_Point = 1;
    
    while ( 1 )
    {
        OLED_Clear();

        Default_Display_Game();

        /* 选定游戏显示 */
        Select_Game( &GameMenu_Point );

        /* 游戏运行状态显示 */
        GameRunState( &GameMenu_Point );

        /* 是否重复按键判断 */
        GameMenu_Last_Key = GameMenu_Key;
        GameMenu_Key = Get_KeyNum();
        if (  GameMenu_Last_Key != GameMenu_Key )
        {
             if ( ( ( GameMenu_Key == KEY_RIGHT_RIGHT_NUM ) || ( GameMenu_Key == KEY_RIGHT_LEFT_NUM ) ) && ( GameMenu_Point == 1 ) )
            {
                GameMenu_Point = 2;
            }
            else if ( ( ( GameMenu_Key == KEY_RIGHT_LEFT_NUM ) || ( GameMenu_Key == KEY_RIGHT_RIGHT_NUM ) ) && ( GameMenu_Point == 2 ) )
            {
                GameMenu_Point = 1;
            }

            /* 选择开启游戏任务 */
            else if ( GameMenu_Key == KEY_LEFT_A_NUM )
            {
                OpenGame( &GameMenu_Point );
            }
            
             /* 选择关闭游戏 */
            else if ( GameMenu_Key == KEY_DOWN_SET_NUM )
            {
                CloseGame( &GameMenu_Point );
            }
        }

        OLED_Update();

        if ( GameMenu_Key == KEY_LEFT_B_NUM )
        {
            break;
        }
        vTaskDelay( 50 );
    }
}
#endif

/**
 * @description: 设置子菜单
 * @return {*}
 * @Date: 2024-10-25 14:21:31
 * @Author: Jchen
 */
#if 0   // 没想好放什么
static void Open_SetMenu( void )
{
    uint8_t SetMenu_Key = KEY_LEFT_A_NUM;

    while ( 1 )
    {
        OLED_Clear();

        OLED_DrawRectangle( 0, 0, 128, 64, OLED_UNFILLED );
        OLED_ShowString( 13, 20, "I haven't figured", OLED_6X8 );
        OLED_ShowString( 1, 36, "out what to place yet", OLED_6X8 );

        /* 是否重复按键判断 */
        // SetMenu_Last_Key = SetMenu_Key;
        SetMenu_Key = Get_KeyNum();

        OLED_Update();
        if ( SetMenu_Key == KEY_LEFT_B_NUM )
        {
            break;
        }
        vTaskDelay( 50 );
    }
}
#endif

/**
 * @description: 开启选定任务 或者 子菜单
 * @param {uint8_t} *Menu_Point
 * @return {*}
 * @Date: 2024-10-25 16:33:56
 * @Author: Jchen
 */
static void OpenOption( const uint8_t *Menu_Point )
{
    if ( MenuPage_key == KEY_LEFT_A_NUM )
    {
        if ( *Menu_Point == 1 )
        {
            xTaskCreate( TimeSet_Task, "TimeSetTask", TIME_SET_STK_SIZE, NULL, osPriorityNormal, &xTimeSet_Handle );
        }
        else if ( *Menu_Point == 2 )
        {
            xTaskCreate( Alarm_Task, "AlarmTask", ALARM_SET_STK_SIZE, NULL, osPriorityNormal, &xAlarm_Handle );
        }
        else if ( *Menu_Point == 3 )
        {
            vTaskGetInfo( xMusic_Handle, &xMusic_State, pdFALSE, eInvalid );

            if ( xMusic_State.eCurrentState == eSuspended )
            {
                vTaskResume( xMusic_Handle );
            }
            else if ( xMusic_State.eCurrentState == eBlocked )
            {
                PassiveBuzzer_Control( 0 );
                vTaskSuspend( xMusic_Handle );
            }
            else 
            {												 
                xTaskCreate( PlayMusic, "PlayMusicTask", MUSIC_STK_SIZE, NULL, osPriorityNormal, &xMusic_Handle ); 
            }
        }
        else if ( *Menu_Point == 4 )
        {
            Open_GameMenu();
        }
        else if ( *Menu_Point == 5 )
        {
            xTaskCreate( Timer_Task, "TimerTask", TIMER_STK_SIZE, NULL, osPriorityNormal, &xTimer_Handle ); 
        }
        else if ( *Menu_Point == 6 )
        {
            // Open_SetMenu();
            xTaskCreate( Info_Task, "InfoTask", INFO_STK_SIZE, NULL, osPriorityNormal, &xInfoPage_Handle );
            
            xInfoPage_Handle = NULL;
        }
    }
    else if ( MenuPage_key == KEY_DOWN_SET_NUM )
    {
        if ( *Menu_Point == 3 )
        {
            PassiveBuzzer_Control( 0 );
            vTaskDelete( xMusic_Handle );
            xMusic_Handle = NULL;
        }
    }
}

/**
 * @description: 菜单主任务
 * @param {void} *param
 * @return {*}
 * @Date: 2024-10-25 16:33:10
 * @Author: Jchen
 */
void MenuPage_Task( void *param )
{
    vTaskSuspend( xHomePageTaskHandle );
    MenuPage_Last_key = KEY_LEFT_A_NUM;
    static uint8_t Menu_Point = 1;
  
    while ( 1 )
    {
        OLED_Clear();
        /* 显示内容 */
        Default_Display( &Menu_Point );

        OLED_Update();
        MenuPage_Last_key = MenuPage_key;
		MenuPage_key = Get_Key();
		if ( ( MenuPage_Last_key != MenuPage_key ) && ( MenuPage_key != Key_NONE ) )
        {
            /* 选择菜单显示 */
            Menu_Point = Select_Option();

            /* 选择打开选项 */
            OpenOption( &Menu_Point );

            if ( MenuPage_key == KEY_LEFT_B_NUM )
            {
                vTaskResume( xHomePageTaskHandle );                 // 恢复主页任务线程
                vTaskSuspend( NULL );                               // 暂停菜单任务线程
                MenuPage_key = KEY_LEFT_B_NUM;
            }
        }
        vTaskDelay( 100 );
    }  
}
