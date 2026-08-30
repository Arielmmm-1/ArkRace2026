#include "Servo.h"

// 创建两个Servo对象
// 第一个舵机：通道0，引脚18
Servo servo1(0, 18);
// 第二个舵机：通道1，引脚19
Servo servo2(1, 19);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("初始化舵机...");
  // 调用每个舵机对象的setup方法进行初始化
  servo1.setup();
  servo2.setup();
  
  delay(2000); // 等待舵机到达中位
}

void loop() {
  // 舵机1从0度转到180度
  for (int angle = 0; angle <= 180; angle++) {
    servo1.setAngle(angle);
    Serial.printf("舵机1: 角度=%d\n", servo1.getAngle());
    delay(15);
  }
  delay(1000);

  // 舵机2从180度转到0度
  for (int angle = 180; angle >= 0; angle--) {
    servo2.setAngle(angle);
    Serial.printf("舵机2: 角度=%d\n", servo2.getAngle());
    delay(15);
  }
  delay(1000);
  
  // 两个舵机同时转到45度和135度
  Serial.println("两个舵机同时转动...");
  servo1.setAngle(45);
  servo2.setAngle(135);
  delay(2000);
}
