using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tests : MonoBehaviour
{

    public float f;

    public byte[] b;

    public string s;

// Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        b = BitConverter.GetBytes(f);
        s = Utils.ByteArrayToHexString(b);
    }
}
