using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pot : MonoBehaviour
{
    [SerializeField] private int potValue;
    public int PotValue
    {
        get { return potValue; }
        set { potValue = value; }
    }
}
