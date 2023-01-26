using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Compass : MonoBehaviour
{
    [SerializeField] private float headingRaw;
    public float HeadingRaw {
        get { return headingRaw; }
        set { headingRaw = value; }
    }
}
