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
 * @Date: 2024-09-27 19:49:40
 * @LastEditors: Jchen
 * @LastEditTime: 2024-09-27 19:52:39
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Games\Src\DinoGame.c
 */

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "MenuPage.h"
#include "driver_Key.h"
#include "OLED.h"
#include "music.h"

#include <stdlib.h>

extern TaskHandle_t xMenuPage_Handle;
extern TaskHandle_t xDinoGame_Handle;

/* 游戏全局参数 */
static uint8_t DinoGame_key_Num = 0;
static uint8_t DinoGame_Last_key_Num = 0;

static uint16_t Score = 1;
static uint16_t Highest_Score = 0;
static uint8_t Game_Speed = 30;
static uint8_t failed = 0;
static uint8_t cnt = 0;

/* 地面、植物速度 */
uint8_t Speed = 4;

/* 绘地面参数 */
uint16_t Ground_Length = 384;
static uint16_t Ground_Flag = 0;

/* 植物参数 */
static int16_t Cactus_Pos = 128;
static int16_t Cactus_Length = 8;
static uint8_t Cactus_Num = 0;

/* 云参数 */
static int16_t Cloud_Pos = 128;
int8_t Cloud_Speed = 1;
uint8_t Cloud_Length = 28;
static int8_t Cloud_Height = 0;

/* 跑动小恐龙 */
static uint8_t Dino_Cnt = 0;

/* 跳动小恐龙 */
int8_t Jump_Speed_Arr[] = { 1, 1, 3, 3, 4, 4, 5, 6, 7 };
static int8_t Speed_Idx = 9 - 1;
static int16_t height = 0;
static int8_t Jump_State = 0;

static void DinoGame_Start_Screen( void )
{
    OLED_Clear();
    OLED_ShowImage( 0, 0, 128, 64, COVER );
    OLED_Update();
}

/**
 * @description: 参数初始化
 * @return {*}
 * @Date: 2024-10-03 15:11:48
 * @Author: Jchen
 */
static void Parameter_Reset( void )
{
    DinoGame_key_Num = 0;
    DinoGame_Last_key_Num = 0;

    Score = 1;
    Game_Speed = 30;
    failed = 0;
    cnt = 0;

    Speed = 4;

    Ground_Length = 384;
    Ground_Flag = 0;

    Cactus_Pos = 128;
    Cactus_Length = 8;
    Cactus_Num = 0;

    Cloud_Pos = 128;
    Cloud_Speed = 1;
    Cloud_Length = 28;
    Cloud_Height = 0;

    Dino_Cnt = 0;

    Speed_Idx = 9 - 1;
    height = 0;
    Jump_State = 0;
}


/**
 * @description: 游戏主体
 * @param {void} *params
 * @return {*}
 * @Date: 2024-10-03 15:11:33
 * @Author: Jchen
 */
