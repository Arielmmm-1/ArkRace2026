#include "board.h"
#include "delay.h"

/**********************************************************
***	Emm_V5.0步进闭环控制例程
***	编写作者：ZHANGDATOU
***	技术支持：张大头闭环伺服
***	淘宝店铺：https://zhangdatou.taobao.com
***	CSDN博客：http s://blog.csdn.net/zhangdatou666
***	qq交流群：262438510
**********************************************************/

void Motor_Run_OneCircle(uint8_t dir)
{
    int32_t i = 0;
    // 设置方向引脚 PA7
    if(dir == 0)
    {
        GPIOA->ODR &= ~GPIO_Pin_7;  // 顺时针
    }
    else
    {
        GPIOA->ODR |= GPIO_Pin_7;   // 逆时针
    }
    
    // 输出3200个脉冲 = 完整一圈
    for(i = 0; i < 3200; i++)
    {
        delay_cnt(3200);
        GPIOA->ODR ^= GPIO_Pin_6;
    }
}

/**
	*	@brief		MAIN函数
	*	@param		无
	*	@retval		无
	*/
	
int main(void)
{
	__IO int32_t i = 0; __IO bool cntDir = false;

/**********************************************************
***	初始化板载外设
**********************************************************/
	board_init();

/**********************************************************
***	上电延时2秒等待Emm_V5.0闭环初始化完毕
**********************************************************/	
	delay_ms(2000);

/**********************************************************
***	WHILE循环发脉冲
		* 1.异或取反PA6(Stp)6400次，也就是发送3200个脉冲给Emm_V5.0闭环
		* 2.延时1秒后取反PA7(Dir)
		* 3.重复步骤1和2
		*	现象：顺时针转一圈 -> 停1秒 -> 逆时针转一圈 -> 停1秒 -> 如此循环...
**********************************************************/	
	while(1)
	{
		// 软件延时
//		delay_cnt(3200);

//		// 异或取反PA6(Stp)
//		GPIOA->ODR ^= GPIO_Pin_6;

//		// 计数
//		if(cntDir) {--i;}	else {++i;}

//		// 取反PA7(Dir)，切换电机旋转方向
//		if(i == 0 || i == 6400) { delay_ms(1000); GPIOA->ODR ^= GPIO_Pin_7; cntDir = !cntDir; }
		        // 正转一圈
        Motor_Run_OneCircle(0);
        delay_ms(1000); // 停顿1秒
		        // 正转一圈
        Motor_Run_OneCircle(0);
        delay_ms(1000); // 停顿1秒
		Motor_Run_OneCircle(1);
        delay_ms(1000);
		Motor_Run_OneCircle(1);
        delay_ms(1000);
	}
}
