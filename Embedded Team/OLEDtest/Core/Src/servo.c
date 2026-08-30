#include "servo.h"
#include "tim.h"        // 使用 CubeMX 生成的 htim9

/**
 * @brief  初始化 TIM9_CH1(PE5) 为 50Hz PWM 控制 360° 舵机
 *         CubeMX 已配好时钟和引脚，这里重设 PSC/ARR 为舵机所需值
 *         PSC=167 → 1MHz,  ARR=19999 → 50Hz,  初始脉宽 1500=停止
 */
void Servo_Init(void)
{
    /* 先把引脚拉低，防止初始化过程中输出不确定电平导致舵机乱转 */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);

    /* 重设 PSC/ARR 为 50Hz */
    __HAL_TIM_SET_PRESCALER(&htim9, 168 - 1);
    htim9.Instance->ARR = 20000 - 1;
    htim9.Instance->EGR = TIM_EGR_UG;    // 产生更新事件，使 PSC/ARR 生效

    /* 设初始脉宽 = 0° (0.5ms) */
    __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 500);

    /* 启动 PWM 输出 */
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
}

/**
 * @brief  直接设置脉宽 (500~2500)
 *          500 =  0.5ms → 全速正转
 *         1500 =  1.5ms → 停止
 *         2500 =  2.5ms → 全速反转
 */
void Servo_SetPulse(uint16_t pulse)
{
    if (pulse < 500)  pulse = 500;
    if (pulse > 2500) pulse = 2500;
    __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, pulse);
}

/**
 * @brief  舵机停止 (1.5ms)
 */
void Servo_Stop(void)
{
    Servo_SetPulse(1500);
}

/**
 * @brief  速度控制
 * @param  speed: -100 ~ +100
 *           +100 = 全速正转
 *              0 = 停止
 *           -100 = 全速反转
 */
void Servo_SetSpeed(int8_t speed)
{
    if (speed > 100)  speed = 100;
    if (speed < -100) speed = -100;
    uint16_t pulse = 1500 - (int16_t)speed * 10;
    Servo_SetPulse(pulse);
}

/**
 * @brief  180° 舵机角度控制
 * @param  angle: 0~180 度
 *          0° → 0.5ms (500)   90° → 1.5ms (1500)   180° → 2.5ms (2500)
 */
void Servo_SetAngle(uint8_t angle)
{
    if (angle > 180) angle = 180;
    uint16_t pulse = 500 + (uint16_t)angle * 2000 / 180;
    Servo_SetPulse(pulse);
}
