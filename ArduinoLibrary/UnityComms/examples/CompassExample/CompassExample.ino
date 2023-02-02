/*
 * This is an example for the UnityArduinoComms plugin to send compass data
 * from an Arduino to Unity. The Unity library can be found here:
 *
 * https://github.com/dominik-zisch/UnityArduinoComms
 *
 * This example uses the QMC5883L - 3-Axis Digital Compass IC
 * you can find the library for this sensor here:
 *
 * https://github.com/DFRobot/DFRobot_QMC5883_old
 * 
 */

#include "UnityComms.h"

#include <Wire.h>
#include <DFRobot_QMC5883.h>

#define SENSOR_INTERVAL 100           // interval between sensor messages [milliseconds]

//============================================================================//
//=========================================================// Unity Declarations

UnityComms unity;


//============================================================================//
//========================================================// Sensor Declarations

DFRobot_QMC5883 compass;
float headingRaw = 0;
unsigned long sensorTimer = 0;

//--------------------------------------------------------//
//------------------------------// Initialise Compass
void compassInit() {

  while (!compass.begin()) {
    Serial.println("Could not find sensor");
    delay(500);
  }

  if (compass.isHMC()) {
    Serial.println("Initialize HMC5883");
    compass.setRange(HMC5883L_RANGE_1_3GA);
    compass.setMeasurementMode(HMC5883L_CONTINOUS);
    compass.setDataRate(HMC5883L_DATARATE_15HZ);
    compass.setSamples(HMC5883L_SAMPLES_8);
  } else if (compass.isQMC()) {
    Serial.println("Initialize QMC5883");
    compass.setRange(QMC5883_RANGE_2GA);
    compass.setMeasurementMode(QMC5883_CONTINOUS);
    compass.setDataRate(QMC5883_DATARATE_50HZ);
    compass.setSamples(QMC5883_SAMPLES_8);
  }
}

//--------------------------------------------------------//
//------------------------------// Compass reading
void compassRead() {
  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
  compass.setDeclinationAngle(declinationAngle);
  Vector mag = compass.readRaw();
  compass.getHeadingDegrees();

  headingRaw = mag.HeadingDegress;
}


//============================================================================//
//========================================================// Lifecycle functions

//--------------------------------------------------------//
//------------------------------// setup - called at startup
void setup()
{
  // put your setup code here, to run once:
  unity.setup();

  // Sensor init
  compassInit();
}


//--------------------------------------------------------//
//--------------------------------// loop - called in a loop
void loop()
{
  // Sensor loop
  if (millis() > sensorTimer) {
    sensorTimer += SENSOR_INTERVAL;
    compassRead();

    unity.sendFloat(1, headingRaw);
    unity.sendVector3(2, 0, headingRaw, 0);
  }
}
