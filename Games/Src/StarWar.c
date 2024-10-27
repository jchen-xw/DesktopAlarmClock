/*
 * @Author: Jchen
 * @Date: 2024-09-27 22:37:57
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-25 16:03:51
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Games\Src\StarWar.c
 */

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "OLED.h"
#include "driver_Key.h"
#include "StarWar.h"
#include <stdlib.h>

extern TaskHandle_t xMenuPage_Handle;

/* 游戏全局参数 */
static uint8_t Level = 1;
static uint8_t  StarWar_Key = 0x00;
static uint8_t  StarWar_Last_Key = 0x00;
static uint8_t  Death_State = 0;
static uint16_t  Highest_Score = 0;
static uint16_t  Score = 0;

/* 背景参数 */
static uint16_t Starry_Flows_Pos = 0;

/* 飞机 */
static uint8_t  Plane_HP = 5;
static uint8_t  Plane_Pos = 24;
static uint8_t  Plane_Cnt = 0;

/* 导弹 */
static uint8_t Missile_Location_X = 0;
static uint8_t Missile_Location_Y = 0;

/* 敌人 */
static uint8_t Enemy = 0;
static uint8_t Enemy_HP = 10;
static uint8_t Enemy_Build_Cnt = 0;
static uint8_t Enemy_Center_Y = 12;
static uint8_t Enemy_Center_Last_Y = 12;

/* 敌人导弹 */
static uint8_t Enemy_Missile_Y = 12;
static uint8_t Enemy_Missile_Add_Y = 12;
static uint8_t Enemy_Missile_Cnt = 0;
static uint8_t Enemy_Missile_Add = 0;


static void StarWar_Start_Screen( void )
{
    OLED_Clear();
    OLED_ShowImage( 0, 0, 128, 64, StarWar_COVER );
    OLED_ShowString( 92, 56, STARWAR_VERSION_DISPLAY, OLED_6X8 );
    OLED_Update();
}


static void Parameter_Reset( void )
{
    Level = 1;
    StarWar_Key = 0x00;
    StarWar_Last_Key = 0x00;
    Death_State = 0;
    Score = 0;

    Starry_Flows_Pos = 0;

    Plane_HP = 5;
    Plane_Pos = 24;
    Plane_Cnt = 0;
    
    Missile_Location_X = 0;
    Missile_Location_Y = 0;
    
    Enemy = 0;
    Enemy_HP = 10;
    Enemy_Build_Cnt = 0;
    Enemy_Center_Y = 12;
    Enemy_Center_Last_Y = 12;
    
    Enemy_Missile_Y = 12;
    Enemy_Missile_Add_Y = 12;
    Enemy_Missile_Cnt = 0;
    Enemy_Missile_Add = 0;
}

void StarWar_Task( void *params )
{
    vTaskSuspend( xMenuPage_Handle );

	Parameter_Reset();

    StarWar_Start_Screen();
	
    
#if 1   // 防止进入游戏连续读取按键A跳过封面显示
    StarWar_Last_Key = KEY_LEFT_A_NUM;
    StarWar_Key = Get_KeyNum();

    while ( ( StarWar_Key != KEY_LEFT_A_NUM ) || ( StarWar_Last_Key == StarWar_Key ) )
    {
        StarWar_Last_Key = StarWar_Key;
        StarWar_Key = Get_KeyNum();

        if ( Get_KeyNum() == KEY_DOWN_START_NUM )
        {
            vTaskResume( xMenuPage_Handle );            // 恢复菜单任务线程
            vTaskSuspend( NULL );                       // 暂停游戏任务线程
            StarWar_Key = KEY_LEFT_A_NUM;
            StarWar_Start_Screen();
        }
        else if ( Get_KeyNum() == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xMenuPage_Handle );            // 恢复菜单任务线程
            vTaskDelete( NULL );                        // 删除游戏任务线程
        }
        vTaskDelay( 50 );
    }
