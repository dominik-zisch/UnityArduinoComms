#include "UnityComms.h"

#include <Wire.h>
#include <DFRobot_QMC5883.h>

#define BAUDRATE 115200             //Data Speed
#define PACKET_SIZE 512            // Input Packet size
#define SENSOR_INTERVAL 100           // interval between sensor messages [milliseconds]

// Data Type Identifiers:
//
//  01 - ByteArray
//  02 - bool
//  03 - int
//  04 - float
//  05 - string
//  06 - int2
//  07 - int3
//  08 - float2
//  09 - float3
//  10 - float4

UnityComms unity;
DFRobot_QMC5883 compass;
float headingRaw = 0;
unsigned long sensorTimer = 0;

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

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
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
