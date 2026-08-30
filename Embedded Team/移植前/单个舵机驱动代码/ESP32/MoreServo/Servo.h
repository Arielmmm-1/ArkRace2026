#ifndef _SERVO_H
#define _SERVO_H

#include <Arduino.h>

/**
 * @class Servo
 * @brief 封装了ESP32上使用LEDC控制舵机的所有功能。
 * * 这个类简化了舵机的使用，将底层的PWM配置和角度计算
 * 抽象为简单的setAngle()方法。
 */
class Servo {
public:
  /**
   * @brief 构造函数，创建Servo对象。
   * * @param channel PWM通道 (0-15)。
   * @param pin 舵机信号线连接的GPIO引脚。
   * @param minAngle 舵机最小角度。
   * @param maxAngle 舵机最大角度。
   */
  Servo(int channel, int pin, int minAngle = 0, int maxAngle = 180);

  /**
   * @brief 初始化舵机并设置PWM参数。
   * * @param freq PWM频率 (Hz)，默认为50Hz。
   * @param resolution PWM分辨率，默认为16位。
   */
  void setup(int freq = 50, int resolution = 16);

  /**
   * @brief 设置舵机角度。
   * * @param angle 目标角度，范围由构造函数定义。
   */
  void setAngle(int angle);

  /**
   * @brief 获取当前舵机角度。
   * * @return 当前角度值。
   */
  int getAngle() const;

private:
  int _channel;      // 舵机使用的PWM通道
  int _pin;          // 舵机连接的GPIO引脚
  int _minAngle;     // 舵机最小角度
  int _maxAngle;     // 舵机最大角度

  int _currentAngle; // 舵机当前角度
  int _freq;         // PWM频率
  int _resolution;   // PWM分辨率
  int _maxDuty;      // PWM最大占空比值 (2^resolution - 1)
  int _cycle;        // PWM周期 (µs)

  /**
   * @brief 将角度转换为占空比值。
   * * @param angle 角度值。
   * @return 对应的占空比值。
   */
  int _angleToDuty(int angle) const;
};

#endif // _SERVO_H
