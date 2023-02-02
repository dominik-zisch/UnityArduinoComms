using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

// Originally written by keijiro for his OscJack Unity asset.
// Adapted for use with this Arduino asset.
// Original code:
// https://github.com/keijiro/OscJack

[AddComponentMenu("Arduino/Arduino Event Receiver")]
public class ArduinoEventReceiver : MonoBehaviour
{
    
    #region Receiver event classes
    
    [System.Serializable] class BytesReceived : UnityEvent<byte[]> {}
    [System.Serializable] class BoolReceived : UnityEvent<bool> {}
    [System.Serializable] class IntReceived : UnityEvent<int> {}
    [System.Serializable] class FloatReceived : UnityEvent<float> {}
    [System.Serializable] class StringReceived : UnityEvent<string> {}
    [System.Serializable] class Int2Received : UnityEvent<Vector2Int> {}
    [System.Serializable] class Int3Received : UnityEvent<Vector3Int> {}
    [System.Serializable] class Vector2Received : UnityEvent<Vector2> {}
    [System.Serializable] class Vector3Received : UnityEvent<Vector3> {}
    [System.Serializable] class Vector4Received : UnityEvent<Vector4> {}
    
    #endregion
    
    #region Editable fields
    
    [SerializeField] ArduinoHandler _arduino;
    [SerializeField] int _command = 1;
    [SerializeField] ArduinoDataType _dataType = ArduinoDataType.Bytearray;

    [SerializeField] BytesReceived _bytesReceived = null;
    [SerializeField] BoolReceived _boolReceived = null;
    [SerializeField] IntReceived _intReceived = null;
    [SerializeField] FloatReceived _floatReceived = null;
    [SerializeField] StringReceived _stringReceived = null;
    [SerializeField] Int2Received _int2Received = null;
    [SerializeField] Int3Received _int3Received = null;
    [SerializeField] Vector2Received _vector2Received = null;
    [SerializeField] Vector3Received _vector3Received = null;
    [SerializeField] Vector4Received _vector4Received = null;
    
    #endregion
    
    #region Internal members
    
    #endregion
    
    #region MonoBehaviour implementation
    
    // Start is called before the first frame update
    void Start()
    {
        if (_arduino == null)
            _arduino = GameObject.Find("Arduino").GetComponent<ArduinoHandler>();

        _arduino.RegisterListener(OnDataReceive);
    }

    // Update is called once per frame
    void Update()
    {
        if (_arduino == null) return;
    }
    
    #endregion
    
    #region Arduino event callback

    void OnDataReceive(ArduinoBuffer data)
    {
        if (_command != data.cmd) return;
        if (_dataType != data.dataType) return;
        
        if (_dataType == ArduinoDataType.Bytearray)
        {
            byte[] ba = data.buffer;
            _bytesReceived.Invoke(ba);
        }
        else if (_dataType == ArduinoDataType.Bool)
        {
            bool b = BitConverter.ToBoolean(data.buffer, 0);
            _boolReceived.Invoke(b);
        }
        else if (_dataType == ArduinoDataType.Int)
        {
            int i = BitConverter.ToInt32(data.buffer, 0);
            _intReceived.Invoke(i);
        }
        else if (_dataType == ArduinoDataType.Float)
        {
            float f = BitConverter.ToSingle(data.buffer, 0);
            _floatReceived.Invoke(f);
        }
        else if (_dataType == ArduinoDataType.String)
        {
            string s = System.Text.Encoding.ASCII.GetString(data.buffer);
            _stringReceived.Invoke(s);
        }
        else if (_dataType == ArduinoDataType.Int2)
        {
            Vector2Int v = new Vector2Int(
                BitConverter.ToInt32(data.buffer, 0),
                BitConverter.ToInt32(data.buffer, 4));
            _int2Received.Invoke(v);
        }
        else if (_dataType == ArduinoDataType.Int3)
        {
            Vector3Int v = new Vector3Int(
                BitConverter.ToInt32(data.buffer, 0),
                BitConverter.ToInt32(data.buffer, 4),
                BitConverter.ToInt32(data.buffer, 8));
            _int3Received.Invoke(v);
        }
        else if (_dataType == ArduinoDataType.Vector2)
        {
            Vector2 v = new Vector2(
                BitConverter.ToSingle(data.buffer, 0),
                BitConverter.ToSingle(data.buffer, 4));
            _vector2Received.Invoke(v);
        }
        else if (_dataType == ArduinoDataType.Vector3)
        {
            Vector3 v = new Vector3(
                BitConverter.ToSingle(data.buffer, 0),
                BitConverter.ToSingle(data.buffer, 4),
                BitConverter.ToSingle(data.buffer, 8));
            _vector3Received.Invoke(v);
        }
        else if (_dataType == ArduinoDataType.Vector4)
        {
            Vector4 v = new Vector4(
                BitConverter.ToSingle(data.buffer, 0),
                BitConverter.ToSingle(data.buffer, 4),
                BitConverter.ToSingle(data.buffer, 8),
                BitConverter.ToSingle(data.buffer, 12));
            _vector4Received.Invoke(v);
        }
    }
    
    #endregion
}
