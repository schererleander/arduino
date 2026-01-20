#include "ESP32Servo.h"

Servo base;
Servo shoulder;
Servo elbow;

void setup() {
  base.attach(15);
  shoulder.attach(2);
  elbow.attach(4);
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) {
    base.write(pos);
    shoulder.write(pos);
    elbow.write(pos);
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos -= 1) {
    base.write(pos);
    shoulder.write(pos);
    elbow.write(pos);
    delay(15);
  }
}
