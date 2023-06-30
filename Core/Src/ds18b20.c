#include "ds18b20.h"
#include "main.h"


//IO��������
void DS18B20_IO_IN(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = DS18B20_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DS18B20_GPIO_Port,&GPIO_InitStructure);
}

void DS18B20_IO_OUT(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = DS18B20_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS18B20_GPIO_Port,&GPIO_InitStructure);
}


//��λDS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_OUT=1; //DQ=1
    delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void)
{
    uint8_t retry=0;
    DS18B20_IO_IN();//SET PA0 INPUT
    while (DS18B20_DQ_IN&&retry<200)
    {
        retry++;
        delay_us(1);
    };
    if(retry>=200)return 1;
    else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
    {
        retry++;
        delay_us(1);
    };
    if(retry>=240)return 1;
    return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
    uint8_t data;
    DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT=0;
    delay_us(2);
    DS18B20_DQ_OUT=1;
    DS18B20_IO_IN();//SET PA0 INPUT
    delay_us(12);
    if(DS18B20_DQ_IN)data=1;
    else data=0;
    delay_us(50);
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i,j,dat;
    dat=0;
    for (i=1;i<=8;i++)
    {
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();
    for (j=1;j<=8;j++)
    {
        testb=dat&0x01;
        dat=dat>>1;
        if (testb)
        {
            DS18B20_DQ_OUT=0;
            delay_us(2);
            DS18B20_DQ_OUT=1;
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT=0;
            delay_us(60);
            DS18B20_DQ_OUT=1;
            delay_us(2);
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0x44);
}
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����
uint8_t DS18B20_Init(void)
{
    DS18B20_Rst();
    return DS18B20_Check();
}
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250��
float DS18B20_Get_Temp(void)
{
    uint16_t temp;
    uint8_t TL,TH;
    float value;
    DS18B20_Start();//��ʼ�¶�ת����������һ��������
    DS18B20_Rst();//��λ
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);
    TL=DS18B20_Read_Byte();
    TH=DS18B20_Read_Byte();
    temp=TH;
    temp=(temp<<8)+TL;
    if((temp&0xf800)==0xf800)
    {
        temp=(~temp)+1;
        value=temp*(-0.0625);
    }
    else
    {
        value=temp*0.0625;
    }
    return value;
}


