using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using System.IO.Ports;
using UnityEngine.Events;

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


/// <summary>
/// Enumeration of all the possible types of data that can be sent from/to the arduino.
/// </summary>
public enum ArduinoDataType
{
    Bytearray = 1,
    Bool = 2,
    Int = 3,
    Float = 4,
    String = 5,
    Int2 = 6,
    Int3 = 7,
    Float2 = 8,
    Float3 = 9,
    Float4 = 10
};


/// <summary>
/// Struct that holds a from/to the arduino.
/// </summary>
public struct ArduinoBuffer
{
    public int cmd;
    public ArduinoDataType dataType;
    public byte[] buffer;

    public ArduinoBuffer(int _cmd, ArduinoDataType _dataType, byte[] _buffer)
    {
        cmd = _cmd;
        dataType = _dataType;
        buffer = _buffer;
    }
}

/// <summary>
/// Main Arduino Handler class. Attach this to a GameObject.
/// </summary>
public class ArduinoHandler : MonoBehaviour
{
    [Header("Serial Port")]
    public string port;                                                // Arduino serial port
    public int baudrate = 115200;                                      // Serial communication speed
    public int bufferSize = 1024;                                      // Size of input buffer
    
    [Header("Receive Debug")]
    public int packetsWaiting;
    public bool verboseEncodedReceive = false;
    public bool verboseDecodedReceive = false;
    
    [Header("Send Debug")]
    public bool verboseEncodedSend = false;
    public bool verboseDecodedSend = false;

    [Header("Message Callback")]
    public MessageReceived OnMessageReceived;                          // Callback for Message received

    protected bool running = false;                                    // Boolean to keep track of running thread
    private SerialPort stream;                                         // Serialport
    private Thread ReceiverThread;                                     // Thread for receiving packets from Arduino
    protected Queue<ArduinoBuffer> receivedPackets;                    // Queue for passing messages from thread to main thread
    private static readonly object receiveLockObject = new object();   // Lock object for thread saftey
    protected Queue<byte[]> sendPackets;                               // Queue for passing messages from thread to main thread
    private static readonly object sendLockObject = new object();      // Lock object for thread saftey

    private int headerSize = 6;

    [Serializable] public class MessageReceived : UnityEvent<ArduinoBuffer> {}

    // Start is called before the first frame update
    void Start()
    {
        stream = new SerialPort(port, baudrate);
        stream.Open();

        receivedPackets = new Queue<ArduinoBuffer>();
        sendPackets = new Queue<byte[]>();

        running = true;
        ReceiverThread = new Thread(ReceiveThreadDoTask);
        ReceiverThread.Start();
    }


    // OnApplicationQuit is called when the game stops.
    private void OnApplicationQuit()
    {
        running = false;
        ReceiverThread.Abort();
        stream.Close();
    }


    // Update is called once per frame
    void Update()
    {
        packetsWaiting = PacketsAvailable();
        
        while (PacketsAvailable() > 0)
        {
            ArduinoBuffer packet = GetReceivedPacket();
            OnMessageReceived.Invoke(packet);
        }
    }

    public void RegisterListener(UnityAction<ArduinoBuffer> call)
    {
        OnMessageReceived.AddListener(call);
    }

    public void DeregisterListener(UnityAction<ArduinoBuffer> call)
    {
        OnMessageReceived.RemoveListener(call);
    }

    /// <summary>
    /// Sends a packet to the arduino.
    /// </summary>
    /// <param name="cmd">The command number.</param>
    /// <param name="buffer">The bytearray to be sent.</param>
    public void Send(int cmd, ArduinoDataType dataType, byte[] buffer)
    {
        // build serial packet
        int checksum = Crc16.ComputeChecksum(buffer);
        byte[] serialPacket = new byte[buffer.Length + headerSize];

        serialPacket[0] = (byte) (buffer.Length & 0xff);
        serialPacket[1] = (byte) ((buffer.Length >> 8) & 0xff);
        serialPacket[2] = (byte) (checksum & 0xff);
        serialPacket[3] = (byte) ((checksum >> 8) & 0xff);
        serialPacket[4] = (byte) cmd;
        serialPacket[5] = (byte) dataType;

        Array.Copy(buffer, 0, serialPacket, headerSize, buffer.Length);
        
        if (verboseDecodedSend)
        {
            Debug.Log(String.Format("Decoded Send Packet: {0}", Utils.ByteArrayToHexString(serialPacket)));
        }

        // encode packet
        byte[] encodedBuffer = new byte[COBS.GetEncodedBufferSize(serialPacket.Length)];
        int encodedBufferSize = COBS.Encode(ref serialPacket, serialPacket.Length, ref encodedBuffer);
        Array.Resize(ref encodedBuffer, encodedBufferSize + 1);
        encodedBuffer[encodedBufferSize] = 0;
        
        // add msg to outgoing queue
        lock (sendLockObject)
        {
            sendPackets.Enqueue(encodedBuffer);
        }
    }


