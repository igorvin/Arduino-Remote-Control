
##Remote control system based on Arduino and HC-12 long range module

I created this project to help my friend extend existing remote control system and add additional commands to his project.<br />
This project contains two separated projects Transmitter and Receiver.
Both was created on Microsoft Visual Studio.<br />
Arduino files you can find by the following links:<br />
Transmitter: https://github.com/igorvin/Arduino-Remote-Control/blob/master/Transmitter/Transmitter/Transmitter.ino<br />
Receiver: https://github.com/igorvin/Arduino-Remote-Control/blob/master/Receiver/Receiver/Receiver.ino<br />
The project was created and tested on Arduino version 1.6.6<br />

##Specifications

**Libraries:**<br />
		**EEPROM**- Built in Arduino library. Ability to write to Eeprom memory<br />
		**Servo**-  Built in Arduino library. Enable Servo motors<br />
		**SoftwareSerial**- Built in Arduino library. Enable software serial for HC-12 module<br />
		**EasyTransfer**-[http://www.billporter.info/2011/05/30/easytransfer-arduino-library/](http://www.billporter.info/2011/05/30/easytransfer-arduino-library/"). Easy transfer data between two arduino

###Functionality:
- Remote control long range, up to 2km<br />
- Control 8 relays and 2 servo motors<br />
- Two way communication<br />
- Remember GPIO status after reboot<br />
- Display communication status<br />
- Display GPIO status from receiver<br/>
		

###Transmitter:<br />
**Hardware:**<br />

- Arduino Nano<br />
- HC-12 433 MHz<br />
- 0.96" I2C IIC 128X64 LED OLED LCD Display<br />
- 2x Variable Resistor<br />

![alt tag](https://github.com/igorvin/Arduino-Remote-Control/blob/master/Fritzing/Transmitter.png)

###Receiver:<br />

**Hardware:**<br />

- Arduino Nano<br />
- HC-12 433 MHz<br />
- Relay board<br />


![alt tag](https://github.com/igorvin/Arduino-Remote-Control/blob/master/Fritzing/Receiver.png)



## Installation

Provide code examples and explanations of how to get the project.

