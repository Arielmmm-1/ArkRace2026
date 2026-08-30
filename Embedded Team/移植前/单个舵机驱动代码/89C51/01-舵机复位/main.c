/********************************************************
 *  STC89C51 单舵机 PWM 控制示例
 *  晶振：11.0592 MHz
 *  定时器 0 工作方式 1，产生 0.1 ms 基准时基
 *  P1.0 输出 20 ms 周期 PWM
 *  angle: 0°~180° 线性对应 0.5 ms~2.5 ms
 ********************************************************/
#include <REG51.H>

sbit SERVO = P1^0;          // 舵机信号脚

/* 根据晶振计算 0.1 ms 的 TH0/TL0 重装值 */
#define CRYSTAL   11059200L  // 当前晶振频率
#define PER_01MS  (65536 - CRYSTAL/12/10000)   // 0.1 ms 计数值
unsigned int reload = PER_01MS;

volatile unsigned int  high_01ms = 15;   // 1.5 ms 缺省中位
volatile unsigned int  cnt_01ms  = 0;    // 0.1 ms 计数器

/* ----------------------------------------------
 * 设置角度：0~max_angle 度
 * ----------------------------------------------*/
void set_servo_angle(unsigned char angle, unsigned char max_angle)
{
    if(angle > max_angle) angle = max_angle;

    /* 0° -> 5 个 0.1ms = 0.5 ms
       180° -> 25 个 0.1ms = 2.5 ms */
    high_01ms = 5 + (angle * 20)/180;
}

/* ----------------------------------------------
 * T0 中断服务：每 0.1 ms 进入一次
 * ----------------------------------------------*/
void timer0_ISR(void) interrupt 1
{
    TH0 = reload >> 8;
    TL0 = reload;

    if(++cnt_01ms >= 200)      // 20 ms 到，重新开始周期
        cnt_01ms = 0;

    SERVO = (cnt_01ms < high_01ms);
}

/* ----------------------------------------------
 * 初始化定时器 0
 * ----------------------------------------------*/
void timer0_init(void)
{
    TMOD &= 0xF0;              // T0 方式 1
    TMOD |= 0x01;

    TH0 = reload >> 8;
    TL0 = reload;

    ET0 = 1;                   // 开 T0 中断
    EA  = 1;                   // 开总中断
    TR0 = 1;                   // 启动 T0
}

/* ----------------------------------------------
 * 主函数：循环扫描串口或按键，改变角度
 * ----------------------------------------------*/
void main(void)
{
    timer0_init();
    set_servo_angle(90, 180);       // 先回到中位

    while(1)
    {

    }
}

