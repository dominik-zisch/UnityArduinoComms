using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArduinoSendTest : MonoBehaviour
{
    
    [SerializeField] private byte[] sendBytes;
    public byte[] SendBytes {
        get { return sendBytes; }
        set { sendBytes = value; }
    }
    
    [SerializeField] private bool sendBool;
    public bool SendBool {
        get { return sendBool; }
        set { sendBool = value; }
    }
    
    [SerializeField] private int sendInt;
    public int SendInt {
        get { return sendInt; }
        set { sendInt = value; }
    }
    
    [SerializeField] private float sendFloat;
    public float SendFloat {
        get { return sendFloat; }
        set { sendFloat = value; }
    }
    
    [SerializeField] private string sendString;
    public string SendString {
        get { return sendString; }
        set { sendString = value; }
    }
    
    [SerializeField] private Vector2 sendVector2;
    public Vector2 SendVector2 {
        get { return sendVector2; }
        set { sendVector2 = value; }
    }

    [SerializeField] private Vector3 sendVector3;
    public Vector3 SendVector3 {
        get { return sendVector3; }
        set { sendVector3 = value; }
    }

    [SerializeField] private Vector4 sendVector4;
    public Vector4 SendVector4 {
        get { return sendVector4; }
        set { sendVector4 = value; }
    }

    [SerializeField] private Vector2Int sendVector2Int;
    public Vector2Int SendVector2Int {
        get { return sendVector2Int; }
        set { sendVector2Int = value; }
    }

    [SerializeField] private Vector3Int sendVector3Int;
    public Vector3Int SendVector3Int {
        get { return sendVector3Int; }
        set { sendVector3Int = value; }
    }
}
