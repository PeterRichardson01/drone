
void loop() {
  unsigned long tStart = micros();

  for(int i = 4;i > 0; i--)
  {
    alt[i] = alt[i-1];
  }

  if (! bmp.performReading())
  {
    desalt = 0.0;
    alt[0] = 0.1;
  }
  else
  {
    alt[0] = (alt[1] + alt[2] + alt[3] + alt[4] + bmp.readAltitude(bmpOffset))/5.0;
  }

  sensors_event_t event;
  bno.getEvent(&event);

  if(millis() < 9500)
  {
    desalt = 0.1;
  }
  else
  {
    desalt = -1.0;
  }

  double thrust = thrustCTL.calculate(desalt, alt[0]);
  double pitch = pitchCTL.calculate(0, event.orientation.y);
  double roll  = rollCTL.calculate (0, event.orientation.z);
  double yaw;
  if(event.orientation.x < 180)
  {
    yaw = yawCTL.calculate(0, event.orientation.x);
  }
  else
  {
    yaw = yawCTL.calculate(0, event.orientation.x - 360.0);
  }
  /*
  //front motors = thrust - pitch
  //back motors = thrust + pitch
  //left motors = thrust + roll
  //right motors = thrust - roll
  
  
  Serial.println(millis());
  */

  
  if(millis() < 0)//10500)
  {
    //Motors UL-UR-BL-BR --> 1-2-3-4
    motor1.writeMicroseconds(1320 - pitch + roll + yaw);
    motor2.writeMicroseconds(1320 - pitch - roll - yaw);
    motor3.writeMicroseconds(1320 + pitch + roll - yaw);
    motor4.writeMicroseconds(1320 + pitch - roll + yaw);

    /*
    Serial.print(event.orientation.x);
    Serial.print("\t");
    Serial.println(yaw);
    
    Serial.println(SAMPLERATE/1000.0);
    Serial.println(yawCTL.getDT());
    Serial.println(yawCTL.getinteg());
    */
  }/*
  else
  {
    motor1.writeMicroseconds(1000);
    motor2.writeMicroseconds(1000);
    motor3.writeMicroseconds(1000);
    motor4.writeMicroseconds(1000);
  }*/

  /*
  Serial.print(millis());
  Serial.print("\t");
  Serial.print(alt[0]);
  Serial.print("\t");
  Serial.println(bmp.readAltitude(bmpOffset));
  */
  Serial.print(pitch);
  Serial.print("\t");
  Serial.println(roll);
  while((micros() - tStart) < (SAMPLERATE * 1000)){}
}
