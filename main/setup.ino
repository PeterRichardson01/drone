//Pin change interrupts for RC Receiver PWM signals

void rising1()
{
  prev_time1 = micros();
  PCintPort::detachInterrupt(CH1PIN);
  PCintPort::attachInterrupt(CH1PIN, &falling1, FALLING);
}

void falling1()
{
  ch1 = micros()-prev_time1;
  PCintPort::detachInterrupt(CH1PIN);
  PCintPort::attachInterrupt(CH1PIN, &rising1, RISING);
}

void rising2()
{
  prev_time2 = micros();
  PCintPort::detachInterrupt(CH2PIN);
  PCintPort::attachInterrupt(CH2PIN, &falling2, FALLING);

}

void falling2()
{
  ch2 = micros()-prev_time2;
  PCintPort::detachInterrupt(CH2PIN);
  PCintPort::attachInterrupt(CH2PIN, &rising2, RISING);
}

void rising3()
{
  prev_time3 = micros();
  PCintPort::detachInterrupt(CH3PIN);
  PCintPort::attachInterrupt(CH3PIN, &falling3, FALLING);
}

void falling3()
{
  ch3 = micros()-prev_time3;
  PCintPort::detachInterrupt(CH3PIN);
  PCintPort::attachInterrupt(CH3PIN, &rising3, RISING);
}

void rising4()
{
  prev_time4 = micros();
  PCintPort::detachInterrupt(CH4PIN);
  PCintPort::attachInterrupt(CH4PIN, &falling4, FALLING);
}

void falling4()
{
  PCintPort::detachInterrupt(CH4PIN);
  PCintPort::attachInterrupt(CH4PIN, &rising4, RISING);
  ch4 = micros()-prev_time4;
}

void setup() {
  //Serial.begin(115200);

  pinMode(CH1PIN, INPUT_PULLUP);
  pinMode(CH2PIN, INPUT_PULLUP);
  pinMode(CH3PIN, INPUT_PULLUP);
  pinMode(CH4PIN, INPUT_PULLUP);

  PCintPort::attachInterrupt(CH1PIN, &rising1, RISING);
  PCintPort::attachInterrupt(CH2PIN, &rising2, RISING);
  PCintPort::attachInterrupt(CH3PIN, &rising3, RISING);
  PCintPort::attachInterrupt(CH4PIN, &rising4, RISING);
  
  motor1.attach(MOTOR1PIN);
  motor2.attach(MOTOR2PIN);
  motor3.attach(MOTOR3PIN);
  motor4.attach(MOTOR4PIN);

  motor1.writeMicroseconds(1000);
  motor2.writeMicroseconds(1000);
  motor3.writeMicroseconds(1000);
  motor4.writeMicroseconds(1000);

   if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  bno.setExtCrystalUse(true);
  
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
 
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  
  delay(6000);
  
  motor1.writeMicroseconds(1435);
  motor2.writeMicroseconds(1445);
  motor3.writeMicroseconds(1445);
  motor4.writeMicroseconds(1435);
  
  //First few inputs from sensors are inaccurate
  sensors_event_t event;
  for(int i = 0; i < 1000/SAMPLERATE; i++)
  {
    bno.getEvent(&event);
    bmp.performReading();
    delay(SAMPLERATE);
  }

  //set Offsets to set altitude to 0 at ground and yaw at 0
  bmpOffset = bmp.pressure/100.0;
  desalt = 0.0;

  tStart = micros();
}
