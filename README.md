# UnityArduinoComms
Simple Unity to Android Serial Protocol. The library uses COBS to encode the data packets to enable you to send the full range of 255 bytes. It also calculates CRC16 checksums to verify packet integrity.

## Unity Setup

### Arduino Handler

You need to add the Arduino Prefab to your scene. It has the Arduino Handler Component
attached to it.

![Arduino Handler](https://github.com/dominik-zisch/UnityArduinoComms/blob/master/img/ArduinoHandler.png?raw=true)

It has the following properties:

* **Port**: you need to set the arduinos serial port here. The easiest way to determine this is through the Arduino IDE. In the top bar, select Tools - Port and check the serial port next to your micro controller. On Windows it will be COMX, where X is a number. On macOS it will be /dev/cu* where * usually starts with usbmodem if it's an Arduino.

* **Baudrate**: the speed of communication between the Arduino and Unity. You need to make sure this matches whatever you set in the Arduino script. The libraries default is 115200.

* **Buffer Size**: the size of the incoming serial buffer in bytes. If you're not planning on sending large amounts of data, you can just ignore this.

* **Packets Waiting**: This shows the packets currently waiting to be processed. If the number doesn't stay close to 0 when you're running your game, then you're trysending too many packets from your Arduino for your Unity project to handle.

* **Verbose Encoded Receive**: Enabling this will show the encoded data packets that are being received within Unity. For debug purposes. Enabling this will affect your framerate if you're sending a lot of data.

* **Verbose Decoded Receive**: Enabling this will show the decoded data packets that are being received within Unity. For debug purposes. Enabling this will affect your framerate if you're sending a lot of data.

* **Verbose Encoded Send**: Enabling this will show the encoded data packets that are being sent from Unity. For debug purposes. Enabling this will affect your framerate if you're sending a lot of data.

* **Verbose Decoded Send**: Enabling this will show the decoded data packets that are being sent from Unity. For debug purposes. Enabling this will affect your framerate if you're sending a lot of data.

* **Message Callback**: You can register a callback function for packets received here. It is advised to use the Event receiver instead.

* **Startup Timer**: the library will wait this amount of seconds before it sends any data to Arduino to allow the arduino to exit the bootloader. Default of 2 seconds should be fine for most cases.

### Arduino Property Sender

You can use this to send properties to the Arduino.

![Arduino Property Sender](https://github.com/dominik-zisch/UnityArduinoComms/blob/master/img/ArduinoPropertySender.png?raw=true)

It has the following properties:

* **Arduino Handler**: You need to specify the Arduino Handler you want to use. If you don't specify any, it will find the first GameObject called Arduino and use that.

* **Framerate**: You can specify the framerate that you want to send the data at. (frames per seconds)

* **Command**: You can use this to specify a command byte. This is used to distinguish between different packets of the same type.

* **Data Source**: You specify the GameObject you want to send data from here. Once you have done so, the Component and Property will show up below.

* **Component**: Specify the component of the GameObject selected above.

* **Property**: Specify the property from the Component selected above.

### Arduino Field Sender

This works the same way as the property sender, but uses fields instead of senders.

### Arduino Event Receiver

You can use this to receive data from the Arduino. You can specify callback functions for the data. You need to make sure that the functions you indent to call (or properties) match the type of data you are receiving.

![Arduino Event Receiver](https://github.com/dominik-zisch/UnityArduinoComms/blob/master/img/ArduinoEventReceiver.png?raw=true)

It has the following properties:

* **Arduino Handler**: You need to specify the Arduino Handler you want to use. If you don't specify any, it will find the first GameObject called Arduino and use that.

* **Command Byte**: You can use this to specify a command byte. This is used to distinguish between different packets of the same type. You have to make sure that the command byte you send from Arduino matches this.

* **Data Type**: Specify the type of data you are expecting here. It will change the callback below accordingly.

* **Callbacks**: You can specify the callback for the received data here.


## Arduino

To use this library you need to add the Arduino Libraryu to your libraries folder. On both Windows and macOS this can be found under Documents/Arduino/libraries.

You have to include the library in your sketch:

```c
#include "UnityComms.h"
```

Add a UnityComms object and call it's setup function in the sketch's setup:

```c
void setup()
{
  unity.setup();
  ...
}
```

If you are sending data from Unity to the Arduino, you have to call the receive function in the loop:

```c
void loop()
{
  unity.receive();
  ...
}
```

You also have to set callback functions for the datatype you want to receive. You have to make sure to pass the callback function with the & before the function name! A full list of callbacks and their headers can be found in the EchoExample. An example for floats below:

```c
void onFloatReceived (int cmd, float value)
{
  // you can check the command byte here and use the value.
}

void setup()
{
  // put your setup code here, to run once:
  unity.setup();

  // set unity callbacks
  unity.floatCallback = &onFloatReceived;
}
```

If you want to send data from Arduino to unity, you can use one of the send functions. A full list of send send fucntions can be found in the EchoExample. Example for the sendFloat function below:

```c
void loop()
{
  ...
  unity.sendFloat(1, 5.5);
  ...
}
```
