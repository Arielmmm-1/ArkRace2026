#include "stm32f10x.h"
#include "Delay.h"
#include "stepper.h"
#include "Key.h"

int main(void)
{
	Stepper_Init();
	Key_Init();
	
	while (1)
	{
		uint8_t key = Key_GetNum();
		if(key == 1)
		{
			Step_Dir = 1;
			Step_Remain = 1600;  //半圈脉冲
		}
		if(key == 2)
		{
			Step_Dir = 0;
			Step_Remain = 1600;
		}

		if(Step_Remain > 0)
		{
			Stepper_SendOneStep(Step_Dir);
			Step_Remain--;
		}
	}
}
