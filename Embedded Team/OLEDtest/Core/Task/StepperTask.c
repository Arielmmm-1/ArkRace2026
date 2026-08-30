#include "cmsis_os.h"
#include "StepperTask.h"
#include "MotorPulse.h"

void StartStepperTask(void const * argument)
{
    StepperCmd_t cmd;

    for(;;)
    {
        /* 阻塞等待步进电机命令（超时无限，有命令才执行） */
        if(xQueueReceive(stepperQueueHandle, &cmd, portMAX_DELAY) == pdTRUE)
        {
            MotorPulse(cmd.dir, cmd.pulse);
        }
    }
}
