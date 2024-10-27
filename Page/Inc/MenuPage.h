/*
 * @Author: Jchen
 * @Date: 2024-10-23 10:41:22
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-27 11:09:33
 * @Description: 
 * @FilePath: \DesktopAlarmClock\Page\Inc\MenuPage.h
 */
#ifndef __MENUPAGE_H_
#define __MENUPAGE_H_

/* 菜单标题 */
#define MENU_TITLE_POSITION_X                   48
#define MENU_TITLE_POSITION_Y                   0
#define MENU_TITLE_CONTENT                      "Menu"

/* 当前选项框位置 */
#define OPTION_BOX_POSITION_X                   48
#define OPTION_BOX_POSITION_Y                   17
#define OPTION_BOX_SIDE_LENGTJ                  35

/* 当前选项位置 */
#define OPTION_ICON_POSITION_X                  50
#define OPTION_ICON_POSITION_Y                  19

/* 下一选项位置 */
#define OPTION_NEXT_POSITION_X                  130

/* 上一选项位置 */
#define OPTION_PREVIOUS_POSITION_X              -30

/* 选项名称位置 */
#define OPTION_TITLE_POSITION_X                 49
#define OPTION_TITLE_POSITION_Y                 54

#define MENU_1_TITLE_CONTENT                    "Time"
#define MENU_2_TITLE_CONTENT                    "Alarm"
#define MENU_3_TITLE_CONTENT                    "Music"
#define MENU_4_TITLE_CONTENT                    "Game"
#define MENU_5_TITLE_CONTENT                    "Timer"
#define MENU_6_TITLE_CONTENT                    "Set"

/* 任务栈 */
#define TIME_SET_STK_SIZE                       60
#define ALARM_SET_STK_SIZE                      60
#define MUSIC_STK_SIZE                          45
#define TIMER_STK_SIZE                          60
#define DINOGAME_STK_SIZE                       60
#define STARWAR_STK_SIZE                        50
#define INFO_STK_SIZE                           50


/* 游戏图标 */
#define GAME1_ICON                              Dino_Game
#define GAME2_ICON                              Star_War

#define GAME1_ICON_POSITION_X                   34
#define GAME1_ICON_POSITION_Y                   21

#define GAME2_ICON_POSITION_X                   76
#define GAME2_ICON_POSITION_Y                   21

#define GAME_ICON_LENGTH                        18

/* 游戏运行字符显示 */
#define RUNNING_STATE_DISPLAY_X                 108
#define RUNNING_STATE_DISPLAY_Y                 1

/* 图标方框 */
#define SQUARE_1_POSITION_X                     33    
#define SQUARE_1_POSITION_Y                     20

#define SQUARE_2_POSITION_X                     75    
#define SQUARE_2_POSITION_Y                     20

#define SQUARE_LENGTH                           20

void MenuPage_Task( void *param );


#endif /* __MENUPAGE_H_ */
