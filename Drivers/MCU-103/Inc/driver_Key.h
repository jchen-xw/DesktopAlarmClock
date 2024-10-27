/*
 * @Author: Jchen
 * @Date: 2024-09-27 14:40:09
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-24 21:11:07
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Drivers\MCU-103\Inc\driver_Key.h
 */
#ifndef __DRIVER_KEY_H_
#define __DRIVER_KEY_H_

#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"

#define Key_QUEUE_LEN                   10
#define BUF_LEN                         2

/*  KEY_按键位置_按键_NUM  */
#define KEY_RIGHT_UP_NUM                0x40
#define KEY_RIGHT_DOWN_NUM              0x41
#define KEY_RIGHT_RIGHT_NUM             0x42
#define KEY_RIGHT_LEFT_NUM              0x43
#define KEY_RIGHT_MID_NUM               0x44

#define KEY_LEFT_A_NUM                  0x45
#define KEY_LEFT_B_NUM                  0x46
#define KEY_LEFT_X_NUM                  0x47
#define KEY_LEFT_Y_NUM                  0x48

#define KEY_DOWN_START_NUM              0x49
#define KEY_DOWN_SET_NUM                0x50

#define KEY_LB_NUM                      0x51
#define KEY_LT_NUM                      0x52
#define KEY_RB_NUM                      0x53
#define KEY_RT_NUM                      0x54

#define Key_NONE	                    0x00

// QueueHandle_t GetQueueKey(void);

uint8_t Get_Key( void );
uint8_t Get_KeyNum( void );
void Scan_Key( void );


#endif /* __DRIVER_KEY_H_ */
