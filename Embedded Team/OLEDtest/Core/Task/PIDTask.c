#include "cmsis_os.h"
#include "encoder.h"
#include "motor.h"
#include "PID.h"

void StartPIDTask(void const * argument)
{
    /* PID 控制参数 */
    PID_t pid_motor1, pid_motor2, pid_motor3, pid_motor4;
    int32_t pwm_out[4];

    /* 初始化四个 PID 控制器 */
    PID_Init(&pid_motor1);
    PID_Init(&pid_motor2);
    PID_Init(&pid_motor3);
    PID_Init(&pid_motor4);

    /* 设置 PID 参数（需根据实际调试） */
    pid_motor1.Kp = 1.0f;   pid_motor1.Ki = 0.0f;   pid_motor1.Kd = 0.0f;
    pid_motor2.Kp = 1.0f;   pid_motor2.Ki = 0.0f;   pid_motor2.Kd = 0.0f;
    pid_motor3.Kp = 1.0f;   pid_motor3.Ki = 0.0f;   pid_motor3.Kd = 0.0f;
    pid_motor4.Kp = 1.0f;   pid_motor4.Ki = 0.0f;   pid_motor4.Kd = 0.0f;

    /* 输出限幅 */
    pid_motor1.OutMax =  999;  pid_motor1.OutMin = -999;
    pid_motor2.OutMax =  999;  pid_motor2.OutMin = -999;
    pid_motor3.OutMax =  999;  pid_motor3.OutMin = -999;
    pid_motor4.OutMax =  999;  pid_motor4.OutMin = -999;

    for(;;)
    {
        /* 读取编码器速度 */
        Encoder_GetSpeed(Actual_Wheel);

        /* PID 计算 — 目标值来自 ControlTask 设的 Target_Wheel[] */
        pid_motor1.Target = Target_Wheel[0];  pid_motor1.Actual = Actual_Wheel[0];
        pid_motor2.Target = Target_Wheel[1];  pid_motor2.Actual = Actual_Wheel[1];
        pid_motor3.Target = Target_Wheel[2];  pid_motor3.Actual = Actual_Wheel[2];
        pid_motor4.Target = Target_Wheel[3];  pid_motor4.Actual = Actual_Wheel[3];

        PID_Update(&pid_motor1);
        PID_Update(&pid_motor2);
        PID_Update(&pid_motor3);
        PID_Update(&pid_motor4);

        pwm_out[0] = (int32_t)pid_motor1.Out;
        pwm_out[1] = (int32_t)pid_motor2.Out;
        pwm_out[2] = (int32_t)pid_motor3.Out;
        pwm_out[3] = (int32_t)pid_motor4.Out;

        Motor_Load((int16_t)pwm_out[0], (int16_t)pwm_out[1],
                   (int16_t)pwm_out[2], (int16_t)pwm_out[3]);

        osDelay(5);   // 5ms 周期
    }
}
