void setup() {
  Serial.begin(115200);
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

  //First few inputs from sensors are inaccurate
  sensors_event_t event;

  
  delay(5000);
  
  
  motor1.writeMicroseconds(1200);
  motor2.writeMicroseconds(1200);
  motor3.writeMicroseconds(1200);
  motor4.writeMicroseconds(1200);
  
  
  for(int i = 0; i < 1000/SAMPLERATE; i++)
  {
    bno.getEvent(&event);
    bmp.performReading();
    delay(SAMPLERATE);
  }

  //set Offsets to set altitude to 0 at ground and yaw at 0
  bmpOffset = bmp.pressure/100.0;

}