#endif

	while (1)
	{
		 OLED_Clear();

        /* 结算画面 */
        if ( Death_State == 1 )
        {
            OLED_ShowImage( 1, 10, 51, 40, GameOver );
            OLED_ShowString( 54, 24, "High Score:", OLED_6X8 );
            OLED_ShowNum( 104, 32, Highest_Score, 4, OLED_6X8 );
            OLED_ShowString( 54, 44, "Now Score:", OLED_6X8 );
            OLED_ShowNum( 104, 52, Score, 4, OLED_6X8 );
            OLED_Update();

			StarWar_Key = Get_KeyNum();
			if (StarWar_Key == KEY_LEFT_A_NUM)
			{
				if( Score > Highest_Score )
                {
                    Highest_Score = Score;
                }
                Parameter_Reset();

				OLED_Clear();
                OLED_Update();
			}
            else if ( StarWar_Key == KEY_DOWN_START_NUM )
            {
                vTaskResume( xMenuPage_Handle );            // 恢复菜单任务线程
                vTaskSuspend( NULL );                       // 暂停游戏任务线程
                StarWar_Key = KEY_LEFT_A_NUM;

            }
            else if ( StarWar_Key == KEY_DOWN_SET_NUM )
            {
                vTaskResume( xMenuPage_Handle );            // 恢复菜单任务线程
                vTaskDelete( NULL );                        // 删除游戏任务线程
            }
            vTaskDelay( 50 );
			continue;
        }
        
#if 1   // 流转星空背景
        OLED_DrawPoint( ( 5   - Starry_Flows_Pos + 128 ),  5 );
        OLED_DrawPoint( ( 8   - Starry_Flows_Pos + 128 ),  7 );
        OLED_DrawPoint( ( 10  - Starry_Flows_Pos + 128 ), 56 );
        OLED_DrawPoint( ( 14  - Starry_Flows_Pos + 128 ), 49 );
        OLED_DrawPoint( ( 24  - Starry_Flows_Pos + 128 ), 24 );
        OLED_DrawPoint( ( 25  - Starry_Flows_Pos + 128 ), 43 );
        OLED_DrawPoint( ( 50  - Starry_Flows_Pos + 128 ), 30 );
        OLED_DrawPoint( ( 55  - Starry_Flows_Pos + 128 ), 16 );
        OLED_DrawPoint( ( 70  - Starry_Flows_Pos + 128 ), 28 );
        OLED_DrawPoint( ( 78  - Starry_Flows_Pos + 128 ), 36 );
        OLED_DrawPoint( ( 70  - Starry_Flows_Pos + 128 ), 25 );
        OLED_DrawPoint( ( 80  - Starry_Flows_Pos + 128 ), 62 );
        OLED_DrawPoint( ( 88  - Starry_Flows_Pos + 128 ), 57 );
        OLED_DrawPoint( ( 100 - Starry_Flows_Pos + 128 ), 43 );
        OLED_DrawPoint( ( 107 - Starry_Flows_Pos + 128 ), 11 );
        OLED_DrawPoint( ( 117 - Starry_Flows_Pos + 128 ), 52 );
        OLED_DrawPoint( ( 138 - Starry_Flows_Pos + 128 ), 11 );
        OLED_DrawPoint( ( 150 - Starry_Flows_Pos + 128 ),  6 );
        OLED_DrawPoint( ( 163 - Starry_Flows_Pos + 128 ), 56 );
        OLED_DrawPoint( ( 186 - Starry_Flows_Pos + 128 ), 12 );
        OLED_DrawPoint( ( 189 - Starry_Flows_Pos + 128 ), 44 );
        OLED_DrawPoint( ( 198 - Starry_Flows_Pos + 128 ), 25 );
        OLED_DrawPoint( ( 200 - Starry_Flows_Pos + 128 ), 32 );
        OLED_DrawPoint( ( 206 - Starry_Flows_Pos + 128 ), 60 );
        OLED_DrawPoint( ( 215 - Starry_Flows_Pos + 128 ), 18 );
        OLED_DrawPoint( ( 234 - Starry_Flows_Pos + 128 ), 56 );

        Starry_Flows_Pos += 2;
        if ( Starry_Flows_Pos > 254 )
        {
            Starry_Flows_Pos = 0;
        }
#endif

#if 1   // 信息显示
        OLED_ShowString( 28, 1, "HP:", OLED_6X8 );
        OLED_DrawRectangle( 46, 1, 32, 8, OLED_UNFILLED );
        if ( Plane_HP == 5 )
        {
            OLED_DrawRectangle( 47, 2, 30, 6, OLED_FILLED );
        }
        else if ( Plane_HP == 4 )
        {
            OLED_DrawRectangle( 47, 2, 24, 6, OLED_FILLED );
        }
        else if ( Plane_HP == 3 )
        {
            OLED_DrawRectangle( 47, 2, 18, 6, OLED_FILLED );
        }
        else if ( Plane_HP == 2 )
        {
            OLED_DrawRectangle( 47, 2, 12, 6, OLED_FILLED );
        }
        else if ( Plane_HP == 1 )
        {
            OLED_DrawRectangle( 47, 2, 6, 6, OLED_FILLED );
        }
        OLED_ShowString( 28, 54, "Score:", OLED_6X8 );
        OLED_ShowNum( 64, 54, Score, 3, OLED_6X8 );

        OLED_ShowString( 110, 54, "L:", OLED_6X8 );
        OLED_ShowNum( 122, 54, Level, 1, OLED_6X8 );

        /* 敌人血量显示 */
        OLED_ShowNum( 116, 0, Enemy_HP, 2, OLED_6X8 );
#endif

#if 1   // 绘制敌人导弹

        if ( Enemy == 1 )
        {
            if ( Enemy_Missile_Cnt == 0 )
            {
                Enemy_Missile_Y = Enemy_Center_Y;
            }
            if ( Enemy_Missile_Cnt == 42 )
            {
                Enemy_Missile_Add_Y = Enemy_Center_Y;
                Enemy_Missile_Add = 2;
            }
            
            if ( Level < 4 )
            {
                Enemy_Missile_Cnt += 6;
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X - Enemy_Missile_Cnt , Enemy_Missile_Y + 3, 2, OLED_FILLED );

                if ( Enemy_Missile_Cnt >= 95 )
                {
                    Enemy_Missile_Cnt = 0;
                }
            } 
            
            if ( Level > 3 )
            {
                Enemy_Missile_Cnt += 6;
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X - Enemy_Missile_Cnt , Enemy_Missile_Y + 3, 2, OLED_FILLED );
                
                if ( Enemy_Missile_Add != 0 )
                {
                   OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X - Enemy_Missile_Add , Enemy_Missile_Add_Y + 3, 3, OLED_FILLED );
                   Enemy_Missile_Add += 6;
                }
                

                if ( Enemy_Missile_Cnt >= 95 )
                {
                    Enemy_Missile_Cnt = 0;
                }
                if ( Enemy_Missile_Add >= 95 )
                {
                    Enemy_Missile_Add = 0;
                }
            }
            // 敌人导弹和飞机碰撞
            if ( ( ( ENEMY_CENTER_DISPLAY_X - Enemy_Missile_Cnt ) <= ( 4 + 16 ) ) && ( ( Enemy_Missile_Y+ 2 ) > Plane_Pos ) && ( ( Enemy_Missile_Y - 2 ) < ( Plane_Pos + 16 ) ) )
            {
                Plane_HP -= 1;
                Enemy_Missile_Cnt = 0;
            }
            if ( ( ( ENEMY_CENTER_DISPLAY_X - Enemy_Missile_Add ) <= ( 4 + 16 ) ) && ( ( Enemy_Missile_Add_Y + 3 ) > Plane_Pos ) && ( ( Enemy_Missile_Add_Y - 3 ) < ( Plane_Pos + 16 ) ) )
            {
                Plane_HP -= 1;
                Enemy_Missile_Add = 0;
            }
        }    
