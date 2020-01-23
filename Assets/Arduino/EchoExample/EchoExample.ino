#include "COBS.h"
#include "Crc16.h"

#define BAUDRATE 115200             //Data Speed
#define PACKET_SIZE 1024
#define HEADER_SIZE 6

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

//============================================================================//
//========================================================// Serial Declarations

int bytesReceived = 0;                        // number of bytes received
const uint8_t packetMarker = (uint8_t) 0x00;  // packet marker
uint8_t inputBuffer[PACKET_SIZE];             // buffer to store input
uint8_t decodedBuffer[2 * PACKET_SIZE];       // buffer to decoded input
uint8_t encodedBuffer[2 * PACKET_SIZE];       // buffer to encoded input
Crc16 crc;                                    // CRC algorithm for checksum


//============================================================================//
//========================================================// Lifecycle functions

//--------------------------------------------------------//
//------------------------------// setup - called at startup
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  pinMode(LED_BUILTIN, OUTPUT);

  flashLED(8, 100);
}


//--------------------------------------------------------//
//--------------------------------// loop - called in a loop
void loop()
{
  // put your main code here, to run repeatedly:
  receive();
}



//============================================================================//
//===========================================================// Serial functions

void copyBoolToBuf(bool val, uint8_t* buffer, int offset)
{
  byte* b = (byte*) &val;
  memcpy(buffer + offset, b, 1);
}

void copyIntToBuf(int val, uint8_t* buffer, int offset)
{
  byte* b = (byte*) &val;
  memcpy(buffer + offset, b, 4);
}

void copyFloatToBuf(float val, uint8_t* buffer, int offset)
{
  byte* b = (byte*) &val;
  memcpy(buffer + offset, b, 4);
}


//--------------------------------------------------------//
//-----------------------------------------// Send ByteArray
void sendByteArray(int cmd, uint8_t* buffer, size_t len) {
  sendPacket(cmd, 1, buffer, len);
}


//--------------------------------------------------------//
//-------------------------------------------// Send Boolean
void sendBool(int cmd, bool b)
{
  uint8_t buffer[4];
  copyIntToBuf(b, buffer, 0);
  sendPacket(cmd, 2, buffer, 1);
}


//--------------------------------------------------------//
//-------------------------------------------// Send Integer
void sendInt(int cmd, int i)
{
  uint8_t buffer[4];
  copyIntToBuf(i, buffer, 0);
  sendPacket(cmd, 3, buffer, 4);
}


//--------------------------------------------------------//
//---------------------------------------------// Send Float
void sendFloat(int cmd, float f)
{
  uint8_t buffer[4];
  copyFloatToBuf(f, buffer, 0);
  sendPacket(cmd, 4, buffer, 4);
}


//--------------------------------------------------------//
//--------------------------------------------// Send String
void sendString(int cmd, uint8_t* s, size_t len)
{
  sendPacket(cmd, 5, s, len);
}


//--------------------------------------------------------//
//----------------------------------------// Send 2 Integers
void sendInt2(int cmd, int i1, int i2)
{
  uint8_t buffer[8];
  copyIntToBuf(i1, buffer, 0);
  copyIntToBuf(i2, buffer, 4);
  sendPacket(cmd, 6, buffer, 8);
}


//--------------------------------------------------------//
//----------------------------------------// Send 3 Integers
void sendInt3(int cmd, int i1, int i2, int i3)
{
  uint8_t buffer[12];
  copyIntToBuf(i1, buffer, 0);
  copyIntToBuf(i2, buffer, 4);
  copyIntToBuf(i3, buffer, 8);
  sendPacket(cmd, 7, buffer, 12);
}


//--------------------------------------------------------//
//------------------------------------------// Send 2 Floats
void sendFloat2(int cmd, float f1, float f2)
{
  uint8_t buffer[8];
  copyFloatToBuf(f1, buffer, 0);
  copyFloatToBuf(f2, buffer, 4);
  sendPacket(cmd, 8, buffer, 8);
}


//--------------------------------------------------------//
//------------------------------------------// Send 3 Floats
void sendFloat3(int cmd, float f1, float f2, float f3)
{
  uint8_t buffer[12];
  copyFloatToBuf(f1, buffer, 0);
  copyFloatToBuf(f2, buffer, 4);
  copyFloatToBuf(f3, buffer, 8);
  sendPacket(cmd, 9, buffer, 12);
}


