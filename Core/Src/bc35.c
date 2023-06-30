#include <stdlib.h>
#include <time.h>
#include "bc35.h"
#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
char *strx=NULL,*extstrx;
extern uint8_t USART2_RX_BUF[USART2_REC_LEN];
extern uint16_t USART2_RX_STA;
uint8_t send_flag;
uint8_t reg_flag;
uint8_t login_flag;
unsigned char bc35_send_data[50];
void uart_printf(UART_HandleTypeDef *huart,const char *format, ...)
{
    va_list args;
    char buff[USART2_REC_LEN+1];
    uint32_t length;
    va_start(args, format);
    length = vsnprintf(buff, USART2_REC_LEN+1, (char *)format, args);
    va_end(args);
    HAL_UART_Transmit(huart,(uint8_t *)buff, length,0xffff);   //只需要更改这儿就能一直到其他平台
}

void nb_printf(char *fmt, ...)
{
    char buff[USART2_REC_LEN+1]; 
    uint16_t i=0;
    va_list arg_ptr;
    va_start(arg_ptr,fmt);
    vsnprintf(buff, USART2_REC_LEN+1,fmt,arg_ptr);
    i=strlen(buff);
    if(strlen(buff)>USART2_REC_LEN)i=USART2_REC_LEN;
    HAL_UART_Transmit(&huart2,(uint8_t *)buff,i,0Xffff);
    va_end(arg_ptr);
}
void Clear_Buffer(void)//娓呯┖缂撳瓨
{
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,'\0',sizeof(USART2_RX_BUF));
}
void nb_init(void)
{
	nb_printf("AT\r\n");
	while(huart2.gState != HAL_UART_STATE_READY){};
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
	Clear_Buffer();
	while(strx==NULL)
	{
		nb_printf("AT\r\n");
		while(huart2.gState != HAL_UART_STATE_READY){};
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
		Clear_Buffer();
	}
    uart_printf(&huart1,"AT finished\r\n");
	nb_printf("AT+CFUN=1\r\n");
	HAL_Delay(100);
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
	Clear_Buffer();
	while(strx==NULL)
	{
		nb_printf("AT+CFUN=1\r\n");
			HAL_Delay(100);
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
		Clear_Buffer();
	}
		nb_printf("AT+CFUN?\r\n");
		HAL_Delay(100);
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"+CFUN:1");
	Clear_Buffer();
	while(strx==NULL)
	{
		nb_printf("AT+CFUN?\r\n");
		HAL_Delay(100);
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"+CFUN:1");
		Clear_Buffer();
		HAL_Delay(100);
	}
    uart_printf(&huart1,"AT+CFUN finished\r\n");
//鍏ョ綉
	nb_printf("AT+CGATT=1\r\n");
	HAL_Delay(100);
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
	Clear_Buffer();
	while(strx==NULL)
	{
		HAL_Delay(1000);
		nb_printf("AT+CGATT=1\r\n");
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
		Clear_Buffer();
	}
	nb_printf("AT+CGATT?\r\n");
	HAL_Delay(100);
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"+CGATT:1");
	Clear_Buffer();
	while(strx==NULL)
	{
		HAL_Delay(1000);
		nb_printf("AT+CGATT?\r\n");
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"+CGATT:1");
		Clear_Buffer();
	}
    uart_printf(&huart1,"AT+CGATT finished\r\n");
}

char nb_reg(void)
{
	nb_printf("AT+QMTOPEN=0,\"122.51.127.33\",1883\r\n");
	while(huart2.gState != HAL_UART_STATE_READY){};
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"+QMTOPEN: 0,0");
	Clear_Buffer();
	while(strx==NULL)
	{
		nb_printf("AT+QMTOPEN=0,\"122.51.127.33\",1883\r\n");
		while(huart2.gState != HAL_UART_STATE_READY){};
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"ERROR");
		Clear_Buffer();
	}
    uart_printf(&huart1,"MQTT server connected\r\n");
	HAL_Delay(200);
	nb_printf("AT+QMTCONN=0,\"mqtt\"\r\n");
	while(huart2.gState != HAL_UART_STATE_READY){};
	strx=strstr((const char*)USART2_RX_BUF,(const char*)"+QMTCONN: 0,0,0");
	Clear_Buffer();
	while(strx==NULL)
	{
		nb_printf("AT+QMTCONN=0,\"mqtt\"\r\n");
		while(huart2.gState != HAL_UART_STATE_READY){};
		strx=strstr((const char*)USART2_RX_BUF,(const char*)"ERROR");
		Clear_Buffer();
	}
    uart_printf(&huart1,"mqtt build\r\n");
}

int nb_send(char* temp,char* hum,char* mq){

    char msg[200]={0};
    if(HAL_GPIO_ReadPin(LED2_GPIO_Port,LED2_Pin)==0)
    sprintf(msg,"{\"temper\":\"%s\",\"hum\":\"%s\",\"co\":\"%s\",\"swi\":false}",temp,hum,mq);
    else
        sprintf(msg,"{\"temper\":\"%s\",\"hum\":\"%s\",\"co\":\"%s\",\"swi\":true}",temp,hum,mq);
    int length = strlen(msg);
    nb_printf("AT+QMTPUB=0,1,1,1,\"/topic/1\",%d\r\n",length);
    nb_printf("%s\r\n",msg);
    strx=strstr((const char*)USART2_RX_BUF,(const char*)"+QMTPUB");
    Clear_Buffer();
}

int nb_sub(){
    nb_printf("AT+QMTSUB=0,1,\"/topic/2\",0\r\n");
    HAL_Delay(200);
    uart_printf(&huart1,"subscribe success\r\n");
    Clear_Buffer();
}

char nb_recv(){
    if(strstr((const char*)USART2_RX_BUF,(const char*)"+QMTRECV")){
        char str_rev=USART2_RX_BUF[strlen(USART2_RX_BUF)-4];
        Clear_Buffer();
        return str_rev;
    }
    Clear_Buffer();
    return '0';
}