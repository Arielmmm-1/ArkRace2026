#include "encoder.h"

int16_t Actual_Wheel[4] = {0, 0, 0, 0};
int16_t Target_Wheel[4] = {0, 0, 0, 0};
int32_t Raw_Pulse[4] = {0, 0, 0, 0};

/**
  * @brief  读取单路编码器差值（重置法：读后清零，每周期积分一次）
  * @param  htim 定时器句柄
  * @retval 有符号脉冲数（正=正转，负=反转）
  */
int32_t Encoder_ReadDiff(TIM_HandleTypeDef *htim)
{
    int32_t pulse = __HAL_TIM_GET_COUNTER(htim);
    __HAL_TIM_SET_COUNTER(htim, 0);

    if(pulse < 32768)
        return pulse;           // 正转
    else
        return pulse - 65536;   // 反转（负值）
}

/**
  * @brief  脉冲差值转为归一化速度 (-1000 ~ 1000)
  */
int16_t Encoder_CalcSpeed(int32_t pulse_diff, uint16_t time_ms)
{
    float pulse_per_ms = (float)pulse_diff / time_ms;
    float speed_norm = pulse_per_ms / MAX_PULSE_PER_1MS * 1000.0f;

    if(speed_norm > 1000.0f) speed_norm = 1000.0f;
    if(speed_norm < -1000.0f) speed_norm = -1000.0f;

    return (int16_t)speed_norm;
}

/**
  * @brief  批量读取四路编码器速度
  * @param  speed_buf: 输出数组（长度4）
  */
void Encoder_GetSpeed(int16_t *speed_buf)
{
    int32_t pulse0 = Encoder_ReadDiff(&htim3);
    int32_t pulse1 = Encoder_ReadDiff(&htim4);
    int32_t pulse2 = Encoder_ReadDiff(&htim5);
    int32_t pulse3 = Encoder_ReadDiff(&htim1);

    /* 保存原始脉冲（调试用） */
    Raw_Pulse[0] = -pulse0;
    Raw_Pulse[1] = -pulse1;
    Raw_Pulse[2] = pulse2;
    Raw_Pulse[3] = pulse3;

    /* 电机1、2 A/B相反接，取反修正方向 */
    speed_buf[0] = Encoder_CalcSpeed(-pulse0, ENCODER_TIM_PERIOD_MS);
    speed_buf[1] = Encoder_CalcSpeed(-pulse1, ENCODER_TIM_PERIOD_MS);
    speed_buf[2] = Encoder_CalcSpeed(pulse2, ENCODER_TIM_PERIOD_MS);
    speed_buf[3] = Encoder_CalcSpeed(pulse3, ENCODER_TIM_PERIOD_MS);
}