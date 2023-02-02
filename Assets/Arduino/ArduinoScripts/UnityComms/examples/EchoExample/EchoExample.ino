/*
 * This is an example for the UnityArduinoComms plugin to echo data
 * from Unity to an Arduino back to Unity. The Unity library can be found here:
 *
 * https://github.com/dominik-zisch/UnityArduinoComms
 *
 */

#include "UnityComms.h"

//============================================================================//
//=========================================================// Unity Declarations

UnityComms unity;


//============================================================================//
//============================================================// Unity Callbacks

void onBytesReceived      (int cmd, uint8_t* value, size_t len)                             { unity.sendBytes(cmd, value, len); }
void onBoolReceived       (int cmd, bool value)                                             { unity.sendBool(cmd, value); }
void onIntReceived        (int cmd, int32_t value)                                          { unity.sendInt(cmd, value); }
void onFloatReceived      (int cmd, float value)                                            { unity.sendFloat(cmd, value); }
void onStringReceived     (int cmd, String value)                                           { unity.sendString(cmd, value); }
void onVector2IntReceived (int cmd, int32_t value1, int32_t value2)                         { unity.sendVector2Int(cmd, value1, value2); }
void onVector3IntReceived (int cmd, int32_t value1, int32_t value2, int32_t value3)         { unity.sendVector3Int(cmd, value1, value2, value3); }
void onVector2Received    (int cmd, float value1, float value2)                             { unity.sendVector2(cmd, value1, value2); }
void onVector3Received    (int cmd, float value1, float value2, float value3)               { unity.sendVector3(cmd, value1, value2, value3); }
void onVector4Received    (int cmd, float value1, float value2, float value3, float value4) { unity.sendVector4(cmd, value1, value2, value3, value4); }


//============================================================================//
//========================================================// Lifecycle functions

//--------------------------------------------------------//
//------------------------------// setup - called at startup
void setup()
{
  // put your setup code here, to run once:
  unity.setup();

  // set unity callbacks
  unity.bytesCallback = &onBytesReceived;
  unity.boolCallback = &onBoolReceived;
  unity.intCallback = &onIntReceived;
  unity.floatCallback = &onFloatReceived;
  unity.stringCallback = &onStringReceived;
  unity.vector2IntCallback = &onVector2IntReceived;
  unity.vector3IntCallback = &onVector3IntReceived;
  unity.vector2Callback = &onVector2Received;
  unity.vector3Callback = &onVector3Received;
  unity.vector4Callback = &onVector4Received;
}


//--------------------------------------------------------//
//--------------------------------// loop - called in a loop
void loop()
{
  // receive data from computer
  unity.receive();

  // put your main code here, to run repeatedly:
}
