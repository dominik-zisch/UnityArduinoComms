using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArduinoReceiveTest : MonoBehaviour
{
    
    [SerializeField] private byte[] receiveBytes;
    public byte[] ReceiveBytes {
        get { return receiveBytes; }
        set { receiveBytes = value; }
    }
    
    [SerializeField] private bool receiveBool;
    public bool ReceiveBool {
        get { return receiveBool; }
        set { receiveBool = value; }
    }
    
    [SerializeField] private int receiveInt;
    public int ReceiveInt {
        get { return receiveInt; }
        set { receiveInt = value; }
    }
    
    [SerializeField] private float receiveFloat;
    public float ReceiveFloat {
        get { return receiveFloat; }
        set { receiveFloat = value; }
    }
    
    [SerializeField] private string receiveString;
    public string ReceiveString {
        get { return receiveString; }
        set { receiveString = value; }
    }
    
    [SerializeField] private Vector2 receiveVector2;
    public Vector2 ReceiveVector2 {
        get { return receiveVector2; }
        set { receiveVector2 = value; }
    }

    [SerializeField] private Vector3 receiveVector3;
    public Vector3 ReceiveVector3 {
        get { return receiveVector3; }
        set { receiveVector3 = value; }
    }

    [SerializeField] private Vector4 receiveVector4;
    public Vector4 ReceiveVector4 {
        get { return receiveVector4; }
        set { receiveVector4 = value; }
    }

    [SerializeField] private Vector2Int receiveVector2Int;
    public Vector2Int ReceiveVector2Int {
        get { return receiveVector2Int; }
        set { receiveVector2Int = value; }
    }

    [SerializeField] private Vector3Int receiveVector3Int;
    public Vector3Int ReceiveVector3Int {
        get { return receiveVector3Int; }
        set { receiveVector3Int = value; }
    }
}
