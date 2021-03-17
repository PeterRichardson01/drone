
#include <Servo.h>


Servo motor1;
const int motor1Pin = 9;

void setup() {
  motor1.attach(motor1Pin);

  motor1.writeMicroseconds(1000);
  delay(5000);
  for(int i = 1; i < 9; i++)
  {
    motor1.writeMicroseconds(1000 + 100*i);
    delay(1000);
  }
  motor1.writeMicroseconds(1000);
}

void loop() {
  
}
