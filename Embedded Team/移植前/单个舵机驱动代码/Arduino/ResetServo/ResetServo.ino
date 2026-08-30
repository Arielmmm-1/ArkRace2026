/*
 *  ResetServo.ino  ——  ATmega328P 版本
 *  上电后把舵机一次性转到 1500 µs（中位），然后保持
 *  接线：舵机信号 → D9；红色 → 5 V；棕色 → GND
 */

#include <Servo.h>

const uint8_t SERVO_PIN = 9;   // 舵机信号引脚（可改 3, 5, 6, 9, 10, 11）
const uint16_t MID_PULSE = 1500; // 1500 µs = 中位

Servo myServo;

void setup()
{
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);        // 绑定舵机到引脚
  myServo.writeMicroseconds(MID_PULSE); // 直接输出 1500 µs
  Serial.println(F("Power-up: servo moved to 1500 us (center)."));
}

void loop()
{
  // 不再改变舵机位置
}