#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <PinChangeInt.h>

#include "pid.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
 
#define SAMPLERATE (10.0) //100 Hz

//Motors UL-UR-BL-BR --> 1-2-3-4
#define MOTOR1PIN 11
#define MOTOR2PIN 10
#define MOTOR3PIN 9
#define MOTOR4PIN 6

//Receiver Channels
#define CH1PIN 2
#define CH2PIN 3
#define CH3PIN 4
#define CH4PIN 5

volatile unsigned long prev_time1 = 0;
volatile unsigned long prev_time2 = 0;
volatile unsigned long prev_time3 = 0;
volatile unsigned long prev_time4 = 0;

volatile unsigned long ch1, ch2, ch3, ch4;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);
Adafruit_BMP3XX bmp;

double bmpOffset;

double desalt;
double despitch;
double desroll;
double desyaw;

PID thrustCTL = PID(SAMPLERATE/1000.0, 10.0, -10.0, 0.0, 0.000, 0.00);
/*
PID pitchCTL = PID(SAMPLERATE/1000.0, 10.0, -10.0, 5.0, 2.0, 0.05);
PID rollCTL = PID(SAMPLERATE/1000.0, 10.0, -10.0, 5.0, 2.0, 0.05);
PID yawCTL = PID(SAMPLERATE/1000.0, 10.0, -10.0, 5.0, 2.0, 0.05);
*/

PID pitchCTL = PID(SAMPLERATE/1000.0, 15.0, -15.0, 0.7, 0.01, 0.01);
PID rollCTL = PID(SAMPLERATE/1000.0, 15.0, -15.0, 0.7, 0.01, 0.01);
PID yawCTL = PID(SAMPLERATE/1000.0, 100.0, -100.0, 4.0, 0.08, 0.0);

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

double alt[5];

unsigned long tStart;
