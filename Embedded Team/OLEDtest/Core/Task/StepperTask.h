#ifndef __STEPPER_TASK_H
#define __STEPPER_TASK_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

/* 步进电机命令 */
typedef struct
{
    uint8_t dir;       // 方向 0/1
    uint16_t pulse;    // 脉冲数
} StepperCmd_t;

/* 命令队列句柄（ControlTask发，StepperTask收） */
extern QueueHandle_t stepperQueueHandle;

void StartStepperTask(void const * argument);

#endif
