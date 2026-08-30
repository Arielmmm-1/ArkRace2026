#ifndef __SERIAL_H
#define __SERIAL_H

#include<stdio.h>

void Serial_Init(void);
void Serial2_Init(void);
void Serial3_Init(void);
void Serial2_SenByte(uint8_t Byte);
void Serial2_SendString(char*String);
void USART3_SendByte(uint8_t data);
void USART3_SendString(char *str);
void Cmd_NumDown(void);
void Cmd_NumUp(void);
void Cmd_Unknown(void);
void ParseCommand(uint8_t *buf, uint16_t len);
int fputc(int ch,FILE *f);

#endif