    /// <summary>
    /// Sends a packet to the arduino.
    /// </summary>
    /// <param name="buffer">The ArduinoBuffer containing the command number as well as the bytearray.</param>
    public void Send(ArduinoBuffer buffer)
    {
        Send(buffer.cmd, buffer.dataType, buffer.buffer);
    }

    public void SendBytes(int cmd, byte[] buffer)
    {
        Send(cmd, ArduinoDataType.Bytearray, buffer);
    }

    public void SendBool(int cmd, bool b)
    {
        Send(cmd, ArduinoDataType.Bool, BitConverter.GetBytes(b));
    }

    public void SendInt(int cmd, int i)
    {
        Send(cmd, ArduinoDataType.Int, BitConverter.GetBytes(i));
    }

    public void SendFloat(int cmd, float f)
    {
        Send(cmd, ArduinoDataType.Float, BitConverter.GetBytes(f));
    }

    public void SendString(int cmd, string s)
    {
        Send(cmd, ArduinoDataType.String, System.Text.Encoding.ASCII.GetBytes(s));
    }

    public void SendVector2Int(int cmd, Vector2Int v)
    {
        byte[] bytes = new byte[8];
        Array.Copy(BitConverter.GetBytes(v.x), 0, bytes, 0, 4);
        Array.Copy(BitConverter.GetBytes(v.y), 0, bytes, 4, 4);
        Send(cmd, ArduinoDataType.Int2, bytes);
    }

    public void SendVector3Int(int cmd, Vector3Int v)
    {
        byte[] bytes = new byte[12];
        Array.Copy(BitConverter.GetBytes(v.x), 0, bytes, 0, 4);
        Array.Copy(BitConverter.GetBytes(v.y), 0, bytes, 4, 4);
        Array.Copy(BitConverter.GetBytes(v.z), 0, bytes, 8, 4);
        Send(cmd, ArduinoDataType.Int3, bytes);
    }

    public void SendVector2(int cmd, Vector2 v)
    {
        byte[] bytes = new byte[8];
        Array.Copy(BitConverter.GetBytes(v.x), 0, bytes, 0, 4);
        Array.Copy(BitConverter.GetBytes(v.y), 0, bytes, 4, 4);
        Send(cmd, ArduinoDataType.Float2, bytes);
    }

    public void SendVector3(int cmd, Vector3 v)
    {
        byte[] bytes = new byte[12];
        Array.Copy(BitConverter.GetBytes(v.x), 0, bytes, 0, 4);
        Array.Copy(BitConverter.GetBytes(v.y), 0, bytes, 4, 4);
        Array.Copy(BitConverter.GetBytes(v.z), 0, bytes, 8, 4);
        Send(cmd, ArduinoDataType.Float3, bytes);
    }

    public void SendVector4(int cmd, Vector4 v)
    {
        byte[] bytes = new byte[16];
        Array.Copy(BitConverter.GetBytes(v.x), 0, bytes, 0, 4);
        Array.Copy(BitConverter.GetBytes(v.y), 0, bytes, 4, 4);
        Array.Copy(BitConverter.GetBytes(v.z), 0, bytes, 8, 4);
        Array.Copy(BitConverter.GetBytes(v.w), 0, bytes, 12, 4);
        Send(cmd, ArduinoDataType.Float4, bytes);
    }


    /// <summary>
    /// Checks if any packets have been received.
    /// </summary>
    /// <returns>The number of packets available.</returns>
    public int PacketsAvailable() {
        return receivedPackets.Count;
    }


