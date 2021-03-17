#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "pid.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
 
#define SAMPLERATE (10.0) //100 Hz

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);
Adafruit_BMP3XX bmp;

double bmpOffset;
double bnoOffsetx;
double bnoOffsety;
double bnoOffsetz;

double desalt = 0.25;
double despitch;
double desroll;

PID thrustCTL = PID(SAMPLERATE/1000.0, 100.0, 0.0, 20.0, 5.0, 0.0);
PID pitchCTL = PID(SAMPLERATE/1000.0, 10.0, -10.0, 10.0, 2.5, 1.0);
PID rollCTL = PID(SAMPLERATE/1000.0, 10.0, -10.0, 10.0, 2.5, 1.0);

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

//Motors UL-UR-BL-BR --> 1-2-3-4
#define MOTOR1PIN 11
#define MOTOR2PIN 10
#define MOTOR3PIN 9
#define MOTOR4PIN 6
