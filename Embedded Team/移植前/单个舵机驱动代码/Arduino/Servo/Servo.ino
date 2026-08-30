/*
 *  ATmega328P 舵机示例
 *  功能：从 0° 走到 270°（或你自己设置的最大角度），再回到 0°，循环
 *  硬件：舵机信号 → D9；红色 → 5 V；棕色 → GND
 */

#include <Servo.h>

const uint8_t  SERVO_PIN  = 9;     // 舵机信号引脚
const uint16_t MAX_ANGLE  = 270;   // 舵机实际最大角度
const uint16_t STEP_ANGLE = 10;    // 步进角度

Servo myServo;

// 把 0…MAX_ANGLE 映射到 500 µs … 2500 µs
int angleToPulse(int angle)
{
  angle = constrain(angle, 0, MAX_ANGLE);
  return map(angle, 0, MAX_ANGLE, 500, 2500); // 单位 µs
}

void setup()
{
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);                // 绑定到 D9
  myServo.writeMicroseconds(1500);          // 先回中
  delay(1000);
}

void loop()
{
  // 0 → MAX_ANGLE
  for (int a = 0; a <= MAX_ANGLE; a += STEP_ANGLE) {
    int pulse = angleToPulse(a);
    myServo.writeMicroseconds(pulse);
    Serial.print("Angle = "); Serial.print(a);
    Serial.print(" deg  Pulse = "); Serial.print(pulse);
    Serial.println(" us");
    delay(1000);
  }

  // MAX_ANGLE → 0
  for (int a = MAX_ANGLE; a >= 0; a -= STEP_ANGLE) {
    int pulse = angleToPulse(a);
    myServo.writeMicroseconds(pulse);
    Serial.print("Angle = "); Serial.print(a);
    Serial.print(" deg  Pulse = "); Serial.print(pulse);
    Serial.println(" us");
    delay(1000);
  }
}