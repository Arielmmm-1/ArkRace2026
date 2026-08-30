#include <Arduino.h>

// 设置 PWM 信号的参数
const int freq = 50;       // 频率 (Hz)，舵机通常需要 50 Hz 的 PWM 信号，即 20 ms 的周期
const int channel = 8;     // PWM 通道。ESP32 有 16 个 PWM 通道（0-15）。
const int resolution = 16; // 提高分辨率到 16 位，以精确控制脉冲宽度
const int servoPin = 19;   // 定义连接舵机信号线的 GPIO 引脚

// 舵机中位对应的脉冲宽度，单位是微秒 (µs)
// 1500µs 是舵机的标准中位脉冲
const int servoMidPulse = 1500;

void setup() {
  Serial.begin(115200); // 初始化串口

  // 配置 PWM
  // 脉冲宽度由频率和分辨率共同决定。
  // 周期 = 1 / freq = 1 / 50Hz = 20ms = 20000µs
  // 最大计数值 = 2^resolution - 1 = 2^16 - 1 = 65535
  ledcSetup(channel, freq, resolution); 
  ledcAttachPin(servoPin, channel);     
  
  // 计算 1500µs 脉冲对应的占空比值
  // 占空比 = (脉冲宽度 / 周期) * 最大计数值
  int dutyCycle = (int)((float)servoMidPulse / 20000.0 * 65535.0);
  
  // 上电后立即将舵机转到中位
  ledcWrite(channel, dutyCycle);
  
  Serial.printf("上电后发送 1500µs 脉冲，占空比值: %d\n", dutyCycle);
  Serial.println("舵机已转至中位。");
}

void loop() {
  // 舵机位置不再改变
}