
#include "mq7.h"
#include "delay.h"
#include "adc.h"
#include "math.h"
#define CAL_PPM  10  // У׼������PPMֵ
#define RL	10  // RL��ֵ
static float R0 = 8.00;
uint32_t ADC1_Value[10];
//�õ�ADC�����ڲ���������ֵ
//ȡ10��,Ȼ��ƽ��
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

// ������У׼����
void MQ7_PPM_Calibration(float RS)
{

    R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
}

// ��ȡ��������ֵ
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

