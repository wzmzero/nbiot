
#ifndef __BC35_H__
#define __BC35_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
void uart_printf(UART_HandleTypeDef *huart,const char *format, ...);
void nb_printf(char *fmt, ...);	
void Clear_Buffer(void);
void nb_init(void);	
char nb_reg(void);	
void nb_login(char socket_id);
int nb_send(char* temp,char* hum,char* mq);
int nb_sub();
char nb_recv();
extern uint8_t send_flag;
extern uint8_t reg_flag;
extern uint8_t login_flag;
#define NB_UDP "AT+NSOCR=DGRAM,17,0,1"
#define NB_SOST "1,43.249.192.204,46324,4,71756974"	
#define NB_NSORF "AT+NSORF=1,17"	
	
#define NB_TCP	"AT+NSOCR=STREAM,6,35000,1"	 //TCPÁ¬½Ó
#define NB_NSOCO "AT+NSOCO=1,124.226.64.130,48439"	//TCP·¢ËÍ
#define NB_NSOSD "AT+NSOSD=1,5,68656c6c66"	
	
#endif