/*
 * @Author: Jchen
 * @Date: 2024-10-23 10:41:22
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-27 10:14:49
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Inc\HomePage.h
 */
#ifndef __HOMEPAGE_H_
#define __HOMEPAGE_H_

/* 时间显示 */
#define POSITION_TIME_X                     32
#define POSITION_TIME_Y                     0

#define POSITION_HOUR_X                     32
#define POSITION_HOUR_Y                     0

#define POSITION_MIN_X                      56
#define POSITION_MIN_Y                      0

#define POSITION_SEC_X                      80
#define POSITION_SEC_Y                      0

#define POSITION_TIME_LEN                   2

/* 温湿度 */
#define TEMP_HUMI_LOGO_POSITION_X           0
#define TEMP_HUMI_LOGO_POSITION_Y           16

#define TEMP_NUM_POSITION_X                 24
#define TEMP_NUM_POSITION_Y                 16
#define TEMP_NUM_LEN                        2

#define HUMI_NUM_POSITION_X                 88
#define HUMI_NUM_POSITION_Y                 16
#define HUMI_NUM_LEN                        3

/* 运行任务显示 */
/* Dimo Game */
#define GAME1_RUN_ICON_POSITION_X           94
#define GAME1_RUN_ICON_POSITION_Y           48

/* Star War */
#define GAME2_RUN_ICON_POSITION_X           112
#define GAME2_RUN_ICON_POSITION_Y           48

/* Alarm */
#define ALARM_RUN_ICON_POSITION_X           0
#define ALARM_RUN_ICON_POSITION_Y           48

/* Music */
#define MUSIC_RUN_ICON_POSITION_X           18
#define MUSIC_RUN_ICON_POSITION_Y           48

/* 任务栈 */
#define MENU_STK_SIZE                       60






void HomePage_Init( void );
void HomePage_Task( void *params );

#endif /* __HOMEPAGE_H_ */
