
#include "mq7.h"
#include "delay.h"
#include "adc.h"
#include "math.h"
#define CAL_PPM  10  // 校准环境中PPM值
#define RL	10  // RL阻值
static float R0 = 8.00;
uint32_t ADC1_Value[10];
//得到ADC采样内部传感器的值
//取10次,然后平均
void Init_MQ7(void ){
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC1_Value,10);
}
int Get_ADCValue_MQ7(void)
{
    int i = 0;
    int ad1=0;
    for(i=0,ad1=0;i<10;)
    {
        ad1+=ADC1_Value[i++];
    }
    ad1/=10;
//    printf("adc:%d\r\n",ad1);
    return ad1;
}

// 传感器校准函数
void MQ7_PPM_Calibration(float RS)
{

    R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
}

// 获取传感器的值
float MQ7_GetPPM(void)
{
    float Vrl = 3.3f * Get_ADCValue_MQ7() / 4096.f;
    Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
    float RS = (3.3f - Vrl) / Vrl * RL;
//	printf("MQ7_VRL = %.2f\n", Vrl);
    MQ7_PPM_Calibration(RS);
    float ppm = 98.322f * pow(RS/R0, -1.458f);

    return  ppm;
}

