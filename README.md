
##Remote control system based on Arduino and HC-12 long range module

I created this project to help my friend extend an existing remote control system and add additional commands to his project.<br />

I want to say BIG thanks to Gregory Gonorovsky for help with creating and debugging the code <br />.

This project comprises: [Transmitter](https://github.com/igorvin/Arduino-Remote-Control/blob/master/Transmitter/Transmitter/Transmitter.ino) and [Receiver](https://github.com/igorvin/Arduino-Remote-Control/blob/master/Receiver/Receiver/Receiver.ino).<br />
Both were created with Microsoft Visual Studio.<br />
The project was created and tested on Arduino version 1.6.6<br />

##Specifications

**Libraries:**<br />
		**EEPROM**- Built in Arduino library. Ability to write to Eeprom memory<br />
		**Servo**-  Built in Arduino library. Enable Servo motors<br />
		**SoftwareSerial**- Built in Arduino library. Enable software serial for HC-12 module<br />
		**EasyTransfer**- [Easy transfer data between two arduino](http://www.billporter.info/2011/05/30/easytransfer-arduino-library/)

###Functionality:
- Remote control long range, up to 1.8km<br />
- Control 8 relays and 2 servo motors<br />
- Two way communication<br />
- GPIO status memory after reboot<br />
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



### Configuration

Before start using HC-12 modules I strongly recommend configure those modules.
How to do that you can find very good explanation in the following link:<br />
[HC-12 Configuration](http://www.instructables.com/id/Long-Range-18km-Arduino-to-Arduino-Wireless-Commun/step2/Wiring-and-setup/)<br />
Or additionally you can try this utility: [https://github.com/robert-rozee/HC-12-config](https://github.com/robert-rozee/HC-12-config)<br />

