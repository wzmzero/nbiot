#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "stm32f1xx_hal.h"
#include "delay.h"



#define	DS18B20_DQ_OUT PAout(1)
#define	DS18B20_DQ_IN  PAin(1)

uint8_t DS18B20_Init(void);			//初始化DS18B20
float DS18B20_Get_Temp(void);		//获取温度
void DS18B20_Start(void);			//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);	//读出一个字节
uint8_t DS18B20_Read_Bit(void);		//读出一个位
uint8_t DS18B20_Check(void);		//检测是否存在DS18B20
void DS18B20_Rst(void);				//复位DS18B20


#endif

