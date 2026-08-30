#include <stdint.h>
#include <string.h>
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "Key.h"
#include "NRF24L01.h"

// 四路摇杆AD原始值
uint16_t AD_LH, AD_LV, AD_RH, AD_RV;
// 归一化摇杆输出：-100 ~ 100
int8_t LH, LV, RH, RV;

// 按键定义：Bit0=大急停 Bit1=抓取 Bit2=放球 Bit3=坡道复位 Bit4=界面切换
uint8_t KEY0 = 0;
uint8_t Mode = 0;

// 定时计数器
uint32_t MainTick = 0;
uint32_t ScreenTick = 0;
#define AD_DEAD     100
#define FONT_SIZE   16
#define SEND_PERIOD 50    // 50ms低延迟操控发包
#define SCREEN_PERIOD 50

// 无线通信
uint8_t SendBuf[10];
uint8_t BufIdx = 0;
uint8_t SuccessRate = 0;
uint8_t FrameCnt = 0;
uint8_t LoseCnt = 0;
uint8_t IsLose = 0;

// ========== 赛事专属回传变量（修复数组长度+英文防乱码） ==========
char BaseBox[10] = "NoID";    // 专属编码：基础得分筐 Red/Green
char CoreBox[10] = "NoID";    // 共用编码：核心争夺筐 Blue
uint8_t GrabType = 0;         // 0无球 1基础球 2核心球
uint16_t HoldTime = 0;        // 抓取剩余保持时间
uint8_t OnSlope = 0;          // 是否在坡道上
uint8_t CanPutBall = 0;       // 是否可以投球入筐

// 函数声明
int8_t DataProcess(uint16_t ad);
void OLED_SignedNum(uint8_t x, uint8_t y, int8_t num, uint8_t len);
uint8_t GetSuccessRate(uint8_t sta);
uint8_t KEY_GetVal(void);

