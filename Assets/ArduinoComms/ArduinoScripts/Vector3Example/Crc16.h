#pragma once

#include "Arduino.h"

class Crc16 {

  public:

    const uint16_t polynomial = 0xA001;
    uint16_t table[256];

    Crc16() {
      uint16_t value;
      uint16_t tempVal;
      for (uint16_t i = 0; i < 256; ++i)
      {
        value = 0;
        tempVal = i;
        for (uint8_t j = 0; j < 8; ++j)
        {
          if (((value ^ tempVal) & 0x0001) != 0)
          {
            value = (uint16_t)((value >> 1) ^ polynomial);
          }
          else
          {
            value >>= 1;
          }
          tempVal >>= 1;
        }
        table[i] = value;
      }
    }

    uint16_t computeChecksum(const uint8_t* buffer, size_t size) {
      uint16_t crc = 0;
      for (int i = 0; i < size; ++i)
      {
        uint8_t index = (uint8_t)(crc ^ buffer[i]);
        crc = (uint16_t)((crc >> 8) ^ table[index]);
      }
      return crc;
    }
};
