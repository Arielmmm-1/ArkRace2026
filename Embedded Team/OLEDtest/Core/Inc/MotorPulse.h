#ifndef __MOTORPULSE_H
#define __MOTORPULSE_H

#include "main.h"

extern __IO int32_t i;
extern __IO uint8_t cntDir;

void MotorPulse(uint8_t dir, uint16_t pulse);

#endif
