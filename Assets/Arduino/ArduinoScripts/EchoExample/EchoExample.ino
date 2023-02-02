//#include "COBS.h"
//#include "Crc16.h"
#include "UnityComms.h"

#define BAUDRATE 115200             //Data Speed
#define PACKET_SIZE 512            // Input Packet size
//#define HEADER_SIZE 6

//typedef union                       // Used for float to byte conversion
//{
//  float number;
//  uint8_t bytes[4];
//} FLOATUNION_t;

// Packet structure:
//
// <lenlo> <lenhi> <crc16ln> <crc16hi> <cmd> <dataType> [data]
//
// <lenlo>    Packet length low
// <lenhi>    Packet length high
// <crclo>    CRC16 checksum low
// <crchi>    CRC16 checksum high
// <cmd>      Command byte
// <dataType> Data type
// [data]     Data bytes
//
//
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

void onBytesReceived   (int cmd, uint8_t* value, size_t len)                             { unity.sendBytes(cmd, value, len); }
void onBoolReceived    (int cmd, bool value)                                             { unity.sendBool(cmd, value); }
void onIntReceived     (int cmd, int32_t value)                                          { unity.sendInt(cmd, value); }
void onFloatReceived   (int cmd, float value)                                            { unity.sendFloat(cmd, value); }
void onStringReceived  (int cmd, String value)                                           { unity.sendString(cmd, value); }
void onInt2Received    (int cmd, int32_t value1, int32_t value2)                         { unity.sendInt2(cmd, value1, value2); }
void onInt3Received    (int cmd, int32_t value1, int32_t value2, int32_t value3)         { unity.sendInt3(cmd, value1, value2, value3); }
void onVector2Received (int cmd, float value1, float value2)                             { unity.sendVector2(cmd, value1, value2); }
void onVector3Received (int cmd, float value1, float value2, float value3)               { unity.sendVector3(cmd, value1, value2, value3); }
void onVector4Received (int cmd, float value1, float value2, float value3, float value4) { unity.sendVector4(cmd, value1, value2, value3, value4); }

//============================================================================//
//========================================================// Serial Declarations


//int bytesReceived = 0;                        // number of bytes received
//const uint8_t packetMarker = (uint8_t) 0x00;  // packet marker
//uint8_t inputBuffer[PACKET_SIZE];             // buffer to store input
//Crc16 crc;                                    // CRC algorithm for checksum
//
//void (*bytesCallback)(int, uint8_t*, size_t);
//void (*boolCallback)(int, bool);
//void (*intCallback)(int, int32_t);
//void (*floatCallback)(int, float);
//void (*stringCallback)(int, String);
//void (*int2Callback)(int, int32_t, int32_t);
//void (*int3Callback)(int, int32_t, int32_t, int32_t);
//void (*vector2Callback)(int, float, float);
//void (*vector3Callback)(int, float, float, float);
//void (*vector4Callback)(int, float, float, float, float);


//============================================================================//
//========================================================// Lifecycle functions

//--------------------------------------------------------//
//------------------------------// setup - called at startup
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
//  pinMode(LED_BUILTIN, OUTPUT);
  unity.setup();

  unity.bytesCallback = &onBytesReceived;
  unity.boolCallback = &onBoolReceived;
  unity.intCallback = &onIntReceived;
  unity.floatCallback = &onFloatReceived;
  unity.stringCallback = &onStringReceived;
  unity.int2Callback = &onInt2Received;
  unity.int3Callback = &onInt3Received;
  unity.vector2Callback = &onVector2Received;
  unity.vector3Callback = &onVector3Received;
  unity.vector4Callback = &onVector4Received;
  
//  flashLED(2, 100);
}