//--------------------------------------------------------//
//------------------------------------------// Send 4 Floats
void sendFloat4(int cmd, float f1, float f2, float f3, float f4)
{
  uint8_t buffer[16];
  copyFloatToBuf(f1, buffer, 0);
  copyFloatToBuf(f2, buffer, 4);
  copyFloatToBuf(f3, buffer, 8);
  copyFloatToBuf(f4, buffer, 12);
  sendPacket(cmd, 10, buffer, 16);
}


//--------------------------------------------------------//
//-------------------------------------// Send data to Unity
void sendPacket(int cmd, int dataType, uint8_t* buffer, size_t len)
{
  int checksum = crc.computeChecksum(buffer, len);
  uint8_t serialPacket[len + HEADER_SIZE];

  serialPacket[0] = (uint8_t) (len & 0xff);
  serialPacket[1] = (uint8_t) ((len >> 8) & 0xff);
  serialPacket[2] = (uint8_t) (checksum & 0xff);
  serialPacket[3] = (uint8_t) ((checksum >> 8) & 0xff);
  serialPacket[4] = (uint8_t) cmd;
  serialPacket[5] = (uint8_t) dataType;

  memcpy(serialPacket + HEADER_SIZE * sizeof(uint8_t), buffer, len * sizeof(uint8_t));

  uint8_t _encodeBuffer[COBS::getEncodedBufferSize(len + HEADER_SIZE)];

  size_t numEncoded = COBS::encode(serialPacket, len + HEADER_SIZE, _encodeBuffer);

  Serial.write(_encodeBuffer, numEncoded);
  Serial.write(packetMarker);
}


//--------------------------------------------------------//
//------------------------------------// Get data from Unity
void receive()
{
  if (Serial.available())
  {

    uint8_t x = Serial.read();

    if (x == packetMarker)
    {
      parseData();
      bytesReceived = 0;
    }
    else
    {
      if (bytesReceived >= PACKET_SIZE)
      {
        // Packet overrun. if this happens, dismiss current packet. 
        // If this happens your PACKET_SIZE is too small!
        bytesReceived = 0;
      }
      else
      {
        inputBuffer[bytesReceived++] = x;
      }
    }
  }
}


//--------------------------------------------------------//
//-----------------------------------------// Parse the data
void parseData()
{
  // decode packet
  int decodedPacketSize = COBS::decode(inputBuffer, bytesReceived, decodedBuffer);

  // disect packt
  int len = decodedBuffer[0] | (decodedBuffer[1] << 8);
  int checksum = decodedBuffer[2] | (decodedBuffer[3] << 8);
  int cmd = decodedBuffer[4];
  int dataType = decodedBuffer[5];
  uint8_t dataPacket[decodedPacketSize - HEADER_SIZE];
  memcpy(dataPacket, decodedBuffer + HEADER_SIZE * sizeof(uint8_t), (decodedPacketSize - HEADER_SIZE) * sizeof(uint8_t));

  // check length
  if ((decodedPacketSize - HEADER_SIZE) != len)
  {
    flashLED(4, 100);
    return;
  }

  // checksum
  int packetChecksum = crc.computeChecksum(dataPacket, decodedPacketSize - HEADER_SIZE);
  if (checksum != packetChecksum)
  {
    flashLED(6, 100);
    return;
  }

  sendPacket(cmd, dataType, dataPacket, decodedPacketSize - HEADER_SIZE);

//  01 - ByteArray
//  02 - bool
//  03 - int
//  04 - float
//  05 - string
//  06 - int2
//  07 - int3
//  08 - float2
//  09 - float3

//  switch (cmd)
//  {
//    case 1:
//      sendInt(1, 12345);
//      break;
//    case 2:
//      sendInt2(2, 1234, 56789);
//      break;
//    case 3:
//      sendInt3(3, 1234, -4567, 7891);
//      break;
//    case 4:
//      sendFloat(4, 12.345);
//      break;
//    case 5:
//      sendFloat2(5, 12.345, 987.678);
//      break;
//    case 6:
//      sendFloat3(6, 12.345, 321.123, -654.453);
//      break;
//    default:
//      sendPacket(cmd, dataPacket, decodedPacketSize - 5);
//      break;
//  }

}


//--------------------------------------------------------//
//----------------------------------------------// Flash LED
void flashLED(int n, int d) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}
