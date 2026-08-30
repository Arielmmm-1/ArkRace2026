#ifndef __NRF_TASK_H
#define __NRF_TASK_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

/* 遥控器数据帧 */
typedef struct
{
    uint8_t Mode;

    int8_t LH;
    int8_t LV;

    int8_t RH;
    int8_t RV;

    uint8_t KEY0;
    uint8_t KEY1;

}RC_Data_t;
/* 最新接收到的遥控器数据（供 OLEDTask 显示） */
extern RC_Data_t NRF_LastData;

/* 队列句柄（NRFTask 发，ControlTask 收） */
extern QueueHandle_t rcQueueHandle;

void StartNRFTask(void const * argument);

#endif
