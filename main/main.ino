#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to SCL pin (analog 5 on Arduino UNO)
   Connect SDA to SDA pin (analog 4 on Arduino UNO)
   Connect VDD to 3-5V DC (depending on your board's logic level)
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
   2015/AUG/27  - Added calibration and system status helpers
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (10)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

imu::Vector<3> linacc[5]; //size affects filter size: must be at least 2.
imu::Vector<3> angle;

imu::Vector<3> linvel[2];
imu::Vector<3> linpos;

unsigned long totaltime;
unsigned long looptime;

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void displaySensorStatus(void)
{
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print(system, DEC);
  Serial.print("\t");
  Serial.print(gyro, DEC);
  Serial.print("\t");
  Serial.print(accel, DEC);
  Serial.print("\t");
  Serial.print(mag, DEC);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Optional: Display current status */
  displaySensorStatus();

  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  while((system+gyro+accel+mag) < 12)
  {
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print(system, DEC);
    Serial.print("\t");
    Serial.print(gyro, DEC);
    Serial.print("\t");
    Serial.print(accel, DEC);
    Serial.print("\t");
    Serial.print(mag, DEC);
  }

  bno.setExtCrystalUse(true);
  totaltime = millis();
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get and filter sensor data */
  for(int i = sizeof(linacc)-1; i > 0; i--)
  {
    linacc[i] = linacc[i-1];
  }

  angle     = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  linacc[0] = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  for(int i = 0; i < sizeof(linacc); i++)
  {
    linacc[0] = linacc[0] + linacc[i];
  }

  linacc[0] = linacc[0]/sizeof(linacc);

  linvel[1] = linvel[0];
  linvel[0] = linvel[1] + ( linacc[0]+linacc[1] ).scale( (double)looptime*0.5 );

  linpos = linpos + ( linvel[0]+linvel[1] ).scale( (double)looptime*0.5 );

  /*Serial.print(angle.x(), 4);
  Serial.print("\t");
  Serial.print(angle.y(), 4);
  Serial.print("\t");
  Serial.print(angle.z(), 4);
  Serial.print("\t");
  */
  
  Serial.print(linacc[0].z(), 4);
  Serial.print("\t");
  Serial.print(linvel[0].z(), 4);
  Serial.print("\t");
  Serial.print(linpos.z(), 4);
  
  /*Serial.print(linacc[0].y(), 4);
  Serial.print("\t");
  Serial.print(linacc[0].z(), 4);
  Serial.print("\t");
  */
  
  /* Optional: Display calibration status */
  //displayCalStatus();

  /* Optional: Display sensor status (debug only) */
  //displaySensorStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting nex data, then calculate timing */
  if(millis()-totaltime < BNO055_SAMPLERATE_DELAY_MS)
  {
    delay(BNO055_SAMPLERATE_DELAY_MS-(millis()-totaltime));
  }
  looptime = millis()-totaltime;
  totaltime = millis();
}
