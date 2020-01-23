

public class COBS
{
    public static int Encode(ref byte[] buffer, int size, ref byte[] encodedBuffer)
    {
        int readIndex = 0;
        int writeIndex = 1;
        int codeIndex = 0;
        byte code = 1;

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

    public static int Decode(ref byte[] encodedBuffer, int size, ref byte[] decodedBuffer)
    {
        if (size == 0)
            return 0;

        int readIndex = 0;
        int writeIndex = 0;
        int code = 0;
        int i = 0;

        while (readIndex<size)
        {
            code = encodedBuffer[readIndex];

            if (readIndex + code > size && code != 1)
            {
                return 0;
            }

            readIndex++;

            for (i = 1; i<code; i++)
            {
                decodedBuffer[writeIndex++] = encodedBuffer[readIndex++];
            }

            if (code != 0xFF && readIndex != size)
            {
                decodedBuffer[writeIndex++] = 0x00;
            }
        }

        return writeIndex;
    }
    
    public static int GetEncodedBufferSize(int unencodedBufferSize)
    {
        return unencodedBufferSize + unencodedBufferSize / 254 + 1;
    }
}