#endif
#if 1   // 绘制敌人
        /* 敌人生成 */
        if ( Enemy == 0 )
        {
            if ( Enemy_Build_Cnt < 12 )
            {
                Enemy_Build_Cnt ++;
            }
            if ( Enemy_Build_Cnt == 5 )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 30, Enemy_Center_Y, 2, OLED_UNFILLED );
            }
            else if ( Enemy_Build_Cnt == 6  )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 25, Enemy_Center_Y, 3, OLED_UNFILLED );
            }
            else if ( Enemy_Build_Cnt == 7  )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 20, Enemy_Center_Y, 4, OLED_UNFILLED );
            }
            else if ( Enemy_Build_Cnt == 8  )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 16, Enemy_Center_Y, 6, OLED_UNFILLED );
            }
            else if ( Enemy_Build_Cnt == 9 )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 12, Enemy_Center_Y, 8, OLED_UNFILLED );
            }
            else if ( Enemy_Build_Cnt == 10 )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 8, Enemy_Center_Y, 10, OLED_UNFILLED );
            }
            else if ( Enemy_Build_Cnt == 11 )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 4, Enemy_Center_Y, 11, OLED_UNFILLED );
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 4, Enemy_Center_Y + 3, 2, OLED_FILLED );
            }
            else if ( Enemy_Build_Cnt == 12 )
            {
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X, Enemy_Center_Y, 12, OLED_UNFILLED );
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 2, Enemy_Center_Y + 3, 3, OLED_FILLED );
                Enemy = 1;
                if ( Level == 1 )
                {
                    Enemy_HP = 10;
                }
                else if ( ( Level == 2 ) )
                {
                    Enemy_HP = 14;
                }
                else if ( ( Level == 3 ) )
                {
                    Enemy_HP = 20;
                }
                else if ( Level == 4 )
                {
                    Enemy_HP = 30;
                }
                else if ( Level == 5 )
                {
                    Enemy_HP = 46;
                }
                Enemy_Build_Cnt = 0;
            }
        }
