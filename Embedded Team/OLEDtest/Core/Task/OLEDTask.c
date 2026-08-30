#include "cmsis_os.h"
#include "oled.h"
#include "encoder.h"
#include "NRFTask.h"
#include "NRF24L01.h"
#include "serial_protocol.h"
#include <stdlib.h>

/* 显示带符号的 int8_t 值（-100~100） */
static void OLED_ShowInt8(u8 x, u8 y, int8_t val, u8 size)
{
    u8 buf[8];
    if (val < 0)
    {
        buf[0] = '-';
        val = -val;
    }
    else
    {
        buf[0] = ' ';
    }
    buf[1] = '0' + (val / 100) % 10;
    buf[2] = '0' + (val / 10) % 10;
    buf[3] = '0' + val % 10;
    buf[4] = '\0';
    OLED_ShowString(x, y, buf, size, 1);
}

void StartOLEDTask(void const * argument)
{
    for(;;)
    {
        OLED_Clear();

        /* ========== 串口接收显示 ========== */
        OLED_ShowString(0,  0, (u8 *)"B:", 8, 1);
        OLED_ShowString(16, 0, (u8 *)QRData.base, 8, 1);
        OLED_ShowString(64, 0, (u8 *)"C:", 8, 1);
        OLED_ShowString(80, 0, (u8 *)QRData.core, 8, 1);
        OLED_ShowString(0,  12, (u8 *)"L:", 8, 1);
        OLED_ShowString(16, 12, (u8 *)QRData.left, 8, 1);
        OLED_ShowString(64, 12, (u8 *)"M:", 8, 1);
        OLED_ShowString(80, 12, (u8 *)QRData.middle, 8, 1);
        OLED_ShowString(0,  24, (u8 *)"R:", 8, 1);
        OLED_ShowString(16, 24, (u8 *)QRData.right, 8, 1);

        /* ---- 按键指示 ---- */
        if (K1_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(0,  48, (u8 *)"K1", 8, 1);
        if (K2_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(24, 48, (u8 *)"K2", 8, 1);
        if (K3_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(48, 48, (u8 *)"K3", 8, 1);
        if (K4_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(72, 48, (u8 *)"K4", 8, 1);
        if (K5_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(96, 48, (u8 *)"K5", 8, 1);
        if (K6_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(0,  56, (u8 *)"K6", 8, 1);
        if (K7_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(24, 56, (u8 *)"K7", 8, 1);
        if (K8_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(48, 56, (u8 *)"K8", 8, 1);

        /* ================================================================ */
        /*  原有的摇杆+编码器显示，暂时注释掉                                  */
        // OLED_ShowString(0,  0, (u8 *)"LH:", 8, 1);
        // OLED_ShowInt8(24, 0, NRF_LastData.LH, 8);
        // OLED_ShowString(56, 0, (u8 *)"LV:", 8, 1);
        // OLED_ShowInt8(80, 0, NRF_LastData.LV, 8);
        //
        // OLED_ShowString(0,  16, (u8 *)"RH:", 8, 1);
        // OLED_ShowInt8(24, 16, NRF_LastData.RH, 8);
        // OLED_ShowString(56, 16, (u8 *)"RV:", 8, 1);
        // OLED_ShowInt8(80, 16, NRF_LastData.RV, 8);
        //
        // OLED_ShowString(0,  32, (u8 *)"Mode:", 8, 1);
        // OLED_ShowNum(40, 32, NRF_LastData.Mode, 1, 8, 1);
        //
        // if (K1_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(0,  48, (u8 *)"K1", 8, 1);
        // if (K2_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(24, 48, (u8 *)"K2", 8, 1);
        // if (K3_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(48, 48, (u8 *)"K3", 8, 1);
        // if (K4_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(72, 48, (u8 *)"K4", 8, 1);
        // if (K5_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(96, 48, (u8 *)"K5", 8, 1);
        // if (K6_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(0,  56, (u8 *)"K6", 8, 1);
        // if (K7_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(24, 56, (u8 *)"K7", 8, 1);
        // if (K8_PRESSED(NRF_LastData.KEY0))  OLED_ShowString(48, 56, (u8 *)"K8", 8, 1);
        /* ================================================================ */

        OLED_Refresh();

        osDelay(50);
    }
}
