#include "UnityComms.h"

UnityComms::UnityComms () {}

void UnityComms::setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  flashLED(4, 100);
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


//--------------------------------------------------------//
//--------------------------------------------// Send String
void UnityComms::sendString(int cmd, String s)
{
  int len = s.length() + 1;

  char sc[len];
  s.toCharArray(sc, len);

  sendPacket(cmd, 5, (uint8_t*)sc, len);
}

void UnityComms::sendInt2(int cmd, long i1, long i2)
{
  uint8_t buffer[8];
  copyIntToBuf(i1, buffer, 0);
  copyIntToBuf(i2, buffer, 4);
  sendPacket(cmd, 6, buffer, 8);
}

void UnityComms::sendInt3(int cmd, long i1, long i2, long i3)
{
  uint8_t buffer[12];
  copyIntToBuf(i1, buffer, 0);
  copyIntToBuf(i2, buffer, 4);
  copyIntToBuf(i3, buffer, 8);
  sendPacket(cmd, 7, buffer, 12);
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
    //    flashLED(4, 100);
    return;
  }

  // checksum
  int packetChecksum = crc.computeChecksum(dataPacket, decodedPacketSize - HEADER_SIZE);
  if (checksum != packetChecksum)
  {
    //    flashLED(6, 100);
    return;
  }

  // echo msg back - remove this and add your own implementation below
  //  sendPacket(cmd, dataType, dataPacket, decodedPacketSize - HEADER_SIZE);

  size_t packetLen = decodedPacketSize - HEADER_SIZE;
  callbackFunc(cmd, dataType, dataPacket, packetLen);
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
