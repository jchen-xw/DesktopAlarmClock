/*
 * @Author: Jchen
 * @Date: 2024-10-25 13:59:40
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-25 23:43:09
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Inc\TimePage.h
 */
#ifndef __TIMEPAGE_H_
#define __TIMEPAGE_H_

#define TIME_DEFAULT_DISPLAY_X              32
#define TIME_DEFAULT_DISPLAY_Y              24

#define TIME_HOUR_DEFAULT_DISPLAY_X         32
#define TIME_HOUR_DEFAULT_DISPLAY_Y         24

#define TIME_MIN_DEFAULT_DISPLAY_X          56
#define TIME_MIN_DEFAULT_DISPLAY_Y          24

#define TIME_SEC_DEFAULT_DISPLAY_X          80
#define TIME_SEC_DEFAULT_DISPLAY_Y          24


#define TIME_RECTANGLE_WIDE                 16
#define TIME_RECTANGLE_HIGH                 16

void TimeSet_Task( void *params );


#endif /* __TIMEPAGE_H_ */
