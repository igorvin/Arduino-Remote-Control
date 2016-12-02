/*
 Name:		Receiver.ino
 Created:	16/11/2016 8:26:38 AM
 Author:	igorv
 Libraries:
 https://github.com/pythonista/CyberLib
*/


#include <Servo.h>
#include <SoftwareSerial.h>
#include "CyberLib.h"

SoftwareSerial RCSerial(2, 3); // RX, TX

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11
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
	Servo1.attach(2);
	Servo1.attach(3);
	Servo1.attach(4);

	Serial.begin(9600);
	RCSerial.begin(9600);
	Serial.println("ArduDroid Receiver 0.1");
	Serial.flush();

	D12_Out;
	D11_Out;
	D10_Out;
	D9_Out;
	D8_Out;
	D7_Out;
	D6_Out;
	D5_Out;

	D12_High;
	delay_ms(500);
	D12_Low;
	D11_High;
	delay_ms(500);
	D11_Low;
	D10_High;
	delay_ms(500);
	D10_Low;
	D9_High;
	delay_ms(500);
	D9_Low;
	D8_High;
	delay_ms(500);
	D8_Low;
	D7_High;
	delay_ms(500);
	D7_Low;
	D6_High;
	delay_ms(500);
	D6_Low;
	D5_High;
	delay_ms(500);
	D5_Low;



}

// the loop function runs over and over again until power down or reset
void loop() {

	Serial.flush();
	int ard_command = 0;
	int pin_num = 0;
	int pin_value = 0;

	char get_char = ' ';  //read serial


	// wait for incoming data
	if (RCSerial.available() < 1) return; // if serial empty, return to loop().

// parse incoming command start flag 
	get_char = RCSerial.read();
	if (get_char != START_CMD_CHAR) return; // if no command start flag, return to loop().

// parse incoming command type
	ard_command = RCSerial.parseInt(); // read the command
// parse incoming pin# and value  
	pin_num = RCSerial.parseInt(); // read the pin
	pin_value = RCSerial.parseInt();  // read the value

// 1) GET TEXT COMMAND FROM ARDUDROID
	if (ard_command == CMD_TEXT) {
		inText = ""; //clears variable for new input   
		while (RCSerial.available()) {
			char c = RCSerial.read();  //gets one byte from serial buffer
			delay(5);
			if (c == END_CMD_CHAR) { // if we the complete string has been read
									 // add your code here
				break;
			}
			else {
				if (c != DIV_CMD_CHAR) {
					inText += c;
					delay(5);
				}
			}
		}
	}

// 2) GET digitalWrite DATA FROM ARDUDROID
	if (ard_command == CMD_DIGITALWRITE) {
		if (pin_value == PIN_LOW) pin_value = LOW;
		else if (pin_value == PIN_HIGH) pin_value = HIGH;
		else return; // error in pin value. return. 
		set_digitalwrite(pin_num, pin_value);  // Uncomment this function if you wish to use 
		return;  // return from start of loop()
	}

// 3) GET analogWrite DATA FROM ARDUDROID
	if (ard_command == CMD_ANALOGWRITE) {
		analogWrite(pin_num, pin_value);
		// add your code here
		return;  // Done. return to loop();
	}

// 4) SEND DATA TO ARDUDROID
	if (ard_command == CMD_READ_ARDUDROID) {
		// char send_to_android[] = "Place your text here." ;
		// Serial.println(send_to_android);   // Example: Sending text
		Serial.print(" Analog 0 = ");
		Serial.println(analogRead(A0));  // Example: Read and send Analog pin value to Arduino
		return;  // Done. return to loop();
	}

// wait for incoming data
	if (Serial.available() < 1) return; // if serial empty, return to loop().

// parse incoming command start flag 
	get_char = Serial.read();
	if (get_char != START_CMD_CHAR) return; // if no command start flag, return to loop().

// parse incoming command type
	ard_command = Serial.parseInt(); // read the command

// parse incoming pin# and value  
	pin_num = Serial.parseInt(); // read the pin
	pin_value = Serial.parseInt();  // read the value

// 1) GET TEXT COMMAND FROM ARDUDROID
	if (ard_command == CMD_TEXT) {
		inText = ""; //clears variable for new input   
		while (Serial.available()) {
			char c = Serial.read();  //gets one byte from serial buffer
			delay(5);
			if (c == END_CMD_CHAR) { // if we the complete string has been read
									 // add your code here
				break;
			}
			else {
				if (c != DIV_CMD_CHAR) {
					inText += c;
					delay(5);
				}
			}
		}
	}

// 2) GET digitalWrite DATA FROM ARDUDROID
	if (ard_command == CMD_DIGITALWRITE) {
		if (pin_value == PIN_LOW) pin_value = LOW;
		else if (pin_value == PIN_HIGH) pin_value = HIGH;
		else return; // error in pin value. return. 
		set_digitalwrite(pin_num, pin_value);  // Uncomment this function if you wish to use 
		return;  // return from start of loop()
	}

// 3) GET analogWrite DATA FROM ARDUDROID
	if (ard_command == CMD_ANALOGWRITE) {
		analogWrite(pin_num, pin_value);
		// add your code here
		return;  // Done. return to loop();
	}

// 4) SEND DATA TO ARDUDROID
	if (ard_command == CMD_READ_ARDUDROID) {
		// char send_to_android[] = "Place your text here." ;
		// Serial.println(send_to_android);   // Example: Sending text
		Serial.print(" Analog 0 = ");
		Serial.println(analogRead(A0));  // Example: Read and send Analog pin value to Arduino
		return;  // Done. return to loop();
	}
}

// 2a) select the requested pin# for DigitalWrite action
void set_digitalwrite(int pin_num, int pin_value)
{
	switch (pin_num) {
	case 13:
		pinMode(13, OUTPUT);
		digitalWrite(13, pin_value);
		// add your code here      
		break;
	case 12:
		pinMode(12, OUTPUT);
		digitalWrite(12, pin_value);
		// add your code here       
		break;
	case 11:
		pinMode(11, OUTPUT);
		digitalWrite(11, pin_value);
		// add your code here 
		break;
	case 10:
		pinMode(10, OUTPUT);
		digitalWrite(10, pin_value);
			// add your code here 
		break;
	case 9:
		pinMode(9, OUTPUT);
		digitalWrite(9, pin_value);
			// add your code here 
		break;
	case 8:
		pinMode(8, OUTPUT);
		digitalWrite(8, pin_value);
			// add your code here 
		break;
	case 7:
		pinMode(7, OUTPUT);
		digitalWrite(7, pin_value);
			// add your code here 
		break;
	case 6:
		pinMode(6, OUTPUT);
		digitalWrite(6, pin_value);
			// add your code here 
		break;
	case 5:
		pinMode(5, OUTPUT);
		digitalWrite(5, pin_value);
			// add your code here       
		break;
	case 4:
		pinMode(4, OUTPUT);
		digitalWrite(4, pin_value);
			// add your code here 
		break;
	case 3:
		pinMode(3, OUTPUT);
		digitalWrite(3, pin_value);
			// add your code here 
		break;
	case 2:
		pinMode(2, OUTPUT);
		digitalWrite(2, pin_value);
			// add your code here       
		break;
		// default: 
		// if nothing else matches, do the default
		// default is optional
	}
}
  

