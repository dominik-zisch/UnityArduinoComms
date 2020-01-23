

public class SLIP
{
    private const int End = 192; // Indicates the end of a packet
    private const int Esc = 219; // Indicates byte stuffing
    private const int EscEnd = 220; // Esc EscEnd means End data byte
    private const int EscEsc = 221; // Esc EscEsc means Esc data byte
    
    public static int Encode(ref byte[] buffer, int size, ref byte[] encodedBuffer)
    {
        if (size == 0)
            return 0;

        int readIndex  = 0;
        int writeIndex = 0;

        // Double-ENDed, flush any data that may have accumulated due to line 
        // noise.
        encodedBuffer[writeIndex++] = End;

        while (readIndex < size)
        {
            if(buffer[readIndex] == End)
            {
                encodedBuffer[writeIndex++] = Esc;
                encodedBuffer[writeIndex++] = EscEnd;
                readIndex++;
            }
            else if(buffer[readIndex] == Esc)
            {
                encodedBuffer[writeIndex++] = Esc;
                encodedBuffer[writeIndex++] = EscEsc;
                readIndex++;
            }
            else
            {
                encodedBuffer[writeIndex++] = buffer[readIndex++];
            }
        }

        return writeIndex;
    }
    
    public static int Decode(ref byte[] encodedBuffer, int size, ref byte[] decodedBuffer)
    {
        if (size == 0)
            return 0;

        int readIndex  = 0;
        int writeIndex = 0;

        while (readIndex < size)
        {
            if (encodedBuffer[readIndex] == End)
            {
                // flush or done
                readIndex++;
            }
            else if (encodedBuffer[readIndex] == Esc)
            {
                if (encodedBuffer[readIndex+1] == EscEnd)
                {
                    decodedBuffer[writeIndex++] = End;
                    readIndex += 2;
                }
                else if (encodedBuffer[readIndex+1] == EscEsc)
                {
                    decodedBuffer[writeIndex++] = Esc;
                    readIndex += 2;
                }
                else
                {
                    // This case is considered a protocol violation.
                }
            }
            else
            {
                decodedBuffer[writeIndex++] = encodedBuffer[readIndex++];
            }
        }

        return writeIndex;
    }
}
