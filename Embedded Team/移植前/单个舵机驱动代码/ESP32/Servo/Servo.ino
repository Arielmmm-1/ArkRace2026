#include <Arduino.h>

// 设置 PWM 信号的参数
int freq = 50;       // 频率 (Hz)，舵机通常需要 50 Hz 的 PWM 信号，即 20 ms 的周期
int channel = 8;     // PWM 通道。ESP32 有 16 个 PWM 通道（0-15）。
                     // 我们使用通道 8，它是一个空闲通道。
int resolution = 8;  // PWM 分辨率。这里设置为 8 位，这意味着 PWM 占空比的值范围是 0 到 2^8-1 (0-255)。
const int servoPin = 19; // 定义连接舵机信号线的 GPIO 引脚，这里使用 GPIO 19

// 根据舵机的角度计算对应的 PWM 占空比
// 舵机的控制信号是一个脉冲宽度，通常在 500μs 到 2500μs 之间。
// 对于 50Hz 的频率 (20ms 周期)，占空比需要根据这个脉冲宽度来计算。
// 占空比 = (脉冲宽度 / 周期) * 占空比最大值
// 占空比最大值 = 2^resolution - 1 = 2^8 - 1 = 255
int calculatePWM(int degree, float ServoAngle) {
  // 舵机中位通常是 1500μs，但不同舵机可能略有不同。
  // 500μs (0.5ms) 对应最小角度，2500μs (2.5ms) 对应最大角度。
  // 最小脉冲宽度对应的占空比：(0.5ms / 20ms) * 255 ≈ 6.375，四舍五入为 6
  // 最大脉冲宽度对应的占空比：(2.5ms / 20ms) * 255 ≈ 31.875，四舍五入为 32
  const float deadZone = 6.4; // 对应 0.5ms 的占空比值
  const float max = 32.0;     // 对应 2.5ms 的占空比值
  
  // 确保输入的角度在 0 到 ServoAngle 度之间
  if (degree < 0) degree = 0;
  if (degree > ServoAngle) degree = ServoAngle;
  
  // 根据角度线性插值计算占空比
  // 映射关系：
  // 角度 0°  -> 占空比 deadZone
  // 角度 180° -> 占空比 max
  // 公式：( (max - deadZone) / ServoAngle) * degree + deadZone
  return (int)(((max - deadZone) / ServoAngle) * degree + deadZone);
}

void setup() {
  Serial.begin(115200); // 初始化串口，波特率为 9600
  
  // 配置 PWM
  ledcSetup(channel, freq, resolution); // 设置 PWM 通道 (channel), 频率 (freq) 和分辨率 (resolution)
  ledcAttachPin(servoPin, channel);     // 将指定的 GPIO 引脚 (servoPin) 绑定到 PWM 通道 (channel)
}

void loop() {
  // 循环让舵机从 0 度到 180 度，每隔 10 度转一次
  for (int d = 0; d <= 270; d += 10) {
    // 调用 ledcWrite 函数，将计算出的占空比值写入指定的 PWM 通道，从而控制舵机
    ledcWrite(channel, calculatePWM(d, 270));
    
    // 通过串口打印当前角度和计算出的占空比值，方便调试
    Serial.printf("value=%d, calcu=%d\n", d, calculatePWM(d, 270));
    
    delay(1000); // 延时 1 秒，让舵机有足够时间转到新位置
  }
}