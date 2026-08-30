#include "Servo.h"

// 构造函数
Servo::Servo(int channel, int pin, int minAngle, int maxAngle)
  : _channel(channel), 
    _pin(pin),
    _minAngle(minAngle),
    _maxAngle(maxAngle),
    _currentAngle(0) {
}

// 初始化舵机，设置PWM参数
void Servo::setup(int freq, int resolution) {
  _freq = freq;
  _resolution = resolution;
  // 计算16位分辨率下的最大占空比值 (2^16 - 1)
  _maxDuty = (1 << resolution) - 1;
  // 计算一个PWM周期的微秒数 (1,000,000µs / 50Hz = 20,000µs)
  _cycle = 1000000 / freq;

  // 配置PWM通道、频率和分辨率
  ledcSetup(_channel, _freq, _resolution);
  // 将PWM通道与指定的GPIO引脚绑定
  ledcAttachPin(_pin, _channel);
  
  // 上电后将舵机转到中位
  setAngle((_minAngle + _maxAngle) / 2);
}

// 设置舵机角度
void Servo::setAngle(int angle) {
  // 确保输入的角度在有效范围内
  if (angle < _minAngle) {
    angle = _minAngle;
  }
  if (angle > _maxAngle) {
    angle = _maxAngle;
  }
  
  // 将角度转换为占空比值并输出
  int duty = _angleToDuty(angle);
  ledcWrite(_channel, duty);

  _currentAngle = angle;
}

// 获取当前舵机角度
int Servo::getAngle() const {
  return _currentAngle;
}

// 内部函数：将角度转换为占空比值
int Servo::_angleToDuty(int angle) const {
  // 舵机通常的脉冲宽度范围是 500µs (0度) 到 2500µs (180度)
  const int minPulse = 500;
  const int maxPulse = 2500;

  // 使用map函数将角度映射到脉冲宽度
  int pulseWidth = map(angle, _minAngle, _maxAngle, minPulse, maxPulse);
  
  // 将脉冲宽度转换为占空比值
  return (int)((float)pulseWidth / _cycle * _maxDuty);
}
