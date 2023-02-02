using UnityEngine;
using System.Collections;
using System;

public class GradientSwoosh : MonoBehaviour {

	public Gradient gradient;
	public Texture2D texture;
	public int TextureLength;
	private float inc; 
	public float timeMult;
	public float scale = 1.0f;
	
	public int frameRate = 25;
	private float currentTime = 0;
	private int currentFrame = 0;
	private int lastFrame;

	void Start () {

		//-----------------------------------------------------------// Texture
		// Create a new texture ARGB32 (32 bit with alpha) and no mipmaps
		texture 		   = new Texture2D(TextureLength, 1, TextureFormat.ARGB32, false);
		texture.filterMode = FilterMode.Point;

		inc = 1.0f/(TextureLength * 1);

		// set the pixel values
		for (int x = 0; x < TextureLength; x++) {
			texture.SetPixel (x,0, new Color (0.5f,0.5f,0.5f));
		}

		// Apply all SetPixel calls
		texture.Apply();

		// connect texture to material of GameObject this script is attached to
		GetComponent<Renderer>().material.mainTexture = texture;
		GetComponent<Renderer>().material.SetTexture("_EmissionMap" ,texture);

		//-------------------------------------------------------------------// 

	}

	// Update is called once per frame
	void Update () {

		lastFrame = currentFrame;
		currentFrame = (int)(currentTime * (float)frameRate);					// calculate current frame from elapsied time since beginning of sequence

		if (lastFrame != currentFrame) {
			int   cnt = 0;
			float pos; 
			Color c;

			for (int x = 0; x < TextureLength; x++) {

				pos = Mathf.Repeat( (inc * cnt * scale) + (Time.time * timeMult),1.0f);
				c   = gradient.Evaluate (pos);
				texture.SetPixel (x, 0, c);

				cnt++;
			}

			// JBS_PrototypeOutput proto = GetComponent<JBS_PrototypeOutput> ();
			// proto.SendData (texture, Flag);

			texture.Apply();
		}

		currentTime += Time.deltaTime;											// add this frames duration to overall time
	}
}