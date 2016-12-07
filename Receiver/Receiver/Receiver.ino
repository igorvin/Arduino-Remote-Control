﻿/*
Name:    Receiver.ino
Created: 16/11/2016 8:26:38 AM
Author:  Igor Vinokur
Email: Igorvin2@gmail.comSec, I'm checking
Libraries:
https://github.com/pythonista/CyberLib
*/

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

	//D12_High;
	//delay_ms(500);
	//D12_Low;
	//D11_High;
	//delay_ms(500);
	//D11_Low;
	//D10_High;
	//delay_ms(500);
	//D10_Low;
	//D9_High;
	//delay_ms(500);
	//D9_Low;
	//D8_High;
	//delay_ms(500);
	//D8_Low;
	//D7_High;
	//delay_ms(500);
	//D7_Low;
	//D6_High;
	//delay_ms(500);
	//D6_Low;
	//D5_High;
	//delay_ms(500);
	//D5_Low;
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
				Serial.println("Data Recived is correct!!!!");

			}
			else Serial.println("ALARM!!! Received Wrong Data");            // received previus packet
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
		pinMode(12, OUTPUT);
		digitalWrite(12, pin_value);
		// add your code here      
		break;
	case 2:
		pinMode(11, OUTPUT);
		digitalWrite(11, pin_value);
		// add your code here       
		break;
	case 3:
		pinMode(10, OUTPUT);
		digitalWrite(10, pin_value);
		// add your code here 
		break;
	case 4:
		pinMode(9, OUTPUT);
		digitalWrite(9, pin_value);
		// add your code here 
		break;
	case 5:
		pinMode(8, OUTPUT);
		digitalWrite(8, pin_value);
		// add your code here 
		break;
	case 6:
		pinMode(7, OUTPUT);
		digitalWrite(7, pin_value);
		// add your code here 
		break;
	case 7:
		pinMode(6, OUTPUT);
		digitalWrite(6, pin_value);
		// add your code here 
		break;
	case 8:
		pinMode(5, OUTPUT);
		digitalWrite(5, pin_value);
		// add your code here 
		break;
		// default: 
		// if nothing else matches, do the default
		// default is optional
	}
}

