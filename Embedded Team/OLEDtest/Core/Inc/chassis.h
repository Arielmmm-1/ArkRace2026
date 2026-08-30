#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <stdint.h>
#include "motor.h"
/* ========== 底盘参数 ========== */

#define CHASSIS_SPEED_MAX   999

/**
  * @brief  麦轮几何比例系数 K_geo (浮点数)
  *         完整公式: m = forward + strafe ± K_geo * rotate
  *         K_geo = (轴距 + 轮距) / (2 * 轮半径)
  *         先设为1.0f，实际测量后调整
  */
#define CHASSIS_K_GEO       1.0f

/* ========== 函数声明 ========== */

/**
  * @brief  底盘麦轮控制（逆运动学）
  * @param  forward  前后速度  -999 ~ +999  (正向=前进)
  * @param  strafe   左右平移  -999 ~ +999  (正向=左移)
  * @param  rotate   旋转速度  -999 ~ +999  (正向=逆时针旋转)
  * @note   内部会自动限幅并归一化，保证各电机不超限
  */
void Chassis_Ctrl(int16_t forward, int16_t strafe, int16_t rotate);

/**
  * @brief  底盘急停 — 所有电机设为0
  */
void Chassis_Stop(void);

#endif /* __CHASSIS_H */
