#include "MotorPulse.h"

//步进电机PD0(En) PD1(Stp) PD2(Dir)
/**********************************************************
***	上电延时2秒等待Emm_V5.0闭环初始化完毕
**********************************************************/	
/*	WHILE循环发脉冲
		* 1.异或取反Stp6400次，也就是发送3200个脉冲给Emm_V5.0闭环
		* 2.延时1秒后取反Dir
		* 3.重复步骤1和2
		*	现象：顺时针转一圈 -> 停1秒 -> 逆时针转一圈 -> 停1秒 -> 如此循环...
*/
__IO int32_t i = 0;
__IO uint8_t cntDir = 0;

void MotorPulse(uint8_t dir, uint16_t pulse)
{
    if(dir)
    {
        GPIOD->BSRR = GPIO_PIN_2;
    }
    else
    {
        //高16位置1，实现引脚清零，替代BRR
        GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16;
    }

    for(uint16_t step = 0; step < pulse; step++)
    {
        for(uint16_t t = 0; t < 800; t++);
        GPIOD->ODR ^= GPIO_PIN_1;
    }
}