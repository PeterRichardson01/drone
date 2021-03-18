
void loop() {
  unsigned long tStart = micros();
  double alt;
  if (! bmp.performReading())
  {
    desalt = -0.1;
    alt = 0.5;
  }
  else
  {
    alt = bmp.readAltitude(bmpOffset);
  }

  sensors_event_t event;
  bno.getEvent(&event);

  double thrust = thrustCTL.calculate(desalt, alt);
  double pitch = pitchCTL.calculate(0, event.orientation.y);
  double roll  = rollCTL.calculate (0, event.orientation.z);

  //front motors = thrust - pitch
  //back motors = thrust + pitch
  //left motors = thrust + roll
  //right motors = thrust - roll

  //Motors UL-UR-BL-BR --> 1-2-3-4
  motor1.writeMicroseconds(1200 + thrust - pitch + roll);
  motor2.writeMicroseconds(1200 + thrust - pitch - roll);
  motor3.writeMicroseconds(1200 + thrust + pitch + roll);
  motor4.writeMicroseconds(1200 + thrust + pitch - roll);

  if(millis() > 6500)
  {
    desalt = -0.1;
  }
  while((micros() - tStart) < (SAMPLERATE * 1000)){}
}
