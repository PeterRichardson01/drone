
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

  if(millis()>10000)
  {
    desalt = desalt - 0.5;
  }

  long ch1, ch2, ch3, ch4;
  ch1 = pulseIn(CH1PIN, HIGH); //altitude
  ch2 = pulseIn(CH2PIN, HIGH); //yaw
  ch3 = pulseIn(CH3PIN, HIGH); //pitch
  ch4 = pulseIn(CH4PIN, HIGH); //roll

  //channel values now between 1000 min and 2000 max
  
  desalt = desalt - (double(ch1) - 1000.0)/500.0*SAMPLERATE/1000.0 //changes within 1 m/s
  despitch = (double(ch3) - 1000.0)/100.0; //angle within +/- 5 degrees
  desroll = (double(ch4) - 1000.0)/100.0; //angle within +/- 5 degrees
  desyaw = (double(ch2) - 1500.0)*150.0/500.0; // angle within +/- 150 degrees

  double thrust = thrustCTL.calculate(desalt, alt[0]);
  double pitch = pitchCTL.calculate(despitch, event.orientation.y);
  double roll  = rollCTL.calculate (desroll, event.orientation.z);
  double yaw;
  if(event.orientation.x < 180.0)
  {
    yaw = yawCTL.calculate(desyaw, event.orientation.x);
  }
  else
  {
    yaw = yawCTL.calculate(desyaw, event.orientation.x - 360.0);
  }
  
  //front motors = thrust - pitch
  //back motors = thrust + pitch
  //left motors = thrust + roll
  //right motors = thrust - roll
  
  //Motors UL-UR-BL-BR --> 1-2-3-4

  motor1.writeMicroseconds(1440 + thrust - pitch + roll + yaw);
  motor2.writeMicroseconds(1440 + thrust - pitch - roll - yaw);
  motor3.writeMicroseconds(1440 + thrust + pitch + roll - yaw);
  motor4.writeMicroseconds(1440 + thrust + pitch - roll + yaw);
  Serial.println(1440 + thrust - pitch + roll + yaw);
  /*
  Serial.print(event.orientation.x);
  Serial.print("\t");
  Serial.println(yaw);
  
  Serial.println(SAMPLERATE/1000.0);
  Serial.println(yawCTL.getDT());
  Serial.println(yawCTL.getinteg());
  */

  /*
  Serial.print(millis());
  Serial.print("\t");
  Serial.print(alt[0]);
  Serial.print("\t");
  Serial.println(bmp.readAltitude(bmpOffset));
  */
  /*
  Serial.print("Y: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.println(event.orientation.z, 4);
  
  Serial.print(pitch);
  Serial.print("\t");
  Serial.println(roll);
  */

  //Serial.println(event.orientation.x);
  //Serial.println(yaw);
  
  while((micros() - tStart) < (SAMPLERATE * 1000)){}
  //Serial.println(micros()-tStart);
}
