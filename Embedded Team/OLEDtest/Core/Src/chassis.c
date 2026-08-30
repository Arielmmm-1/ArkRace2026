#include "chassis.h"
#include <math.h>
/* ---------- 内部工具函数 ---------- */

/**
  * @brief  绝对值
  */
static int16_t _abs(int16_t x)
{
    return (x < 0) ? -x : x;
}

/* ========== 底盘控制 — 麦轮逆运动学 ========== */

/**
  * 麦轮布局（俯视）：
  *
  *       前
  *   左上(LF) ─── 右上(RF)
  *       \          /
  *        \   ╳   /
  *       /          \
  *   左下(LB) ─── 右下(RB)
  *       后
  *
  * 轮子类型：
  *   左上(LF) — 右旋轮    右上(RF) — 左旋轮
  *   左下(LB) — 左旋轮    右下(RB) — 右旋轮
  *
 * 逆运动学公式 (vx=前后, vy=左右, vw=旋转, K=CHASSIS_K_GEO)：
 *   LF =  vx + vy + K * vw
 *   RF =  vx - vy - K * vw
 *   LB =  vx - vy + K * vw
 *   RB =  vx + vy - K * vw
  */


  /*
    // 前进 Chassis_Ctrl(500, 0, 0);
    // 后退 Chassis_Ctrl(-500, 0, 0);
    // 左移 Chassis_Ctrl(0, 500, 0);
    // 右移 Chassis_Ctrl(0, -500, 0);
    // 逆时针旋转 Chassis_Ctrl(0, 0, 500);
    // 顺时针旋转 Chassis_Ctrl(0, 0, -500);
  */

void Chassis_Ctrl(int16_t forward, int16_t strafe, int16_t rotate)
{
    int16_t m1;  // 左上(LF)
    int16_t m2;  // 右上(RF)
    int16_t m3;  // 左下(LB)
    int16_t m4;  // 右下(RB)

    /* 1. 逆运动学计算 (带几何系数) */
    int16_t kr = (int16_t)roundf((float)rotate * CHASSIS_K_GEO);
    m1 =  forward + strafe + kr;
    m2 =  forward - strafe - kr;
    m3 =  forward - strafe + kr;
    m4 =  forward + strafe - kr;

    /* 2. 归一化 — 防某一轮超限而其他轮浪费 */
    int16_t max_speed = _abs(m1);
    if (_abs(m2) > max_speed) max_speed = _abs(m2);
    if (_abs(m3) > max_speed) max_speed = _abs(m3);
    if (_abs(m4) > max_speed) max_speed = _abs(m4);

    if (max_speed > CHASSIS_SPEED_MAX)
    {
        m1 = (int16_t)((int32_t)m1 * CHASSIS_SPEED_MAX / max_speed);
        m2 = (int16_t)((int32_t)m2 * CHASSIS_SPEED_MAX / max_speed);
        m3 = (int16_t)((int32_t)m3 * CHASSIS_SPEED_MAX / max_speed);
        m4 = (int16_t)((int32_t)m4 * CHASSIS_SPEED_MAX / max_speed);
    }

    /* 3. 调用你已有的电机控制函数，一次设置4个电机 */
    Motor_Load(m1, m2, m3, m4);   // 左上, 右上, 左下, 右下
}

/* ========== 底盘急停 ========== */

void Chassis_Stop(void)
{
    Chassis_Ctrl(0, 0, 0);
}
