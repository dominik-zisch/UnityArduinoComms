#include "UnityComms.h"
#include <Adafruit_NeoPixel.h>

#define BAUDRATE 115200             //Data Speed
#define PACKET_SIZE 512            // Input Packet size

#define LED_PIN    9
#define LED_COUNT 60

// Data Type Identifiers:
//
//  01 - ByteArray
//  02 - bool
//  03 - int
//  04 - float
//  05 - string
//  06 - int2
//  07 - int3
//  08 - vector2
//  09 - vector3
//  10 - vector4

UnityComms unity;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void onMessageReceived(int cmd, int dataType, uint8_t *dataPacket, size_t len)
{
  switch (dataType)
  {
    case 1: // ByteArray
    {
      uint8_t* bytes = dataPacket;
      // use the byte array here

      strip.setBrightness(bytes[0]);
      for (int x = 0; x < LED_COUNT; x++) {
        uint8_t r = bytes[1 + 3*x + 0];
        uint8_t g = bytes[1 + 3*x + 1];
        uint8_t b = bytes[1 + 3*x + 2];
        strip.setPixelColor(x, r,g,b);
      }
      strip.show();
      
    }
      break;
    case 3: // int
    {
      int i = unity.getIntFromBuf(dataPacket, 0);
      // use the integer here

      for (int x = 0; x < LED_COUNT; x++) {
        if (x == i) {
          strip.setPixelColor(x, 200,200,200);
        } else {
          strip.setPixelColor(x, 0,0,0);
        }
      }
      strip.show();

    }
      break;
    default:
      break;
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);

  strip.begin();
  strip.show();
  strip.setBrightness(50);
  
  unity.setup();
  unity.callbackFunc = &onMessageReceived;
}

void loop()
{
  // receive data from computer
  unity.receive();

  // put your main code here, to run repeatedly:
}