#endif
#if 1   // 敌人移动
        else if ( Enemy == 1 )
        {
            if ( ( (Enemy_Center_Y > 12 ) && ( Enemy_Center_Last_Y > Enemy_Center_Y ) ) || ( Enemy_Center_Y >= 51 ) )
            {
                Enemy_Center_Last_Y = Enemy_Center_Y;
                if ( Level < 4 )
                {
                    Enemy_Center_Y -= Level;
                }
                else
                {
                    Enemy_Center_Y -= 3;
                }
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X, Enemy_Center_Y, 12, OLED_UNFILLED );
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 2, Enemy_Center_Y + 3, 3, OLED_FILLED );
            }
            else if ( ( ( Enemy_Center_Y < 51) && ( Enemy_Center_Last_Y < Enemy_Center_Y ) ) || ( Enemy_Center_Y <= 12 ) )
            {
                Enemy_Center_Last_Y = Enemy_Center_Y;
                if ( Level < 4 )
                {
                    Enemy_Center_Y += Level;
                }
                else
                {
                    Enemy_Center_Y += 3;
                }
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X, Enemy_Center_Y, 12, OLED_UNFILLED );
                OLED_DrawCircle( ENEMY_CENTER_DISPLAY_X + 2, Enemy_Center_Y + 3, 3, OLED_FILLED );
            } 
        }
#endif
#if 1   // 绘制导弹
        StarWar_Key = Get_KeyNum();
        if ( ( StarWar_Key == KEY_LEFT_A_NUM ) && ( Missile_Location_X == 0  ) )
        {
            Missile_Location_X = 2;
            Missile_Location_Y = Plane_Pos + 7;
        }
        if ( Missile_Location_X != 0 )
        {
            OLED_DrawLine( ( MISSILE_DEFAULT_POSITION_X + Missile_Location_X ), ( Missile_Location_Y + 0 ), ( MISSILE_DEFAULT_POSITION_X + Missile_Location_X + MISSILE_DEFAULT_LENGTH ), Missile_Location_Y + 0 );
            OLED_DrawLine( ( MISSILE_DEFAULT_POSITION_X + Missile_Location_X ), ( Missile_Location_Y + 1 ), ( MISSILE_DEFAULT_POSITION_X + Missile_Location_X + MISSILE_DEFAULT_LENGTH ), Missile_Location_Y + 1 );

            Missile_Location_X = Missile_Location_X + 8;
            if ( Missile_Location_X > 121 )
            {
                Missile_Location_X = 0;
            }
            // 导弹和敌人碰撞
            if ( ( Enemy == 1 )
            && ( ( MISSILE_DEFAULT_POSITION_X + Missile_Location_X ) > ( ENEMY_CENTER_DISPLAY_X - 6 ) ) 
            && ( ( Missile_Location_Y ) > ( Enemy_Center_Y - 12 ) )
            && ( ( Missile_Location_Y + 1 ) < ( Enemy_Center_Y + 12 ) ) )
            {
                Enemy_HP -= 1;
                Score += 1;
                Missile_Location_X = 0;
            }
        }     
#endif
#if 1   // 绘制飞机
        /* 飞机显示 */
        if ( ( Plane_Cnt < 3 ) && ( StarWar_Key == KEY_RIGHT_UP_NUM ) )
        {
            Plane_Cnt ++;
            if ( Plane_Pos > 0 )
            {
                Plane_Pos -= 4;
            }
        }
        else if ( ( Plane_Cnt < 3 ) && ( StarWar_Key == KEY_RIGHT_DOWN_NUM ) )
        {
            Plane_Cnt ++;
            if ( Plane_Pos < 48 )
            {
                Plane_Pos += 4;
            }
        }
        if ( Plane_Cnt > 2 )
        {
            Plane_Cnt = 0;
        }
        OLED_ShowImage( 4, Plane_Pos, 16, 16, dioda16 );
#endif
        if ( Score == 20 )
        {
            Level = 2;
        }
        else if ( Score == 62 )
        {
            Level = 3;
        }
        else if ( Score == 162 )
        {
            Level = 4;
        }
        else if ( Score == 402 )
        {
            Level = 5;
        }

        if ( Enemy_HP == 0 )
        {
            Enemy = 0;
        }

		// 结束条件
        if ( Plane_HP == 0 )
        {
            Death_State = 1;
        }
    
        OLED_Update();

        vTaskDelay( 50 );
#if 1   // 嗯
        if ( StarWar_Key == KEY_DOWN_START_NUM )
        {
            vTaskResume( xMenuPage_Handle );            // 恢复菜单任务线程
            vTaskSuspend( NULL );                       // 暂停游戏任务线程
            StarWar_Key = KEY_LEFT_A_NUM;

        }
        else if ( StarWar_Key == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xMenuPage_Handle );            // 恢复菜单任务线程
            vTaskDelete( NULL );                        // 删除游戏任务线程
        }
#endif
	}
}