    /// <summary>
    /// Gets the oldest Packet from the Queue.
    /// </summary>
    /// <returns>The first packet received.</returns>
    public ArduinoBuffer GetReceivedPacket()
    {
        if (PacketsAvailable() > 0)
        {
            lock (receiveLockObject)
            {
                return receivedPackets.Dequeue();
            }
        }
        else
        {
            return new ArduinoBuffer(0, ArduinoDataType.Bytearray, null);
        }
    }


    /// <summary>
    /// Threaded function. Loops and checks for incoming packets.
    /// </summary>
    private void ReceiveThreadDoTask()
    {
        try
        {
            byte[] receiveBuffer = new byte[bufferSize];
            int bytesReceived = 0;

            while (running)
            {
                while (sendPackets.Count > 0)
                {
                    byte[] serialPacket;
                    
                    lock (sendLockObject)
                    {
                        serialPacket = sendPackets.Dequeue();
                    }
                    
                    // print
                    if (verboseEncodedSend)
                    {
                        Debug.Log(String.Format("Encoded Send Packet: {0}", Utils.ByteArrayToHexString(serialPacket)));
                    }
                    
                    stream.Write(serialPacket, 0, serialPacket.Length);
                }

                if (stream.BytesToRead > 0)
                {
                    int x = stream.ReadByte();

                    if (x == -1)
                    {
//                        Debug.LogError("End of Stream!");
//                        running = false;
                        continue;
                    }

                    if (x == 0)
                    {

                        // check if packetsize too small
                        if (bytesReceived < headerSize)
                        {
                            bytesReceived = 0;
                            continue;
                        }

                        // print
                        if (verboseEncodedReceive)
                        {
                            Debug.Log(String.Format("Encoded Receive Packet: {0}", Utils.ByteArrayToHexString(receiveBuffer)));
                        }

                        // decode
                        byte[] decodedBuffer = new byte[receiveBuffer.Length];
                        int decodedBufferSize = COBS.Decode(ref receiveBuffer, bytesReceived, ref decodedBuffer);
                        Array.Resize(ref decodedBuffer, decodedBufferSize);

                        if (decodedBufferSize < 1)
                        {
                            Debug.Log(String.Format("Decoded Packet length too short. \nPacket: {0}", Utils.ByteArrayToHexString(receiveBuffer)));
                            bytesReceived = 0;
                            continue;
                        }

                        // print
                        if (verboseDecodedReceive)
                        {
                            Debug.Log(String.Format("Decoded Receive Packet: {0}", Utils.ByteArrayToHexString(decodedBuffer)));
                        }

                        // disect packt
                        int len = decodedBuffer[0] | (decodedBuffer[1] << 8);
                        int checksum = decodedBuffer[2] | (decodedBuffer[3] << 8);
                        int cmd = decodedBuffer[4];
                        ArduinoDataType dataType = (ArduinoDataType) decodedBuffer[5];
                        byte[] dataPacket = new byte[decodedBuffer.Length - headerSize];
                        Array.Copy(decodedBuffer, headerSize, dataPacket, 0, decodedBufferSize - headerSize);

                        // check length
                        if (dataPacket.Length != len)
                        {
                            bytesReceived = 0;
                            Debug.Log(String.Format("Invalid Packet Length. Want: {0} - Have: {1}", len, dataPacket.Length));
                            continue;
                        }

                        // checksum
                        int packetChecksum = Crc16.ComputeChecksum(dataPacket);
                        if (checksum != packetChecksum)
                        {
                            bytesReceived = 0;
                            Debug.Log(String.Format("Invalid Checksum. Want: {0} - Have: {1}", checksum, packetChecksum));
                            continue;
                        }

                        bytesReceived = 0;

                        // store in queue
                        lock (receiveLockObject)
                        {
                            receivedPackets.Enqueue(new ArduinoBuffer(cmd, dataType, dataPacket));
                        }
                    }
                    else
                    {
                        if (bytesReceived >= bufferSize)
                        {
                            // TODO handle differently!
                            bytesReceived = 0;
                        }
                        else
                        {
                            receiveBuffer[bytesReceived++] = (byte) x;
                        }
                    }
                }
            }
        }
        catch (ThreadAbortException ex)
        {
            Debug.Log("Aborting UDP Receiver Thread!");
//              Debug.LogException(ex);
        }
    }
}
