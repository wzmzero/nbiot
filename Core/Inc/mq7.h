#ifndef __MQ7_H__
#define __MQ7_H__

#include "stm32f1xx_hal.h"
#include "delay.h"
void Init_MQ7(void);
int Get_ADCValue_MQ7(void);
void MQ7_PPM_Calibration(float RS);
float MQ7_GetPPM(void);

#endif