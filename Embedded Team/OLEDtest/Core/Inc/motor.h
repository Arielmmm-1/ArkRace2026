#ifndef __MOTOR_H
#define __MOTOR_H

#include "tim.h"
#include "gpio.h"

#define PWM_MAX 300
#define PWM_MIN -300

void Motor_Load(int16_t moto1, int16_t moto2, int16_t moto3, int16_t moto4);
void Motor_Limit(int *val);
void Stop(float *Med_Jiaodu,float *Jiaodu);
#endif
