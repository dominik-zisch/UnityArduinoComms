#pragma once

#include "Arduino.h"

class COBS
{
  public:

    static size_t encode(const uint8_t* buffer, size_t size, uint8_t* encodedBuffer)
    {
      size_t readIndex = 0;
      size_t writeIndex = 1;
      size_t codeIndex = 0;
      uint8_t code = 1;

      while (readIndex < size)
      {
        if (buffer[readIndex] == 0)
        {
          encodedBuffer[codeIndex] = code;
          code = 1;
          codeIndex = writeIndex++;
          readIndex++;
        }
        else
        {
          encodedBuffer[writeIndex++] = buffer[readIndex++];
          code++;

          if (code == 0xFF)
          {
            encodedBuffer[codeIndex] = code;
            code = 1;
            codeIndex = writeIndex++;
          }
        }
      }

      encodedBuffer[codeIndex] = code;

      return writeIndex;
    }

    static size_t decode(const uint8_t* encodedBuffer, size_t size, uint8_t* decodedBuffer)
    {
        if (size == 0)
            return 0;

        size_t readIndex = 0;
        size_t writeIndex = 0;
        uint8_t code = 0;
        uint8_t i = 0;

        while (readIndex < size)
        {
            code = encodedBuffer[readIndex];

            if (readIndex + code > size && code != 1)
            {
                return 0;
            }

            readIndex++;

            for (i = 1; i < code; i++)
            {
                decodedBuffer[writeIndex++] = encodedBuffer[readIndex++];
            }

            if (code != 0xFF && readIndex != size)
            {
                decodedBuffer[writeIndex++] = '\0';
            }
        }

        return writeIndex;
    }

    static size_t getEncodedBufferSize(size_t unencodedBufferSize)
    {
        return unencodedBufferSize + unencodedBufferSize / 254 + 1;
    }
};