//--------------------------------------------------------//
//--------------------------------// loop - called in a loop
void loop()
{
  // receive data from computer
//  receive();
  unity.receive();

  // put your main code here, to run repeatedly:
}
//
//
//
////============================================================================//
////===========================================================// Serial functions
//
////--------------------------------------------------------//
////-----------------------------------------// Send ByteArray
//void sendBytes(int cmd, uint8_t* bytes, size_t len) {
//  sendPacket(cmd, 1, bytes, len);
//}
//
//
////--------------------------------------------------------//
////-------------------------------------------// Send Boolean
//void sendBool(int cmd, bool b)
//{
//  uint8_t buffer[4];
//  copyIntToBuf(b, buffer, 0);
//  sendPacket(cmd, 2, buffer, 1);
//}
//
//
////--------------------------------------------------------//
////-------------------------------------------// Send Integer
//void sendInt(int cmd, int32_t i)
//{
//  uint8_t buffer[4];
//  copyIntToBuf(i, buffer, 0);
//  sendPacket(cmd, 3, buffer, 4);
//}
//
//
////--------------------------------------------------------//
////---------------------------------------------// Send Float
//void sendFloat(int cmd, float f)
//{
//  uint8_t buffer[4];
//  copyFloatToBuf(f, buffer, 0);
//  sendPacket(cmd, 4, buffer, 4);
//}
//
//
////--------------------------------------------------------//
////--------------------------------------------// Send String
//void sendString(int cmd, String s)
//{
//  int len = s.length() + 1;
//
//  char sc[len];
//  s.toCharArray(sc, len);
//
//  sendPacket(cmd, 5, (uint8_t*)sc, len);
//}
//
//
////--------------------------------------------------------//
////----------------------------------------// Send 2 Integers
//void sendInt2(int cmd, int32_t i1, int32_t i2)
//{
//  uint8_t buffer[8];
//  copyIntToBuf(i1, buffer, 0);
//  copyIntToBuf(i2, buffer, 4);
//  sendPacket(cmd, 6, buffer, 8);
//}
//
//
////--------------------------------------------------------//
////----------------------------------------// Send 3 Integers
//void sendInt3(int cmd, int32_t i1, int32_t i2, int32_t i3)
//{
//  uint8_t buffer[12];
//  copyIntToBuf(i1, buffer, 0);
//  copyIntToBuf(i2, buffer, 4);
//  copyIntToBuf(i3, buffer, 8);
//  sendPacket(cmd, 7, buffer, 12);
//}
//
//
////--------------------------------------------------------//
////------------------------------------------// Send 2 Floats
//void sendVector2(int cmd, float f1, float f2)
//{
//  uint8_t buffer[8];
//  copyFloatToBuf(f1, buffer, 0);
//  copyFloatToBuf(f2, buffer, 4);
//  sendPacket(cmd, 8, buffer, 8);
//}
//
//
////--------------------------------------------------------//
////------------------------------------------// Send 3 Floats
//void sendVector3(int cmd, float f1, float f2, float f3)
//{
//  uint8_t buffer[12];
//  copyFloatToBuf(f1, buffer, 0);
//  copyFloatToBuf(f2, buffer, 4);
//  copyFloatToBuf(f3, buffer, 8);
//  sendPacket(cmd, 9, buffer, 12);
//}
//
//
////--------------------------------------------------------//
////------------------------------------------// Send 4 Floats
//void sendVector4(int cmd, float f1, float f2, float f3, float f4)
//{
//  uint8_t buffer[16];
//  copyFloatToBuf(f1, buffer, 0);
//  copyFloatToBuf(f2, buffer, 4);
//  copyFloatToBuf(f3, buffer, 8);
//  copyFloatToBuf(f4, buffer, 12);
//  sendPacket(cmd, 10, buffer, 16);
//}
//
//
////--------------------------------------------------------//
////-------------------------------------// Send data to Unity
//void sendPacket(int cmd, int dataType, uint8_t* buffer, size_t len)
//{
//  int checksum = crc.computeChecksum(buffer, len);
//  uint8_t serialPacket[len + HEADER_SIZE];
//
//  serialPacket[0] = (uint8_t) (len & 0xff);
//  serialPacket[1] = (uint8_t) ((len >> 8) & 0xff);
//  serialPacket[2] = (uint8_t) (checksum & 0xff);
//  serialPacket[3] = (uint8_t) ((checksum >> 8) & 0xff);
//  serialPacket[4] = (uint8_t) cmd;
//  serialPacket[5] = (uint8_t) dataType;
//
//  memcpy(serialPacket + HEADER_SIZE * sizeof(uint8_t), buffer, len * sizeof(uint8_t));
//
//  uint8_t _encodeBuffer[COBS::getEncodedBufferSize(len + HEADER_SIZE)];
//
//  size_t numEncoded = COBS::encode(serialPacket, len + HEADER_SIZE, _encodeBuffer);
//
//  Serial.write(_encodeBuffer, numEncoded);
//  Serial.write(packetMarker);
//}
//
//
////--------------------------------------------------------//
////------------------------------------// Get data from Unity
//void receive()
//{
//  if (Serial.available())
//  {
//
//    uint8_t x = Serial.read();
//
//    if (x == packetMarker)
//    {
//      if (bytesReceived > HEADER_SIZE)
//        parseData();
//      bytesReceived = 0;
//    }
//    else
//    {
//      if (bytesReceived >= PACKET_SIZE)
//      {
//        // Packet overrun. if this happens, dismiss current packet. 
//        // If this happens your PACKET_SIZE is too small!
//        bytesReceived = 0;
//      }
//      else
//      {
//        inputBuffer[bytesReceived++] = x;
//      }
//    }
//  }
//}
//
//
////--------------------------------------------------------//
////-----------------------------------------// Parse the data
//void parseData()
//{
//  // decode packet
//  uint8_t decodedBuffer[bytesReceived+5];       // buffer to decoded input
//  int decodedPacketSize = COBS::decode(inputBuffer, bytesReceived, decodedBuffer);
//
//  // disect packt
//  int len = decodedBuffer[0] | (decodedBuffer[1] << 8);
//  int checksum = decodedBuffer[2] | (decodedBuffer[3] << 8);
//  int cmd = decodedBuffer[4];
//  int dataType = decodedBuffer[5];
//  uint8_t dataPacket[decodedPacketSize - HEADER_SIZE];
//  memcpy(dataPacket, decodedBuffer + HEADER_SIZE * sizeof(uint8_t), (decodedPacketSize - HEADER_SIZE) * sizeof(uint8_t));
//
//  // check length
//  if ((decodedPacketSize - HEADER_SIZE) != len)
//  {
////    flashLED(4, 100);
//    return;
//  }
//
//  // checksum
//  int packetChecksum = crc.computeChecksum(dataPacket, decodedPacketSize - HEADER_SIZE);
//  if (checksum != packetChecksum)
//  {
////    flashLED(6, 100);
//    return;
//  }
//
//  // echo msg back - remove this and add your own implementation below
////  sendPacket(cmd, dataType, dataPacket, decodedPacketSize - HEADER_SIZE);
//
//  // deal with data here. don't forget about the command byte!
//  switch (dataType)
//  {
//    case 1: // ByteArray
//    {
//      uint8_t* bytes = dataPacket;
//      size_t bytesLen = decodedPacketSize - HEADER_SIZE;
//      bytesCallback(cmd, bytes, bytesLen);
//    }
//      break;
//    case 2: // bool
//    {
//      bool b = getBoolFromBuf(dataPacket, 0);
//      boolCallback(cmd, b);
//    }
//      break;
//    case 3: // int
//    {
//      int32_t i = getIntFromBuf(dataPacket, 0);
//      intCallback(cmd, i);
//    }
//      break;
//    case 4: // float
//    {
//      float f = getFloatFromBuf(dataPacket, 0);
//      floatCallback(cmd, f);
//    }
//      break;
//    case 5: // string
//    {
//      char* string = (char*) dataPacket;
//      stringCallback(cmd, string);
//    }
//      break;
//    case 6: // int2
//    {
//      int32_t i1 = getIntFromBuf(dataPacket, 0);
//      int32_t i2 = getIntFromBuf(dataPacket, 4);
//      int2Callback(cmd, i1, i2);
//    }
//      break;
//    case 7: // int3
//    {
//      int32_t i1 = getIntFromBuf(dataPacket, 0);
//      int32_t i2 = getIntFromBuf(dataPacket, 4);
//      int32_t i3 = getIntFromBuf(dataPacket, 8);
//      int3Callback(cmd, i1, i2, i3);
//    }
//      break;
//    case 8: // float2
//    {
//      float f1 = getFloatFromBuf(dataPacket, 0);
//      float f2 = getFloatFromBuf(dataPacket, 4);
//      vector2Callback(cmd, f1, f2);
//    }
//      break;
//    case 9: // float3
//    {
//      float f1 = getFloatFromBuf(dataPacket, 0);
//      float f2 = getFloatFromBuf(dataPacket, 4);
//      float f3 = getFloatFromBuf(dataPacket, 8);
//      vector3Callback(cmd, f1, f2, f3);
//    }
//      break;
//    case 10: // float4
//    {
//      float f1 = getFloatFromBuf(dataPacket, 0);
//      float f2 = getFloatFromBuf(dataPacket, 4);
//      float f3 = getFloatFromBuf(dataPacket, 8);
//      float f4 = getFloatFromBuf(dataPacket, 12);
//      vector4Callback(cmd, f1, f2, f3, f4);
//    }
//      break;
//    default:
//      break;
//  }
//
//}
//
//
//
////============================================================================//
////======================================================// Utilities and Helpers
//
////--------------------------------------------------------//
////----------------------------------------------// Flash LED
//void flashLED(int n, int d) {
//  for (int i = 0; i < n; i++) {
//    digitalWrite(LED_BUILTIN, HIGH);
//    delay(d);
//    digitalWrite(LED_BUILTIN, LOW);
//    delay(d);
//  }
//}
//
//
////--------------------------------------------------------//
////------------------------------------// Copy bool to buffer
//void copyBoolToBuf(bool val, uint8_t* buffer, int offset)
//{
//  byte* b = (byte*) &val;
//  memcpy(buffer + offset, b, 1);
//}
//
//
////--------------------------------------------------------//
////-------------------------------------// Copy int to buffer
//void copyIntToBuf(int32_t val, uint8_t* buffer, int offset)
//{
//  byte* b = (byte*) &val;
//  memcpy(buffer + offset, b, 4);
//}
//
//
////--------------------------------------------------------//
////-----------------------------------// Copy float to buffer
//void copyFloatToBuf(float val, uint8_t* buffer, int offset)
//{
//  byte* b = (byte*) &val;
//  memcpy(buffer + offset, b, 4);
//}
//
//
////--------------------------------------------------------//
////--------------------------------// Get boolean from buffer
//bool getBoolFromBuf(uint8_t* buffer, int offset) {
//  return (bool) buffer[offset];
//}
//
//
////--------------------------------------------------------//
////--------------------------------// Get integer from buffer
//int32_t getIntFromBuf(uint8_t* buffer, int offset) {
//  return buffer[offset] |
//         (buffer[offset + 1] << 8) |
//         (buffer[offset + 2] << 16) |
//         (buffer[offset + 3] << 24);
//}
//
//
////--------------------------------------------------------//
////----------------------------------// Get float from buffer
//float getFloatFromBuf(uint8_t* buffer, int offset) {
//  FLOATUNION_t f;
//  for (int i = 0; i < 4; i++) {
//    f.bytes[i] = buffer[offset + i];
//  }
//  return f.number;
//}
