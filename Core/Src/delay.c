#include "delay.h"
#include "tim.h"

//void delay_ms(uint16_t nms)	
//{
//		__HAL_TIM_SetCounter(&htim1, 0);//htim1

//		__HAL_TIM_ENABLE(&htim1);

//		while(__HAL_TIM_GetCounter(&htim1) < (8 * nms));//????8KHz,8???1ms
//		/* Disable the Peripheral */
//		__HAL_TIM_DISABLE(&htim1);
//}

void delay_us(uint16_t nus)	//??0-8191us
{
		__HAL_TIM_SetCounter(&htim2, 0);//htim8

		__HAL_TIM_ENABLE(&htim2);

		while(__HAL_TIM_GetCounter(&htim2) < nus);
		/* Disable the Peripheral */
		__HAL_TIM_DISABLE(&htim2);
}


