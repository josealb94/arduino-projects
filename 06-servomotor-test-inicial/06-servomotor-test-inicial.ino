#include <Servo.h>

const int PIN_SERVO = 2;
const int MAX_PULSE = 1000;
const int MIN_PULSE = 2000;

Servo servo1;


void setup() {
  servo1.attach(PIN_SERVO, MIN_PULSE, MAX_PULSE);
}

void loop() {
  servo1.write(0);
  delay(5000);
  servo1.write(180);
  delay(5000);
}
