#ifndef __STEPPER_H
#define __STEPPER_H
#include "stm32f10x.h"

#define STEP_PIN    GPIO_Pin_1
#define DIR_PIN     GPIO_Pin_0
#define EN_PIN      GPIO_Pin_2
#define STEP_PORT   GPIOA

void Stepper_Init(void);
//单次只输出一个脉冲，不阻塞
void Stepper_SendOneStep(uint8_t dir);

extern uint32_t Step_Remain;
extern uint8_t Step_Dir;

#endif
