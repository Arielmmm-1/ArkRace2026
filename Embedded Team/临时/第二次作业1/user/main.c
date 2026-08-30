#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Time.h"
#include "Key.h"
#include "LED.h"
#include "Serial.h"

volatile int32_t num=1;
extern volatile uint8_t cmdSetNumFlag;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    OLED_Init();
	OLED_Clear();
	Time2_Init();
	Time3_Init();
	Serial_Init();
	Serial2_Init();
	Serial3_Init();
	Key_Init();
	LED_Init();
	
	
	
	while(1)
	{
	}
	
}
