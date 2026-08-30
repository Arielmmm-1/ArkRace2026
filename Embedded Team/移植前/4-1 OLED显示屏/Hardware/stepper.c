#include "stepper.h"
#include "Delay.h"

uint32_t Step_Remain = 0;
uint8_t Step_Dir = 0;

void Stepper_Init(void)
{
    GPIO_InitTypeDef gpio_cfg;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpio_cfg.GPIO_Pin = STEP_PIN | DIR_PIN | EN_PIN;
    gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STEP_PORT, &gpio_cfg);
    GPIO_SetBits(STEP_PORT, EN_PIN);
}

void Stepper_SendOneStep(uint8_t dir)
{
    GPIO_ResetBits(STEP_PORT, EN_PIN);
    if(dir)
        GPIO_SetBits(STEP_PORT, DIR_PIN);
    else
        GPIO_ResetBits(STEP_PORT, DIR_PIN);

    GPIO_SetBits(STEP_PORT, STEP_PIN);
    Delay_us(100);
    GPIO_ResetBits(STEP_PORT, STEP_PIN);
    Delay_us(100);

    if(Step_Remain == 0)
    {
        GPIO_SetBits(STEP_PORT, EN_PIN);
    }
}
