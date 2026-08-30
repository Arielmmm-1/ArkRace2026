#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//引脚B0和A6
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1 ;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 ;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
uint8_t Key_GetNum(void)
{

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
	Delay_ms(20);//按下
    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);//如果一直按着还没松手
	Delay_ms(20);//松手都会抖动
	return 1;
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
	{
	Delay_ms(20);//按下
    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);//如果一直按着还没松手
	Delay_ms(20);//松手都会抖动
	return 2;
	}
	return  0;
}
