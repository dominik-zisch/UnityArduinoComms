using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TexLedSender : MonoBehaviour
{
    public Color[] colors;
    public byte[] bytes;
    [Range(0, 255)] public int brightness = 50;

    private GradientSwoosh swoosh;
    private int numLeds = 60;

    public byte[] Bytes => bytes;


    // Start is called before the first frame update
    void Start()
    {
        swoosh = GetComponent<GradientSwoosh>();
        numLeds = swoosh.TextureLength;
        colors = new Color[numLeds];
        bytes = new byte[numLeds * 3 + 1];
    }

    // Update is called once per frame
    void Update()
    {
        bytes[0] = (byte) brightness;
        for (int i = 0; i < numLeds; i++)
        {
            Color c = swoosh.texture.GetPixel(i, 0);
            colors[i] = c;
            bytes[1 + i * 3 + 0] = (byte) (c.r * 255);
            bytes[1 + i * 3 + 1] = (byte) (c.g * 255);
            bytes[1 + i * 3 + 2] = (byte) (c.b * 255);
        }
    }
}
