#ifndef _SERVO328_H          // 防止头文件被重复包含
#define _SERVO328_H

#include <Arduino.h>
#include <Servo.h>          // 引入官方 AVR Servo 库

/**
 * @class Servo328
 * @brief 为 ATmega328P 平台封装的舵机控制类
 *        内部调用官方 Servo 库，对外提供简洁接口
 */
class Servo328
{
public:
  /**
   * @brief 构造函数
   * @param pin       舵机信号线连接的引脚（推荐 9/10）
   * @param minAngle  可设置的最小角度，默认 0°
   * @param maxAngle  可设置的最大角度，默认 180°
   */
  Servo328(uint8_t pin, int minAngle = 0, int maxAngle = 180);

  /**
   * @brief 初始化舵机，执行 attach
   */
  void setup();

  /**
   * @brief 设置舵机角度
   * @param angle 目标角度，会被裁剪到 [minAngle, maxAngle]
   */
  void setAngle(int angle);

  /**
   * @return 当前缓存的角度值
   */
  int getAngle() const;

private:
  uint8_t _pin;         // 信号引脚
  int _minAngle;        // 软件角度下限
  int _maxAngle;        // 软件角度上限
  int _currentAngle;    // 最近一次写入的角度

  ::Servo _servo;       // 官方库实例，前面加 :: 避免名字冲突
};

#endif