#include "UnityComms.h"

UnityComms::UnityComms () {}

void UnityComms::setup() {
  Serial.begin(BAUDRATE);
  pinMode(LED_BUILTIN, OUTPUT);
  flashLED(2, 100);
}

void UnityComms::sendPacket(int cmd, int dataType, uint8_t* buffer, size_t len)
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

void UnityComms::sendBytes(int cmd, uint8_t* bytes, size_t len) {
  sendPacket(cmd, 1, bytes, len);
}

void UnityComms::sendBool(int cmd, bool b)
{
  uint8_t buffer[4];
  copyIntToBuf(b, buffer, 0);
  sendPacket(cmd, 2, buffer, 1);
}

void UnityComms::sendInt(int cmd, long i)
{
  uint8_t buffer[4];
  copyIntToBuf(i, buffer, 0);
  sendPacket(cmd, 3, buffer, 4);
}

void UnityComms::sendFloat(int cmd, float f)
{
  uint8_t buffer[4];
  copyFloatToBuf(f, buffer, 0);
  sendPacket(cmd, 4, buffer, 4);
}

void UnityComms::sendString(int cmd, String s)
{
  int len = s.length() + 1;

  char sc[len];
  s.toCharArray(sc, len);

  sendPacket(cmd, 5, (uint8_t*)sc, len);
}

void UnityComms::sendVector2(int cmd, float f1, float f2)
{
  uint8_t buffer[8];
  copyFloatToBuf(f1, buffer, 0);
  copyFloatToBuf(f2, buffer, 4);
  sendPacket(cmd, 8, buffer, 8);
}

void UnityComms::sendVector3(int cmd, float f1, float f2, float f3)
{
  uint8_t buffer[12];
  copyFloatToBuf(f1, buffer, 0);
  copyFloatToBuf(f2, buffer, 4);
  copyFloatToBuf(f3, buffer, 8);
  sendPacket(cmd, 9, buffer, 12);
}

void UnityComms::sendVector4(int cmd, float f1, float f2, float f3, float f4)
{
  uint8_t buffer[16];
  copyFloatToBuf(f1, buffer, 0);
  copyFloatToBuf(f2, buffer, 4);
  copyFloatToBuf(f3, buffer, 8);
  copyFloatToBuf(f4, buffer, 12);
  sendPacket(cmd, 10, buffer, 16);
}

void UnityComms::sendVector2Int(int cmd, long i1, long i2)
{
  uint8_t buffer[8];
  copyIntToBuf(i1, buffer, 0);
  copyIntToBuf(i2, buffer, 4);
  sendPacket(cmd, 6, buffer, 8);
}

void UnityComms::sendVector3Int(int cmd, long i1, long i2, long i3)
{
  uint8_t buffer[12];
  copyIntToBuf(i1, buffer, 0);
  copyIntToBuf(i2, buffer, 4);
  copyIntToBuf(i3, buffer, 8);
  sendPacket(cmd, 7, buffer, 12);
}

void UnityComms::receive()
{
  if (Serial.available())
  {

    uint8_t x = Serial.read();

    if (x == packetMarker)
    {
      if (bytesReceived > HEADER_SIZE)
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

void UnityComms::parseData()
{
  // decode packet
  uint8_t decodedBuffer[bytesReceived + 5];     // buffer to decoded input
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
    return;
  }

  // checksum
  int packetChecksum = crc.computeChecksum(dataPacket, decodedPacketSize - HEADER_SIZE);
  if (checksum != packetChecksum)
  {
    return;
  }

  // deal with data here. don't forget about the command byte!
  switch (dataType)
  {
   case 1: // ByteArray
   {
     uint8_t* bytes = dataPacket;
     size_t bytesLen = decodedPacketSize - HEADER_SIZE;
     bytesCallback(cmd, bytes, bytesLen);
   }
     break;
   case 2: // bool
   {
     bool b = getBoolFromBuf(dataPacket, 0);
     boolCallback(cmd, b);
   }
     break;
   case 3: // int
   {
     int32_t i = getIntFromBuf(dataPacket, 0);
     intCallback(cmd, i);
   }
     break;
   case 4: // float
   {
     float f = getFloatFromBuf(dataPacket, 0);
     floatCallback(cmd, f);
   }
     break;
   case 5: // string
   {
     char* string = (char*) dataPacket;
     stringCallback(cmd, string);
   }
     break;
   case 6: // int2
   {
     int32_t i1 = getIntFromBuf(dataPacket, 0);
     int32_t i2 = getIntFromBuf(dataPacket, 4);
     vector2IntCallback(cmd, i1, i2);
   }
     break;
   case 7: // int3
   {
     int32_t i1 = getIntFromBuf(dataPacket, 0);
     int32_t i2 = getIntFromBuf(dataPacket, 4);
     int32_t i3 = getIntFromBuf(dataPacket, 8);
     vector3IntCallback(cmd, i1, i2, i3);
   }
     break;
   case 8: // float2
   {
     float f1 = getFloatFromBuf(dataPacket, 0);
     float f2 = getFloatFromBuf(dataPacket, 4);
     vector2Callback(cmd, f1, f2);
   }
     break;
   case 9: // float3
   {
     float f1 = getFloatFromBuf(dataPacket, 0);
     float f2 = getFloatFromBuf(dataPacket, 4);
     float f3 = getFloatFromBuf(dataPacket, 8);
     vector3Callback(cmd, f1, f2, f3);
   }
     break;
   case 10: // float4
   {
     float f1 = getFloatFromBuf(dataPacket, 0);
     float f2 = getFloatFromBuf(dataPacket, 4);
     float f3 = getFloatFromBuf(dataPacket, 8);
     float f4 = getFloatFromBuf(dataPacket, 12);
     vector4Callback(cmd, f1, f2, f3, f4);
   }
     break;
   default:
     break;
  }
}

void UnityComms::flashLED(int n, int d) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}

void UnityComms::copyBoolToBuf(bool val, uint8_t* buffer, int offset)
{
  byte* b = (byte*) &val;
  memcpy(buffer + offset, b, 1);
}

void UnityComms::copyIntToBuf(long val, uint8_t* buffer, int offset)
{
  byte* b = (byte*) &val;
  memcpy(buffer + offset, b, 4);
}

void UnityComms::copyFloatToBuf(float val, uint8_t* buffer, int offset)
{
  byte* b = (byte*) &val;
  memcpy(buffer + offset, b, 4);
}

bool UnityComms::getBoolFromBuf(uint8_t* buffer, int offset) {
  return (bool) buffer[offset];
}

long UnityComms::getIntFromBuf(uint8_t* buffer, int offset) {
  return buffer[offset] |
         (buffer[offset + 1] << 8) |
         (buffer[offset + 2] << 16) |
         (buffer[offset + 3] << 24);
}

float UnityComms::getFloatFromBuf(uint8_t* buffer, int offset) {
  FLOATUNION_t f;
  for (int i = 0; i < 4; i++) {
    f.bytes[i] = buffer[offset + i];
  }
  return f.number;
}
