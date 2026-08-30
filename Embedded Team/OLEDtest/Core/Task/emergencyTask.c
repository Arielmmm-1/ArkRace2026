#include "cmsis_os.h"
#include "gpio.h"
#include "motor.h"
#include "chassis.h"

/**
  * @brief  紧急制动任务 — 检测 PB2 按键
  *         按键按下(PB2=低电平) → 立即停止所有电机
  *         优先级 osPriorityRealtime (6)
  */
void StartEmergencyTask(void const * argument)
{
    for(;;)
    {
        /* 检测 PB2 按键（按下为低电平，已启用内部上拉） */
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
        {
            Chassis_Stop();              // 底盘急停
            Motor_Load(0, 0, 0, 0);     // 电机PWM归零

            /* 等待松手 — 防止反复触发 */
            while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
            {
                osDelay(10);
            }
        }

        osDelay(10);  
    }
}
