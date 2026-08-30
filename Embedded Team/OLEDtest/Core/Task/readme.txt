/*----------------电机--------------*/
TIM8的 四通道PC6(CH1),7(CH2),8(CH3),9(CH4)PWM输出通道
1.电机1编码器:TIM3CH1=PB4、 CH2=PB5
2.电机2编码器:TIM4CH1=PD12、CH2=PD13
3.电机3编码器:TIM5CH1=PA0、CH2=PA1
4.电机4编码器:TIM1一CH1=PE9,CH2=PE11

TIM7做PID定时周期

TB6612-1
电机1-AIN1 = PD6, AIN2 = PD7
电机3-BIN1 = PB0, BIN2 = PB1

TB6612-2
电机2-AIN2 = PA4, AIN2 = PA6
电机4-BIN1 = PA5, BIN2 = PB6
/*--------------------------------*/


/*-------------------------------*/

步进电机
PD1(Stp)
PD2(Dir)

OLED
OLED_SCL- PB10
OLED_SDA- PB11

NRF24
CE → PB12
CSN → PB13
SCK → PB14
MOSI → PB15
MISO → PD9

夹爪舵机
PE5

附加测试：
裁判系统？