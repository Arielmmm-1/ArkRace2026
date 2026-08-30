#include "cmsis_os.h"
#include "NRF24L01.h"
#include "NRFTask.h"

extern QueueHandle_t rcQueueHandle;

/* 最新接收的数据，供 OLEDTask 直接读取 */
RC_Data_t NRF_LastData = {0};

void StartNRFTask(void const * argument)
{
    RC_Data_t rc;

    for(;;)
    {
        /* 尝试接收一包数据 */
        if(NRF24L01_Receive() == 1)     // 1 表示接收成功
        {
            /* 解析遥控器数据（前7字节） */
            rc.Mode = NRF24L01_RxPacket[0];

            rc.LH   = NRF24L01_RxPacket[1];
            rc.LV   = NRF24L01_RxPacket[2];
            rc.RH   = NRF24L01_RxPacket[3];
            rc.RV   = NRF24L01_RxPacket[4];

            rc.KEY0 = NRF24L01_RxPacket[5];
            rc.KEY1 = NRF24L01_RxPacket[6];

            /* 更新显示用全局变量 */
            NRF_LastData = rc;

            /* 发送到队列，给 ControlTask 用 */
            xQueueOverwrite(rcQueueHandle, &rc);
        }

        osDelay(5);     // 5ms 轮询一次 ≈ 200Hz
    }
}
