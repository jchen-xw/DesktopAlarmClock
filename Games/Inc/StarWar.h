/*
 * @Author: Jchen
 * @Date: 2024-09-27 22:41:19
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-02 23:56:12
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Games\Inc\StarWar.h
 */
#ifndef __STARWAR_H_
#define __STARWAR_H_

#define ENEMY_CENTER_DISPLAY_X          98
#define STARWAR_VERSION_DISPLAY         "V1.0.0"
#define MISSILE_DEFAULT_POSITION_X      16
#define MISSILE_DEFAULT_LENGTH          4

void StarWar_Task( void *params );

#endif