int main(void)
{
    OLED_Init();
    Key_Init();
    AD_Init();
    NRF24L01_Init();

    // 开机初始化界面
    OLED_Clear();
    OLED_ShowString(0, 0, "DongGuan Robot Match", FONT_SIZE);
    OLED_ShowString(0, 16, "RC System Ready", FONT_SIZE);
    Delay_ms(1000);
    OLED_Clear();

    while(1)
    {
        MainTick++;
        ScreenTick++;

        // 1、读取摇杆AD并滤波处理
        AD_LH = AD_GetValue(0);
        AD_LV = AD_GetValue(1);
        AD_RH = AD_GetValue(2);
        AD_RV = AD_GetValue(3);
        LH = DataProcess(AD_LH);
        LV = DataProcess(AD_LV);
        RH = DataProcess(AD_RH);
        RV = DataProcess(AD_RV);

        // 2、读取防抖按键
        uint8_t key_val = KEY_GetVal();
        KEY0 |= key_val;

        // 界面切换按键
        if(key_val & 0x10)
        {
            Mode = !Mode;
            OLED_Clear();
        }

        // ========== 赛事硬性安全：大急停最高优先级锁死 ==========
        if((KEY0 & 0x01) || IsLose)
        {
            LH = 0; LV = 0; RH = 0; RV = 0;
        }

        // 3、定时无线发送（7字节标准协议，下位机一一对应解析）
        if(MainTick >= SEND_PERIOD)
        {
            MainTick = 0;
            FrameCnt++;
            NRF24L01_TxPacket[0] = FrameCnt;
            NRF24L01_TxPacket[1] = LH;
            NRF24L01_TxPacket[2] = LV;
            NRF24L01_TxPacket[3] = RH;
            NRF24L01_TxPacket[4] = RV;
            NRF24L01_TxPacket[5] = KEY0;

            uint8_t SendSta = NRF24L01_Send();
            SuccessRate = GetSuccessRate(SendSta);

            if(SendSta == 1)
            {
                LoseCnt = 0;
                IsLose = 0;
                KEY0 &= ~((1<<1)|(1<<2)|(1<<3)); // 抓取/放球/坡道按键自动清零
            }
            else
            {
                LoseCnt++;
                if(LoseCnt >= 10) IsLose = 1;
            }
        }

        // 4、接收下位机回传【赛事二维码+任务状态数据】
        if(NRF24L01_Receive() == 1)
        {
            LoseCnt = 0;
            IsLose = 0;
            uint8_t *rx = NRF24L01_RxPacket;
            // rx[0]：基础筐颜色 0=Red红 / 1=Green绿
            if(rx[0]==0) strcpy(BaseBox,"Red");
            else if(rx[0]==1) strcpy(BaseBox,"Green");
            else strcpy(BaseBox,"Unknown");
            // 核心争夺筐固定Blue蓝色
            strcpy(CoreBox,"Blue");
            // rx[2]：持球类型 0无球/1基础球/2核心球
            GrabType = rx[2];
            // rx[3]：剩余保持时长（ms）
            HoldTime = rx[3];
            // rx[4]：坡道状态 0平地/1坡道
            OnSlope = rx[4];
            // rx[5]：是否满足投球条件
            CanPutBall = rx[5];
        }

        // 5、低频刷新屏幕，防闪烁，裁判清晰查看
        if(ScreenTick >= SCREEN_PERIOD)
        {
            ScreenTick = 0;
            OLED_Clear();
            // 第一行：通信状态+掉线警告
            OLED_ShowString(0, 0, "Sig:", FONT_SIZE);
            if(SuccessRate >= 8) OLED_ShowString(24, 0, "Good", FONT_SIZE);
            else if(SuccessRate >= 4) OLED_ShowString(24, 0, "Mid", FONT_SIZE);
            else OLED_ShowString(24, 0, "Weak", FONT_SIZE);
            if(IsLose) OLED_ShowString(48, 0, "LOST!", FONT_SIZE);

            if(Mode == 0)
            {
                // 模式0：操控界面（摇杆+急停状态，裁判检查安全开关）
                OLED_ShowString(0, 16, "LH:", FONT_SIZE);
                OLED_SignedNum(20, 16, LH, 3);
                OLED_ShowString(48, 16, "LV:", FONT_SIZE);
                OLED_SignedNum(68, 16, LV, 3);

                OLED_ShowString(0, 32, "RH:", FONT_SIZE);
                OLED_SignedNum(20, 32, RH, 3);
                OLED_ShowString(48, 32, "RV:", FONT_SIZE);
                OLED_SignedNum(68, 32, RV, 3);

                if((KEY0 & 0x01) || IsLose)
                    OLED_ShowString(0, 48, "EMERGENCY STOP", FONT_SIZE);
                else
                    OLED_ShowString(0, 48, "RUN NORMAL", FONT_SIZE);
            }
            else
            {
                // 模式1：赛事核验主界面（基础筐+核心筐顺序展示）
                OLED_ShowString(0, 16, "BaseBox:", FONT_SIZE);
                OLED_ShowString(48, 16, BaseBox, FONT_SIZE);

                OLED_ShowString(0, 32, "CoreBox:", FONT_SIZE);
                OLED_ShowString(48, 32, CoreBox, FONT_SIZE);

                // 抓取状态+保持时长（基础3s/核心5s）
                if(GrabType == 0)
                    OLED_ShowString(0, 48, "Grab:None", FONT_SIZE);
                else if(GrabType == 1)
                {
                    OLED_ShowString(0, 48, "BaseBall:", FONT_SIZE);
                    OLED_SignedNum(52, 48, HoldTime/1000, 1);
                }
                else
                {
                    OLED_ShowString(0, 48, "CoreBall:", FONT_SIZE);
                    OLED_SignedNum(56, 48, HoldTime/1000, 1);
                }

                // 坡道+投球状态
                OLED_ShowString(0, 60, OnSlope?"OnSlope":"Flat", FONT_SIZE);
                OLED_ShowString(48, 60, CanPutBall?"CanPut":"NoPut", FONT_SIZE);
            }
        }
        Delay_ms(1);
    }
}

// AD去死区+映射-100~100+限幅防越界
int8_t DataProcess(uint16_t ad)
{
    int32_t val = (int32_t)ad - 2048;
    if(val > AD_DEAD) val -= AD_DEAD;
    else if(val < -AD_DEAD) val += AD_DEAD;
    else val = 0;
    val = val * 100 / (2048 - AD_DEAD);
    if(val > 100) val = 100;
    if(val < -100) val = -100;
    return (int8_t)val;
}

// 带正负号数字显示
void OLED_SignedNum(uint8_t x, uint8_t y, int8_t num, uint8_t len)
{
    if(num < 0)
    {
        OLED_ShowString(x, y, "-", FONT_SIZE);
        OLED_ShowNum(x+12, y, (uint8_t)(-num), len-1, FONT_SIZE);
    }
    else
    {
        OLED_ShowNum(x, y, (uint8_t)num, len, FONT_SIZE);
    }
}

// 统计无线通信成功率
uint8_t GetSuccessRate(uint8_t sta)
{
    SendBuf[BufIdx] = sta;
    BufIdx = (BufIdx + 1) % 10;
    uint8_t cnt = 0;
    for(uint8_t i=0; i<10; i++) if(SendBuf[i]==1) cnt++;
    return cnt;
}

// 外部Key.c实现防抖按键
uint8_t KEY_GetVal(void)
{
    return 0;
}




