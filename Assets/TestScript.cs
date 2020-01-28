using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestScript : MonoBehaviour
{
    public bool ArduinoSendData
    {
        get => arduinoSendData;
        set => arduinoSendData = value;
    }

    public bool ArduinoSendDataCheck
    {
        get => arduinoSendDataCheck;
        set => arduinoSendDataCheck = value;
    }

    public float AppleVal
    {
        get => appleVal;
        set => appleVal = value;
    }

    public float BananaVal
    {
        get => bananaVal;
        set => bananaVal = value;
    }

    public float OrangeVal
    {
        get => orangeVal;
        set => orangeVal = value;
    }
    
    [SerializeField] private bool arduinoSendData = false;
    [SerializeField] private bool arduinoSendDataCheck = false;
    [SerializeField] private float appleVal;
    [SerializeField] private float bananaVal;
    [SerializeField] private float orangeVal;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void setVector3(Vector3 data)
    {
        appleVal = data.x;
        bananaVal = data.y;
        orangeVal = data.z;
    }
}
