#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H

#include "main.h"

/* 解析后的五项数据 */
typedef struct {
    char type[8];      // QR
    char base[16];     // 基础框
    char core[16];     // 核心框
    char left[16];     // 左框
    char middle[16];   // 中框
    char right[16];    // 右框
} QRData_t;

extern QRData_t QRData;          // 最新解析结果（供 OLED 显示）
extern volatile uint8_t QR_Updated;  // 新数据标志

void Serial_Init(void);
void Serial_Process(void);       // 周期调用，解析帧数据
void Serial_SendQRData(void);    // 通过 NRF24L01 将 QR 数据发回遥控器

#endif
