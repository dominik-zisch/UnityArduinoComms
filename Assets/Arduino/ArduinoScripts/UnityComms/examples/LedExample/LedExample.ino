/*
 * This is an example for the UnityArduinoComms plugin to send LED data
 * from Unity to an Arduino. The Unity library can be found here:
 *
 * https://github.com/dominik-zisch/UnityArduinoComms
 *
 * This example uses the Adafruit Neopixel LEDs
 * you can find the library for these LEDs here:
 *
 * https://github.com/adafruit/Adafruit_NeoPixel
 *
 */

#include "UnityComms.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN    9
#define LED_COUNT 24

//============================================================================//
//===========================================================// LED Declarations

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


//============================================================================//
//=========================================================// Unity Declarations

UnityComms unity;


//============================================================================//
//============================================================// Unity Callbacks

void onBytesReceived (int cmd, uint8_t* bytes, size_t len)
{
   strip.setBrightness(bytes[0]);

  for (int x = 0; x < LED_COUNT; x++) {
    uint8_t r = bytes[1 + 3 * x + 0];
    uint8_t g = bytes[1 + 3 * x + 1];
    uint8_t b = bytes[1 + 3 * x + 2];
    strip.setPixelColor(x, r, g, b);
  }
  strip.show();
}


//============================================================================//
//========================================================// Lifecycle functions

//--------------------------------------------------------//
//------------------------------// setup - called at startup
void setup()
{
  // put your setup code here, to run once:
  unity.setup();
  unity.bytesCallback = &onBytesReceived;

  strip.begin();
  strip.show();
  strip.setBrightness(50);
}


//--------------------------------------------------------//
//--------------------------------// loop - called in a loop
void loop()
{
  // receive data from computer
  unity.receive();

  // put your main code here, to run repeatedly:
}
