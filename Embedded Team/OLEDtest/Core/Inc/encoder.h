#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim1;

/* 归一化速度数组 (-1000~1000)，供外部使用 */
extern int16_t Actual_Wheel[4];
extern int16_t Target_Wheel[4];

/* 原始脉冲差值（调试用） */
extern int32_t Raw_Pulse[4];

int32_t Encoder_ReadDiff(TIM_HandleTypeDef *htim);
int16_t Encoder_CalcSpeed(int32_t pulse_diff, uint16_t time_ms);
void Encoder_GetSpeed(int16_t *speed_buf);

/* 测速参数（根据实际电机调整 MAX_PULSE_PER_1MS） */
#define ENCODER_TIM_PERIOD_MS 5     // PIDTask周期5ms
#define MAX_PULSE_PER_1MS      8.0f  // 1ms内最大脉冲数（标定值：全速40脉冲/10ms）

#endif