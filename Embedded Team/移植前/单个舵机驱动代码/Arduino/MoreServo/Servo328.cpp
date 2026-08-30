#include "Servo328.h"

// 构造函数：仅保存参数，还未 attach
Servo328::Servo328(uint8_t pin, int minAngle, int maxAngle)
  : _pin(pin),
    _minAngle(minAngle),
    _maxAngle(maxAngle),
    _currentAngle((minAngle + maxAngle) / 2) {}   // 默认中位

// 初始化：真正把定时器通道绑定到引脚
void Servo328::setup()
{
  _servo.attach(_pin);          // 官方库默认 544~2400 µs
  _servo.write(_currentAngle);  // 上电后先转到中位
}

// 设定角度，带安全裁剪
void Servo328::setAngle(int angle)
{
  if (angle < _minAngle) angle = _minAngle;
  if (angle > _maxAngle) angle = _maxAngle;

  _servo.write(angle);  // 官方库角度转脉冲
  _currentAngle = angle;
}

// 返回最近一次 setAngle 的值（官方库并不提供）
int Servo328::getAngle() const
{
  return _currentAngle;
}