void DinoGame_Task( void *params )
{
    vTaskSuspend( xMenuPage_Handle );
    
    Parameter_Reset();

    DinoGame_Start_Screen();

#if 1   // 防止进入游戏连续读取按键A跳过封面显示
    DinoGame_Last_key_Num = KEY_LEFT_A_NUM;
    DinoGame_key_Num = Get_Key();

    while ( ( DinoGame_key_Num != KEY_LEFT_A_NUM ) || ( DinoGame_Last_key_Num == DinoGame_key_Num ) )
    {
        DinoGame_Last_key_Num = DinoGame_key_Num;
        DinoGame_key_Num = Get_Key();

        if ( DinoGame_key_Num == KEY_DOWN_START_NUM )
        {
            vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
            vTaskSuspend( NULL );                           // 暂停游戏任务线程
            DinoGame_key_Num = KEY_LEFT_A_NUM;
            DinoGame_Start_Screen();

        }
        else if ( DinoGame_key_Num == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
            vTaskDelete( NULL );                            // 删除游戏任务线程
        }
        vTaskDelay( 50 );
    }
#endif

    while (1)
    {
        OLED_Clear();
        /* 结束画面显示 */
        
        if (failed == 1)
		{   
			OLED_ShowImage( 0, 24, 39, 40, RestartDino );
            OLED_ShowString( 0, 1, "GAME OVER!", OLED_8X16 );
            OLED_ShowString( 45, 24, "High Score:", OLED_6X8 );
            OLED_ShowNum( 98, 32, Highest_Score, 5, OLED_6X8 );
            OLED_ShowString( 45, 45, "Now Score:", OLED_6X8 );
            OLED_ShowNum( 98, 53, Score, 5, OLED_6X8 );
            OLED_Update();

			DinoGame_key_Num = Get_Key();
			if ( DinoGame_key_Num == KEY_LEFT_A_NUM )
			{
				if(Score > Highest_Score)
                {
                    Highest_Score = Score;
                }
				Parameter_Reset();

				OLED_Clear();
                OLED_Update();
			}

            if ( DinoGame_key_Num == KEY_DOWN_START_NUM )
            {
                vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
                vTaskSuspend( NULL );                           // 暂停游戏任务线程
                DinoGame_key_Num = KEY_LEFT_A_NUM;

            }
            else if ( DinoGame_key_Num == KEY_DOWN_SET_NUM )
            {
                vTaskResume( xMenuPage_Handle );                // 恢复菜单任务线程
                vTaskDelete( NULL );                            // 删除游戏任务线程
            }
            vTaskDelay( 50 );
			continue;
		}
        
        cnt ++;
		
        if ( cnt == 6 )
        {
            Score ++;
            cnt = 0;
        }
        
		if (height <= 0)
        {
            DinoGame_Last_key_Num = DinoGame_key_Num;
            DinoGame_key_Num = Get_Key();
        }
#if 1  	// 绘制地面
        if ( Ground_Flag < ( Ground_Length - 128 - 4 ) )
        {
            OLED_ShowImage( 0, 56, 128, 8, &GROUND[ Ground_Flag ] );
            Ground_Flag += Speed;
        }
        else
        {
			OLED_ShowImage( 0, 56, 128, 8, &GROUND[ Ground_Flag ] );
            Ground_Flag = 0;
        }
#endif
#if 1	// 绘制云

        if (Cloud_Pos + Cloud_Length <= 0)
        {
            Cloud_Pos = 128;
        }
        if ( Cloud_Pos == 128 )
        {
            Cloud_Height = rand() % 16;
        }
        
        OLED_ShowString( 0, 0, "                ", OLED_8X16 );
        OLED_ShowString( 0, 8, "                ", OLED_8X16 );
        if ( Cloud_Pos < 0 )
        {
            OLED_ShowImage( 0, Cloud_Height, Cloud_Length + Cloud_Pos, 8, &CLOUD[ -Cloud_Pos ] );
        }
        else
        {
            OLED_ShowImage( Cloud_Pos, Cloud_Height, Cloud_Length, 8, CLOUD );
        }
        Cloud_Pos = Cloud_Pos - Cloud_Speed;
#endif

		if ( ( height>0 || DinoGame_key_Num == KEY_LEFT_A_NUM ) && ( DinoGame_Last_key_Num != DinoGame_key_Num ) )
        {
#if 1  // 跳跃小恐龙	
            if ( Jump_State == 0 )
            {
                height += Jump_Speed_Arr[ Speed_Idx ];
                Speed_Idx --;

                OLED_ShowImage( 16, 64 - 20 - height, 16, 16, DINO_JUMP[7] );

                if ( Speed_Idx <= 0 )
                {
                    Speed_Idx = 0;
                }
            }
            if ( Jump_State == 1 )
            {
                height -= Jump_Speed_Arr[ Speed_Idx ];
                Speed_Idx ++;

                OLED_ShowImage( 16, 64 - 20 - height, 16, 16, DINO_JUMP[7] );

                if ( Speed_Idx >= 9 - 1 )
                {
                    Speed_Idx = 9 - 1;
                }
            }
            if( height >= 31 )
            {
                Jump_State = 1;
                height = 31;
            }
            if(height <= 0)
            {
                Jump_State = 0;
                height = 0;
            }
#endif
        }
		else
        {
#if 1   // 绘制跑动小恐龙
            Dino_Cnt++;
            Dino_Cnt = Dino_Cnt % 2;
            
            if ( Dino_Cnt == 0 )
            {
                OLED_ShowImage( 16, 48, 16, 16 , DINO_Left );
                
            }
            else if ( Dino_Cnt == 1 )
            {
                OLED_ShowImage( 16, 48, 16, 16 , DINO_right );
            }
#endif
        }

#if 1   // 绘制随机仙人掌
        if ( Cactus_Pos == 128 )
        {
            Cactus_Num = rand() % 4;
        }
        
        if (Cactus_Num == 0) 
        {
            Cactus_Length = 8;
        }
        else if(Cactus_Num == 1)
        {
            Cactus_Length = 16;
        }
        else if (Cactus_Num == 2 || Cactus_Num == 3)
        {
            Cactus_Length = 24;
        }
        if ( Cactus_Pos < 0 )
        {
            if (Cactus_Num == 0)
            {
                OLED_ShowImage( 0, 48, Cactus_Pos + Cactus_Length, 16, &CACTUS_1[ -Cactus_Pos ] );
            }
            else if (Cactus_Num == 1)
            {
                OLED_ShowImage( 0, 48, Cactus_Pos + Cactus_Length, 16, &CACTUS_2[ -Cactus_Pos ] );
            }
            else if(Cactus_Num == 2)
            {
                OLED_ShowImage( 0, 48, Cactus_Pos + Cactus_Length, 16, &CACTUS_3[ -Cactus_Pos ] );
            }
            else
            {
                OLED_ShowImage( 0, 48, Cactus_Pos + Cactus_Length, 16, &CACTUS_4[ -Cactus_Pos ] );
            }
        }
        else
        {
            if (Cactus_Num == 0)
            {
                OLED_ShowImage( Cactus_Pos, 48, Cactus_Length, 16, CACTUS_1 );
            }
            else if (Cactus_Num == 1)
            {
                OLED_ShowImage( Cactus_Pos, 48, Cactus_Length, 16, CACTUS_2 );
            }
            else if(Cactus_Num == 2)
            {
                OLED_ShowImage( Cactus_Pos, 48, Cactus_Length, 16, CACTUS_3 );
            }
            else
            {
                OLED_ShowImage( Cactus_Pos, 48, Cactus_Length, 16, CACTUS_4 );
            }
        }
        Cactus_Pos = Cactus_Pos - Speed;
        if ( ( Cactus_Pos + Cactus_Length ) <= 0 )
        {
            Cactus_Pos = 128;
        }
#endif

        /* 失败条件 */
		if ((height < 16) && ( ( Cactus_Pos > 16 && Cactus_Pos < 32 ) || ( Cactus_Pos + Cactus_Length > 17 && Cactus_Pos + Cactus_Length < 31 ) ) )
		{
			failed = 1;
            vTaskDelay( 500 );
		}

		OLED_ShowString(35, 0, "HI:", OLED_6X8);
		OLED_ShowNum(58, 0, Highest_Score, 5, OLED_6X8);
		OLED_ShowNum(98, 0, Score, 5, OLED_6X8);
        
        OLED_Update();
        
		Game_Speed = Score/20;
		if ( Game_Speed > 29 )
        {
            Game_Speed = 29;
        }

        if ( DinoGame_key_Num == KEY_DOWN_START_NUM )
        {
            OLED_Clear();
            OLED_Update();
            vTaskResume( xMenuPage_Handle );                    // 恢复菜单任务线程
            vTaskSuspend( NULL );                               // 暂停游戏任务线程
            DinoGame_key_Num = KEY_DOWN_START_NUM;

        }
        else if ( DinoGame_key_Num == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xMenuPage_Handle );                    // 恢复菜单任务线程
            vTaskDelete( NULL );                                // 删除闹钟任务线程
        }
        vTaskDelay( 30 - Game_Speed );
    }
}

