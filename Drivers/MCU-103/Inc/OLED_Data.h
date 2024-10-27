/*
 * @Author: Jchen
 * @Date: 2024-09-26 20:39:16
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-24 20:30:40
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Drivers\MCU-103\Inc\OLED_Data.h
 */
/*
 * @Author: Jchen
 * @Date: 2024-09-26 20:39:16
 * @LastEditors: Jchen
 * @LastEditTime: 2024-09-30 20:50:52
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Drivers\MCU-103\Inc\OLED_Data.h
 */
#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			3		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
	uint8_t Data[32];						//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/

extern const uint8_t Dino_Game[];               // 游戏图标：谷歌小恐龙，18*18像素
extern const uint8_t Star_War[];                // 游戏图标：星球大战，18*18像素

extern const uint8_t SetTime[];                 // 时间设置图标：31*31像素
extern const uint8_t Alarm[];                   // 闹钟显示图标：31*31像素
extern const uint8_t Music[];                   // 音乐显示图标：31*31像素
extern const uint8_t Game[];                    // 游戏显示图标：31*31像素
extern const uint8_t Stopwatch[];               // 秒表显示图标：31*31像素
extern const uint8_t setting[];                 // 设置显示图标：31*31像素

extern const uint8_t AlarmRun[];                // 闹钟运行图标：16*16像素
extern const uint8_t MusicRun[];                // 音乐运行图标：16*16像素

/*********Dino Game Dispaly function start**************************/
extern const uint8_t GROUND[];
extern const uint8_t CLOUD[];
extern const uint8_t DINO_Left[];
extern const uint8_t DINO_right[];
extern const uint8_t DINO_JUMP[8][48];
extern const uint8_t CACTUS_1[];
extern const uint8_t CACTUS_2[];
extern const uint8_t CACTUS_3[];
extern const uint8_t CACTUS_4[];
extern const uint8_t RESTART[];
extern const uint8_t COVER[];
extern const uint8_t RestartDino[];
/**********Dino Game Dispaly function over**************************/

/**********Star War Dispaly function start**************************/
extern const uint8_t dioda16[32];
extern const uint8_t StarWar_COVER[];
extern const uint8_t GameOver[];
/**********Star War Dispaly function over***************************/




#endif


