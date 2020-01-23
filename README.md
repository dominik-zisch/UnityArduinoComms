# UnityArduinoComms

Simple Unity to Arduino Serial Protocol.


## System Requirements

The package requires a ```System.IO.Ports``` implementation. You need to enable .Net 4.x compatibility level in Player settings for this to work.


## Installation

To install ArduinoComms to your project, download the ```.unitypackage``` file from the Releases page, then either drag and drop it into your unity project or go to **Assets > Import Package > Custom Package...** and navigate to the downloaded file. Import everything in the package to your project.

To change the compatibility level, navigate to **Edit > Project Settings > Player > Other Settings > Configuration > Api Compatibility Level** and change this setting to **.NET 4.x**


## Components

### ArduinoHandler

Attach the ArduinoHandler script to a GameObject. Get the serial port for the arduino and add it to the Port property. Change the Baudrate to match your Arduino scripts baudrate. If you plan on sending very large packets, adjust the Buffer Size accordingly.

The ArduinoHandler component offers a Message Callback. Use this to trigger messages when packets come in. The functions have to accept an ArduinoBuffer as the only parameter. This contains the command byte, the data type, and the buffer.


### Arduino Property Sender

This component can be used to stream data to the arduino. You need to give it a reference to the ArduinoHandler.


### Arduino Event Receiver

This component can be used to receive streaming data from the arduino. it also need a reference to the ArduinoHandler and provides callbacks for the different data types.


## Disclaimer

The EventReceiver and PropertySender components were heavily influenced by [Keijira's OSCJack package](https://github.com/keijiro/OscJack).


## License

Public domain.
