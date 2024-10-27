/*
 * @Author: Jchen
 * @Date: 2024-10-06 21:43:25
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-06 23:01:49
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Drivers\MCU-103\Src\driver_uart.c
 */
#include "driver_uart.h"
#include "OLED.h"
#include "driver_timer.h"
#include "usart.h"

#include <stdio.h>

#define DEBUG_UART_TIMEOUT 500

extern UART_HandleTypeDef huart1;
static UART_HandleTypeDef * g_HDebugUART = &huart1;

int fputc(int c, FILE *f) 
{
    (void)f;
    HAL_UART_Transmit(g_HDebugUART, (const uint8_t *)&c, 1, DEBUG_UART_TIMEOUT);
    return c;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    (void)f;

    /* Clear the Overrun flag just before receiving the first character */
    __HAL_UART_CLEAR_OREFLAG(g_HDebugUART);

    /* Wait for reception of a character on the USART RX line and echo this
    * character on console */
    HAL_UART_Receive(g_HDebugUART, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(g_HDebugUART, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;    
}

void UART_Init(void)
{
}


//void UART_Test(void)
//{    
//    char c;
//    
//    UART_Init();

//    while (1)
//    {
//        c = fgetc(NULL);
//        printf("\n\rget %c\n\r", c);
//    }
//}
