#ifndef UnityComms_h
#define UnityComms_h

#include "COBS.h"
#include "Crc16.h"

#define BAUDRATE 115200             //Data Speed
#define HEADER_SIZE 6
#define PACKET_SIZE 512            // Input Packet size

typedef union                       // Used for float to byte conversion
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

class UnityComms {

  public:

    void (*bytesCallback)(int, uint8_t*, size_t);
    void (*boolCallback)(int, bool);
    void (*intCallback)(int, int32_t);
    void (*floatCallback)(int, float);
    void (*stringCallback)(int, String);
    void (*vector2IntCallback)(int, int32_t, int32_t);
    void (*vector3IntCallback)(int, int32_t, int32_t, int32_t);
    void (*vector2Callback)(int, float, float);
    void (*vector3Callback)(int, float, float, float);
    void (*vector4Callback)(int, float, float, float, float);

    UnityComms ();

    void setup();
    void sendBytes(int cmd, uint8_t* bytes, size_t len);
    void sendBool(int cmd, bool b);
    void sendInt(int cmd, long i);
    void sendFloat(int cmd, float f);
    void sendString(int cmd, String s);
    void sendVector2(int cmd, float f1, float f2);
    void sendVector3(int cmd, float f1, float f2, float f3);
    void sendVector4(int cmd, float f1, float f2, float f3, float f4);
    void sendVector2Int(int cmd, long i1, long i2);
    void sendVector3Int(int cmd, long i1, long i2, long i3);

    void sendPacket(int cmd, int dataType, uint8_t* buffer, size_t len);

    bool getBoolFromBuf(uint8_t* buffer, int offset);
    long getIntFromBuf(uint8_t* buffer, int offset);
    float getFloatFromBuf(uint8_t* buffer, int offset);

    void receive();
    void parseData();

  private:

    int bytesReceived = 0;                        // number of bytes received
    const uint8_t packetMarker = (uint8_t) 0x00;  // packet marker
    uint8_t inputBuffer[PACKET_SIZE];             // buffer to store input
    Crc16 crc;                                    // CRC algorithm for checksum

    void flashLED(int n, int d);
    void copyBoolToBuf(bool val, uint8_t* buffer, int offset);
    void copyIntToBuf(long val, uint8_t* buffer, int offset);
    void copyFloatToBuf(float val, uint8_t* buffer, int offset);

};

#endif
