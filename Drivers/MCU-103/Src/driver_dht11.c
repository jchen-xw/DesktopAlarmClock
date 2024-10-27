#include "driver_dht11.h"
#include "driver_timer.h"
#include "stm32f1xx_hal.h"
#include "main.h"


/* 控制GPIO读取DHT11的数据 
 * 1. 主机发出至少18MS的低脉冲: start信号
 * 2. start信号变为高, 20-40us之后, dht11会拉低总线维持80us
      然后拉高80us: 回应信号
 * 3. 之后就是数据, 逐位发送
 *    bit0 : 50us低脉冲, 26-28us高脉冲
 *    bit1 : 50us低脉冲, 70us高脉冲
 * 4. 数据有40bit: 8bit湿度整数数据+8bit湿度小数数据
                   +8bit温度整数数据+8bit温度小数数据
                   +8bit校验和
 */


/* 先实现GPIO的基本操作 */
/**********************************************************************
 * 函数名称： DHT11_PinCfgAsOutput
 * 功能描述： 把DHT11的数据引脚配置为输出
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
static void DHT11_PinCfgAsOutput(void)
{
    /* 对于STM32F103, 已经把DHT11的引脚配置为"open drain, pull-up" */
}

/**********************************************************************
 * 函数名称： DHT11_PinCfgAsInput
 * 功能描述： 把DHT11的数据引脚配置为输入
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
static void DHT11_PinCfgAsInput(void)
{
    /* 对于STM32F103, 已经把DHT11的引脚配置为"open drain, pull-up" 
	* 让它输出1就不会驱动这个引脚, 并且可以读入引脚状态
     */
    HAL_GPIO_WritePin( DHT11_PA11_GPIO_Port, DHT11_PA11_Pin, GPIO_PIN_SET );
}

/**********************************************************************
 * 函数名称： DHT11_PinSet
 * 功能描述： 设置DHT11的数据引脚的输出值
 * 输入参数： val - 输出电平
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
static void DHT11_PinSet(int val)
{
	if (val)
		HAL_GPIO_WritePin(DHT11_PA11_GPIO_Port, DHT11_PA11_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DHT11_PA11_GPIO_Port, DHT11_PA11_Pin, GPIO_PIN_RESET);
}

/**********************************************************************
 * 函数名称： DHT11_PinRead
 * 功能描述： 读取DHT11的数据引脚
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 1-高电平, 0-低电平
 ***********************************************************************/
static int DHT11_PinRead(void)
{
    if ( GPIO_PIN_SET == HAL_GPIO_ReadPin( DHT11_PA11_GPIO_Port, DHT11_PA11_Pin ) )
		return 1;
	else
		return 0;
}


/* 再来实现DHT11的读操作 */
/**********************************************************************
 * 函数名称： DHT11_Start
 * 功能描述： 给DHT11发出启动信号 
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
static void DHT11_Start(void)
{
	DHT11_PinSet(0);
	mdelay(20);
	DHT11_PinCfgAsInput();
}


/**********************************************************************
 * 函数名称： DHT11_Wait_Ack
 * 功能描述： 等待DHT11的回应信号
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 1-无响应, 0-有响应
 ***********************************************************************/
static int DHT11_Wait_Ack(void)
{
	udelay(60);
	return DHT11_PinRead();
}

/**********************************************************************
 * 函数名称： DHT11_WaitFor_Val
 * 功能描述： 在指定时间内等待数据引脚变为某个值
 * 输入参数： val - 期待数据引脚变为这个值
 *            timeout_us - 超时时间(单位us)
 * 输出参数： 无
 * 返 回 值： 0-成功, (-1) - 失败
 ***********************************************************************/
static int DHT11_WaitFor_Val(int val, int timeout_us)
{
	while (timeout_us--)
	{
		if (DHT11_PinRead() == val)
			return 0; /* ok */
		udelay(1);
	}
	return -1; /* err */
}

/**********************************************************************
 * 函数名称： DHT11_ReadByte
 * 功能描述： 读取DH11 1byte数据
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 数据
 ***********************************************************************/
static int DHT11_ReadByte(void)
{
	int i;
	int data = 0;
	
	for (i = 0; i < 8; i++)
	{
		if (DHT11_WaitFor_Val(1, 1000))
		{
			return -1;
		}
		udelay(40);
		data <<= 1;
		if (DHT11_PinRead() == 1)
			data |= 1;
		
		if (DHT11_WaitFor_Val(0, 1000))
		{
			return -1;
		}
	}
	
	return data;
}



/* 公开的函数 */

/**********************************************************************
 * 函数名称： DHT11_Init
 * 功能描述： DHT11的初始化函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void DHT11_Init(void)
{
	DHT11_PinCfgAsOutput();
	DHT11_PinSet(1);
	//mdelay(2000);
}


/**********************************************************************
 * 函数名称： DHT11_Read
 * 功能描述： 读取DHT11的温度/湿度
 * 输入参数： 无
 * 输出参数： hum  - 用于保存湿度值
 *            temp - 用于保存温度值
 * 返 回 值： 0 - 成功, (-1) - 失败
 ***********************************************************************/
int8_t DHT11_Read(uint8_t *hum, uint8_t *temp)
{
	uint8_t hum_m, hum_n;
	uint8_t temp_m, temp_n;
	uint8_t check;	
	
	DHT11_Start();
	
	if (0 != DHT11_Wait_Ack())
	{
		return -1;
	}

	if (0 != DHT11_WaitFor_Val(1, 1000))  /* 等待ACK变为高电平, 超时时间是1000us */
	{
		return -1;
	}

	if (0 != DHT11_WaitFor_Val(0, 1000))  /* 数据阶段: 等待低电平, 超时时间是1000us */
	{
		return -1;
	}

	hum_m  = DHT11_ReadByte();
	hum_n  = DHT11_ReadByte();
	temp_m = DHT11_ReadByte();
	temp_n = DHT11_ReadByte();
	check  = DHT11_ReadByte();

	DHT11_PinCfgAsOutput();
	DHT11_PinSet(1);

	if (hum_m + hum_n + temp_m + temp_n == check)
	{
		*hum  = hum_m;
		*temp = temp_m;
		return 0;
	}
	else
	{
		return -1;
	}

}


/**********************************************************************
 * 函数名称： DHT11_Test
 * 功能描述： DHT11测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 ***********************************************************************/
void DHT11_Test(void)
{
	uint8_t hum, temp;
    // int len;
	
	DHT11_Init();
	
	while (1)
	{
		if (DHT11_Read(&hum, &temp) !=0 )
		{
			DHT11_Init();
		}
		else
		{
//			LCD_PrintString(0, 0, "DHT11:");
//            LCD_PrintString(0, 2, "Humidity Temp");
//            len = LCD_PrintSignedVal(0, 4, hum);
//            LCD_PutChar(len, 4, '%');
//            LCD_PrintSignedVal(9, 4, temp);
		}
		mdelay(2000);  /* 读取周期是2S, 不能读太频繁 */
	}
}



