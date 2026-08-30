#include "cmsis_os.h"
#include "chassis.h"
#include "NRFTask.h"
#include "NRF24L01.h"
#include "math.h"
#include <stdlib.h>
#include "motor.h"
#include "encoder.h"
#include "StepperTask.h"
#include "servo.h"
#include "serial_protocol.h"

#define JOYSTICK_DEAD    15      // 摇杆死区（滤除十字耦合抖动）
#define RV_DEAD          30      // RV步进电机死区
#define RV_PULSE_STEP    800     // 每次触发脉冲数

void StartControlTask(void const * argument)
{
    RC_Data_t rc;
    int16_t forward, strafe, rotate;
    TickType_t lastCmdTick = 0;
    uint8_t lastKEY0 = 0;       // 上一次KEY0状态，用于按键边沿检测
    uint16_t testAngle = 0;     // 舵机角度测试（K7每按一次+30°）

    for(;;)
    {
        /* 从队列读取遥控数据（不阻塞，立即返回） */
        if(xQueueReceive(rcQueueHandle, &rc, 0) == pdTRUE)
        {
            lastCmdTick = xTaskGetTickCount();   // 刷新最近收到数据的时间

            /* ---------- 摇杆死区 ---------- */
            if(abs(rc.LV) < JOYSTICK_DEAD) rc.LV = 0;
            if(abs(rc.LH) < JOYSTICK_DEAD) rc.LH = 0;
            if(abs(rc.RH) < JOYSTICK_DEAD) rc.RH = 0;

            /* 十字耦合抑制：主轴向推到底时，忽略另一轴的小信号 */
            if(abs(rc.LH) > 70 && abs(rc.LV) < 30) rc.LV = 0;
            if(abs(rc.LV) > 70 && abs(rc.LH) < 30) rc.LH = 0;

            /* ---------- 底盘控制 ---------- */
            /*  LV(+)=前进  LH(+)=右移  RH(+)=右转
                映射到Chassis_Ctrl:
                forward = LV × MAX/100
                strafe  = LH × MAX/100   (LH推右→右移)
                rotate  = RH × MAX/100   (RH推右→右转)
            */
            forward =  rc.LV * CHASSIS_SPEED_MAX / 100;
            // strafe  =  rc.LH * CHASSIS_SPEED_MAX / 100;
            strafe  =  0;
            rotate  =  rc.RH * CHASSIS_SPEED_MAX / 100;

            Chassis_Ctrl(forward, strafe, rotate);

            /* ---------- K5/K6步进电机单步控制 ---------- */
            if ((rc.KEY0 & 0x10) && !(lastKEY0 & 0x10))   // K5 按下：反转一圈
            {
                StepperCmd_t cmd = {1, 6400};
                xQueueOverwrite(stepperQueueHandle, &cmd);
            }
            if ((rc.KEY0 & 0x20) && !(lastKEY0 & 0x20))   // K6 按下：正转一圈
            {
                StepperCmd_t cmd = {0, 6400};
                xQueueOverwrite(stepperQueueHandle, &cmd);
            }
            /* ---------- K7/K8 角度测试（原始代码已注释） ---------- */
            if ((rc.KEY0 & 0x40) && !(lastKEY0 & 0x40))   // K7 按下：闭合 (0°)
            {
                Servo_SetAngle(20);
            }
            if ((rc.KEY0 & 0x80) && !(lastKEY0 & 0x80))   // K8 按下：张开 (90°)
            {
                Servo_SetAngle(50);
            }
            lastKEY0 = rc.KEY0;
        }
        else
        {
            /* 超过200ms没收到新数据 → 遥控器掉线，急停 */
            if(xTaskGetTickCount() - lastCmdTick > pdMS_TO_TICKS(200))
            {
                Chassis_Stop();
            }
            /* 否则保持上次的控制值不变，不产生停顿 */
        }
        /* ---------- 串口协议帧解析 + NRF 回传 ---------- */
        Serial_Process();
        Serial_SendQRData();

        osDelay(10);    // 10ms控制周期
    }
}