#include "stm32f10x.h"   // Device header
#include "LED.h"
//如果要跨文件使用变量,第一种方法
extern volatile int32_t num;
uint16_t flag=1;

volatile uint8_t currentState = 0; // 当前状态：0=等待下一秒, 1-3=第几个1秒周期
volatile uint8_t blinkCount = 0;   // 当前秒内已亮灭的次数
volatile uint16_t timeBase_ms = 0; // 时间基准(ms)



void Time2_Init(void)
 {   //开启内部时钟
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);


	 
	
	 //1秒进一次中断,用于控制LED亮灭
	 //通过ETR引脚的外部时钟模式2配置,第三个参数是外部触发的极性
	 //TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x03);
	 //配置时基单元
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数模式
	 TIM_TimeBaseStructure.TIM_Period=99;//范围是0-65535，和计数器有一个数的偏差
	 TIM_TimeBaseStructure.TIM_Prescaler=7199;//10ms
	 TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	 
	
	 TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	 TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	 //使能更新中断
	 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

 
// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; 
 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//相同抢占优先级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//响应优先级
 NVIC_Init(&NVIC_InitStructure);
 
 
 //启动定时器
 TIM_Cmd(TIM2,DISABLE);

 
 }
 void Time3_Init(void)
 {   //开启内部时钟
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	 

	 
	 
	 //配置时基单元
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数模式
	 TIM_TimeBaseStructure.TIM_Period=9999;//范围是0-65535，和计数器有一个数的偏差
	 TIM_TimeBaseStructure.TIM_Prescaler=7199;//1s
	 TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	 TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	 TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	 //使能更新中断
	 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

 
 
 
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
 NVIC_Init(&NVIC_InitStructure);
 //启动定时器
 TIM_Cmd(TIM3,DISABLE);

 
 }
 
void TIM2_IRQHandler(void)
{
	//获取中断标志位
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        
        // 每10ms进入一次，累计100次为1秒
        timeBase_ms += 10;
        
        if(timeBase_ms >= 1000)  // 进入新的一秒
        {
            timeBase_ms = 0;
            currentState++;
            if(currentState > 3) currentState = 1;  // 循环1,2,3
            
            blinkCount = 0;  // 重置亮灭计数
        }
        
        // 根据当前状态决定亮灭次数
        uint8_t targetBlinks = currentState;  // 状态1=1次，状态2=2次，状态3=3次
        
        if(blinkCount < targetBlinks)
        {
            // 翻转 LED
           LED2_Turn();
            blinkCount++;
        }
        else if(blinkCount == targetBlinks)
        {
            // 完成当前秒的亮灭，保持灭的状态直到下一秒
            LED2_OFF();
            blinkCount++;  // 防止重复翻转
        }
	}
	
}

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
//		TIM_Cmd(TIM3, DISABLE);
//		TIM_SetCounter(TIM3, 0);
//		TIM_PrescalerConfig(TIM2, psc, TIM_PSCReloadMode_Immediate);
//		TIM_Cmd(TIM3, ENABLE);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		 // 示例：让 num 在 0,1,2,3 之间循环，从而改变 LED 闪烁频率
        // 实际应用中可通过按键、串口等方式修改 num
//        static uint8_t toggle = 0;
//        toggle++;
//        if(toggle > 3) toggle = 0;
//        num = toggle;
       
        // 同时可以在这里修改 TIM2 的频率（如果需要动态调整 LED 闪烁速度）
        // 例如通过 __HAL_TIM_SET_AUTORELOAD(TIM2, new_period);
	}
	
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
	
}
