#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

/* 180° / 360° 舵机控制（TIM9, PE5）
 * 脉宽范围 500~2500 对应 0.5ms~2.5ms (50Hz)
 *   180° 舵机: 500=0°, 1500=90°, 2500=180°
 *   360° 舵机: 500=全速正转, 1500=停止, 2500=全速反转
 */

void Servo_Init(void);                     // 初始化 TIM9 输出 50Hz PWM
void Servo_SetPulse(uint16_t pulse);       // 直接设脉宽 (500~2500)
void Servo_Stop(void);                     // 停止
void Servo_SetSpeed(int8_t speed);         // 360°: -100~+100 调速
void Servo_SetAngle(uint8_t angle);        // 180°: 0~180 设角度

#endif
