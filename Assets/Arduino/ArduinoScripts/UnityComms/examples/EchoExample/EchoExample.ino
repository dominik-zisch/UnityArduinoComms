#include "UnityComms.h"

#define BAUDRATE 115200             //Data Speed
#define PACKET_SIZE 512            // Input Packet size

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

void onMessageReceived(int cmd, int dataType, uint8_t *dataPacket, size_t len)
{
  // deal with data here. don't forget about the command byte!
  switch (dataType)
  {
    case 1: // ByteArray
      {
        uint8_t* bytes = dataPacket;
        // use the byte array here
        unity.sendBytes(cmd, bytes, len);
      }
      break;
    case 2: // bool
      {
        bool b = unity.getBoolFromBuf(dataPacket, 0);
        // use the boolean here
        unity.sendBool(cmd, b);
      }
      break;
    case 3: // int
      {
        int i = unity.getIntFromBuf(dataPacket, 0);
        // use the integer here
        unity.sendInt(cmd, i);
      }
      break;
    case 4: // float
      {
        float f = unity.getFloatFromBuf(dataPacket, 0);
        // use the float here
        unity.sendFloat(cmd, f);
      }
      break;
    case 5: // string
      {
        char* string = (char*) dataPacket;
        // use the string here
        unity.sendString(cmd, string);
      }
      break;
    case 6: // int2
      {
        int i1 = unity.getIntFromBuf(dataPacket, 0);
        int i2 = unity.getIntFromBuf(dataPacket, 4);
        // use the integers here
        unity.sendInt2(cmd, i1, i2);
      }
      break;
    case 7: // int3
      {
        int i1 = unity.getIntFromBuf(dataPacket, 0);
        int i2 = unity.getIntFromBuf(dataPacket, 4);
        int i3 = unity.getIntFromBuf(dataPacket, 8);
        // use the integers here
        unity.sendInt3(cmd, i1, i2, i3);
      }
      break;
    case 8: // float2
      {
        float f1 = unity.getFloatFromBuf(dataPacket, 0);
        float f2 = unity.getFloatFromBuf(dataPacket, 4);
        // use the floats here
        unity.sendVector2(cmd, f1, f2);
      }
      break;
    case 9: // float3
      {
        float f1 = unity.getFloatFromBuf(dataPacket, 0);
        float f2 = unity.getFloatFromBuf(dataPacket, 4);
        float f3 = unity.getFloatFromBuf(dataPacket, 8);
        // use the floats here
        unity.sendVector3(cmd, f1, f2, f3);
      }
      break;
    case 10: // float4
      {
        float f1 = unity.getFloatFromBuf(dataPacket, 0);
        float f2 = unity.getFloatFromBuf(dataPacket, 4);
        float f3 = unity.getFloatFromBuf(dataPacket, 8);
        float f4 = unity.getFloatFromBuf(dataPacket, 12);
        // use the floats here
        unity.sendVector4(cmd, f1, f2, f3, f4);
      }
      break;
    default:
      break;
  }
}

void setup()
{
  Serial.begin(BAUDRATE);
  unity.setup();
  unity.callbackFunc = &onMessageReceived;
}

void loop()
{
  // receive data from computer
  unity.receive();

  // put your main code here, to run repeatedly:
}
