/*
 * @Author: Jchen
 * @Date: 2024-10-25 14:42:20
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-26 16:24:17
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Inc\AlarmPage.h
 */
#ifndef __ALARMPAGE_H_
#define __ALARMPAGE_H_

#define ALARM_DEFAULT_DISPLAY_X             32
#define ALARM_DEFAULT_DISPLAY_Y             16

#define ALARM_HOUR_DEFAULT_DISPLAY_X        32
#define ALARM_HOUR_DEFAULT_DISPLAY_Y        16

#define ALARM_MIN_DEFAULT_DISPLAY_X         56
#define ALARM_MIN_DEFAULT_DISPLAY_Y         16

#define ALARM_SEC_DEFAULT_DISPLAY_X         80
#define ALARM_SEC_DEFAULT_DISPLAY_Y         16

#define ALARM_RECTANGLE_WIDE                16
#define ALARM_RECTANGLE_HIGH                16

/* 按钮 */
#define A_DISPLAY_X                         22
#define A_DISPLAY_Y                         38

#define B_DISPLAY_X                         19
#define B_DISPLAY_Y                         52

#define ALARM_RINGTONE_STK_SIZE             50

void Alarm_Task( void *params );


#endif /* __ALARMPAGE_H_ */
