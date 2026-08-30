#include "Servo328.h"   // 使用我们封装的类

Servo328 servo1(9);   // 舵机 1 接 D9（Timer1，最稳）
Servo328 servo2(10);  // 舵机 2 接 D10（Timer1，最稳）

void setup()
{
  Serial.begin(115200);   // 与串口监视器波特率对应
  delay(1000);
  Serial.println(F("初始化舵机..."));

  servo1.setup();  // 分别初始化
  servo2.setup();
  delay(2000);     // 等待舵机到达中位
}

void loop()
{
  // 舵机 1：0° → 180° 平滑转动
  for (int angle = 0; angle <= 180; angle++) {
    servo1.setAngle(angle);
    Serial.print(F("舵机1: 角度="));
    Serial.println(angle);
    delay(15);   // 15 ms ≈ 66 Hz，视觉连贯
  }
  delay(1000);

  // 舵机 2：180° → 0° 平滑转动
  for (int angle = 180; angle >= 0; angle--) {
    servo2.setAngle(angle);
    Serial.print(F("舵机2: 角度="));
    Serial.println(angle);
    delay(15);
  }
  delay(1000);

  // 两个舵机同时转到指定位置
  Serial.println(F("两个舵机同时转动..."));
  servo1.setAngle(45);
  servo2.setAngle(135);
  delay(2000);
}