//*
//Name:    Receiver.ino
//Created: 16/11/2016 8:26:38 AM
//Author:  Igor Vinokur
//Email: Igorvin2@gmail.comSec, I'm checking
//Libraries:
//https://github.com/pythonista/CyberLib
//*/

#include <EEPROM.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Keeloq.h>
#include <SoftEasyTransfer.h>
#include "CyberLib.h"

SoftwareSerial RCSerial(2, 3); // RX, TX

Keeloq k(0x01325324, 0x05064718);        /// Encryption Key
unsigned int oldCount = 65535;
unsigned int count; 

SoftEasyTransfer ET;

struct RECEIVE_DATA {
	unsigned long enc;                    // Counter 
	byte id = 1;
	int pin = 0;                          // Pin number 
	int type = 0;							// Analog/Digital
	int val = 0;                         // Pin Value 
	int cmd = 0;
	int key = 0;
};

RECEIVE_DATA RXData;


#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGIO 10
#define CMD_ANALOGIO 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255
#define PIN_HIGH 3
#define PIN_LOW 2

#define DEBUG 1
String inText;


// create servo object to control a servo
Servo Servo1;
Servo Servo2;
Servo Servo3;

// the setup function runs once when you press reset or power the board
void setup() {

	//Set Servo pin
	//Servo1.attach(2);
	//Servo2.attach(3);
	//Servo3.attach(4);
	
	
	Serial.begin(9600);
	RCSerial.begin(9600);
	Serial.println("ArduDroid Receiver 0.1");
	Serial.flush();
	ET.begin(details(RXData), &RCSerial);

	//D12_Out;
	//D11_Out;
	//D10_Out;
	//D9_Out;
	//D8_Out;
	//D7_Out;
	//D6_Out;
	//D5_Out;

	
}

// the loop function runs over and over again until power down or reset
void loop() {

	
	int pin_num = 0;
	int pin_value = 0;
	

	if (ET.receiveData()) {                      // если пришел пакет   
    
		if (RXData.id = 1) {                       // и совпал id
			EEPROM.get(0, oldCount);               // достаем из EEPROM счетчик
			count = k.decrypt(RXData.enc);           // декодируем 
			if (count <= oldCount) {                // если счетчик больше или равен сохраненного        
				count--;                             // отнимаем 1
				EEPROM.put(0, count);                // пишим в еепром
				//Serial.println("Data Recived is correct!!!!");

			}
			//else Serial.println("ALARM!!! Received Wrong Data");            // received previus packet
		}
		


		// 2) GET digitalWrite DATA FROM ARDUDROID
		if (RXData.type == CMD_DIGIO) {
			if (RXData.val == PIN_LOW) pin_value = LOW;
			else if (RXData.val == PIN_HIGH) pin_value = HIGH;
			else return; // error in pin value. return. 
			set_digitalwrite(RXData.key, pin_value);  // Uncomment this function if you wish to use 
			return;  // return from start of loop()
		}

		// 3) GET analogWrite DATA FROM ARDUDROID
		if (RXData.type == CMD_ANALOGIO) {
			analogWrite(RXData.pin, RXData.val);
			// add your code here
			return;  // Done. return to loop();
		}

		
	}
}
// 2a) select the requested pin# for DigitalWrite action
void set_digitalwrite(int pin_num, int pin_value)
{
	switch (RXData.key) {
	case 1:
		#ifdef DEBUG
			Serial.println("Key1 detected");
		#endif
		pinMode(12, OUTPUT);
		digitalWrite(12, pin_value);
		// add your code here      
		break;
	case 2:
		#ifdef DEBUG
			Serial.println("Key2 detected");
		#endif
		pinMode(11, OUTPUT);
		digitalWrite(11, pin_value);
		// add your code here       
		break;
	case 3:
		#ifdef DEBUG
			Serial.println("Key3 detected");
		#endif
		pinMode(10, OUTPUT);
		digitalWrite(10, pin_value);
		// add your code here 
		break;
	case 4:
		#ifdef DEBUG
			Serial.println("Key4 detected");
		#endif
		pinMode(9, OUTPUT);
		digitalWrite(9, pin_value);
		// add your code here 
		break;
	case 5:
		#ifdef DEBUG
			Serial.println("Key5 detected");
		#endif
		pinMode(8, OUTPUT);
		digitalWrite(8, pin_value);
		// add your code here 
		break;
	case 6:
		#ifdef DEBUG
			Serial.println("Key6 detected");
		#endif
		pinMode(7, OUTPUT);
		digitalWrite(7, pin_value);
		// add your code here 
		break;
	case 7:
		#ifdef DEBUG
			Serial.println("Key7 detected");
		#endif
		pinMode(6, OUTPUT);
		digitalWrite(6, pin_value);
		// add your code here 
		break;
	case 8:
		#ifdef DEBUG
			Serial.println("Key8 detected");
		#endif
		pinMode(5, OUTPUT);
		digitalWrite(5, pin_value);
		// add your code here 
		break;
		// default: 
		// if nothing else matches, do the default
		// default is optional
	}
}


