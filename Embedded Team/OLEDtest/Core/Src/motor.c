#include "motor.h"
#include "tim.h"
uint8_t stop;
int abs(int p)
{
	if(p > 0)
		return p;
	else
		return -p;
}

void Motor_Load(int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4)
{
    uint16_t p1, p2, p3, p4;

    // ========== 电机1 LF PD6(AIN1) PD7(AIN2) ==========
    if(moto1 < 0)
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
        p1 = (uint16_t)(-moto1);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
        p1 = (uint16_t)moto1;
    }

    // ========== 电机2 RF PA4(BIN1) PA6(BIN2) ==========
    if(moto2 < 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        p2 = (uint16_t)(-moto2);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        p2 = (uint16_t)moto2;
    }

    // ========== 电机3 LB PB0(AIN1) PB1(AIN2) ==========
    if(moto3 < 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
        p3 = (uint16_t)(-moto3);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
        p3 = (uint16_t)moto3;
    }

    // ========== 电机4 RB PA5(BIN1) PB6(BIN2) ==========
    if(moto4 < 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        p4 = (uint16_t)(-moto4);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        p4 = (uint16_t)moto4;
    }


    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, p1);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, p2);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, p3);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, p4);
}

// 限幅函数
void Motor_Limit(int *val)
{
	if(*val > PWM_MAX) *val = PWM_MAX;
	if(*val < PWM_MIN) *val = PWM_MIN;
}

//阈值停止
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(abs((int)(*Jiaodu-*Med_Jiaodu))>60)
	{
		Motor_Load(0,0,0,0);
		stop=1;
	}
}
