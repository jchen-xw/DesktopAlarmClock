/*
 * @Author: Jchen
 * @Date: 2024-10-23 13:45:26
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-26 23:41:33
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Clock\Inc\Time.h
 */
#ifndef __TIME_H_
#define __TIME_H_

void DateAndTime_Init( void );

void Get_Time( void );
void ReadTime( uint8_t *TimeArr );
void Timer_ReadTime( uint8_t *TimerArr, uint8_t *Timing_Flag );
void Write_Time( const uint8_t *Write_TimeArr );


#endif /* __TIME_H_ */
