#include "serial_protocol.h"
#include <string.h>
#include <stdlib.h>
#include "NRF24L01.h"

extern UART_HandleTypeDef huart2;

static uint8_t rx_byte;
static char frame_buf[128];
static uint16_t frame_len;
static uint8_t receiving;
static uint8_t qr_pending_nrf;   /* 有新 QR 数据待通过 NRF 发送 */

QRData_t QRData = {0};
volatile uint8_t QR_Updated = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART2) return;

    if (rx_byte == '@') { receiving = 1; frame_len = 0; }
    else if (rx_byte == '#')
    {
        receiving = 0;
        if (frame_len > 0) { frame_buf[frame_len] = '\0'; QR_Updated = 1; }
    }
    else if (receiving && frame_len < sizeof(frame_buf) - 1)
        frame_buf[frame_len++] = (char)rx_byte;

    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}

void Serial_Init(void)
{
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}

void Serial_Process(void)
{
    if (!QR_Updated) return;
    QR_Updated = 0;

    char *token, *saveptr, *fields[6];
    uint8_t cnt = 0;
    token = strtok_r(frame_buf, ",", &saveptr);
    while (token && cnt < 6) { fields[cnt++] = token; token = strtok_r(NULL, ",", &saveptr); }

    if (cnt != 6 || strcmp(fields[0], "QR") != 0) return;

    strncpy(QRData.type,   fields[0], sizeof(QRData.type)   - 1);
    strncpy(QRData.base,   fields[1], sizeof(QRData.base)   - 1);
    strncpy(QRData.core,   fields[2], sizeof(QRData.core)   - 1);
    strncpy(QRData.left,   fields[3], sizeof(QRData.left)   - 1);
    strncpy(QRData.middle, fields[4], sizeof(QRData.middle) - 1);
    strncpy(QRData.right,  fields[5], sizeof(QRData.right)  - 1);
    QRData.type[sizeof(QRData.type)-1] = 0;
    QRData.base[sizeof(QRData.base)-1] = 0;
    QRData.core[sizeof(QRData.core)-1] = 0;
    QRData.left[sizeof(QRData.left)-1] = 0;
    QRData.middle[sizeof(QRData.middle)-1] = 0;
    QRData.right[sizeof(QRData.right)-1] = 0;

    /* 回复 ACK */
    HAL_UART_Transmit(&huart2, (uint8_t *)"@ACK,QR#", 8, 100);

    /* 解析成功 → 点亮 LED (PC3 低电平亮) */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);

    /* 标记有新的 QR 数据待通过 NRF 发送回遥控器 */
    qr_pending_nrf = 1;
}

/* 通过 NRF24L01 将 QR 数据发送回遥控器（由 ControlTask 调用） */
void Serial_SendQRData(void)
{
    uint8_t i, idx;
    const char *fields[5];

    if (!qr_pending_nrf) return;
    qr_pending_nrf = 0;

    fields[0] = QRData.base;
    fields[1] = QRData.core;
    fields[2] = QRData.left;
    fields[3] = QRData.middle;
    fields[4] = QRData.right;

    /* 清空发送缓冲区 */
    for (i = 0; i < 32; i++)
        NRF24L01_TxPacket[i] = 0;

    /* Byte 0: 包类型标识 'Q' → QR 数据 */
    idx = 0;
    NRF24L01_TxPacket[idx++] = 'Q';

    /* 依次拼接 5 个字段，逗号分隔，总长不超过 31 字节 */
    for (i = 0; i < 5 && idx < 31; i++)
    {
        const char *p = fields[i];
        while (*p && idx < 31)
            NRF24L01_TxPacket[idx++] = *p++;
        if (i < 4 && idx < 31)
            NRF24L01_TxPacket[idx++] = ',';
    }

    NRF24L01_Send();
}
