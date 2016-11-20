//********************************************
//*
//* Battary and OLED Monitor http://ndogo.sodaq.net/using_-an_oled_screen_to_display_the_battery_voltage_reading/
//*****

//Include the necessary libraries

#include "CyberLib.h"

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10

//define where your pins are
int latchPin = 8;
int dataPin = 9;
int clockPin = 7;
#define casevalue

byte switchVar1 = 72;  //01001000
char note2sing[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };


// Set Trimmer Pin
int Trim1pin = A1;
int Trim2pin = A2;
int Trim3pin = A3;

int Trim1_val;
int Trim2_val;
int Trim3_val;

//String inText;
////////////////////////////////////////////////








void setup() {
	Serial.begin(9600);
	Serial.println("Arduino Transmitter Ver.0.1");
//	Serial.flush();

//define pin modes
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, INPUT);

	//Set Pins

	D2_Out;
	D3_Out;
	D4_Out;
	D5_Out;
	D6_Out;
	D7_Out;
	D8_Out;
	D9_Out;

	D2_High;
		delay_ms(500);
		D2_Low;
	D3_High;
		delay_ms(500);
		D3_Low;
	D4_High;
		delay_ms(500);
		D4_Low;
	D5_High;
		delay_ms(500);
		D5_Low;
	D6_High;
		delay_ms(500);
		D6_Low;
	D7_High;
		delay_ms(500);
		D7_Low;
	D8_High;
		delay_ms(500);
		D8_Low;
	D9_High;
		delay_ms(500);
		D9_Low;
	

}

void loop() {

	digitalWrite(latchPin, 1);
	delayMicroseconds(20);
	digitalWrite(latchPin, 0);
	switchVar1 = shiftIn(dataPin, clockPin);
	//Serial.println(switchVar1, BIN);
	for (int n = 0; n <= 7; n++)
	{
		if (switchVar1 & (1 << n)) {
			//print the value of the array location
			//Serial.println(note2sing[n]);
			switch ((note2sing[n])) {
			case 'A':    // your hand is on the sensor
				Serial.println("Key1");
				Serial.print(START_CMD_CHAR);
				Serial.print(CMD_DIGITALWRITE);
				Serial.print(DIV_CMD_CHAR);
				Serial.print(1);
				Serial.print("\t");
				break;
			case 'B':    // your hand is close to the sensor
				Serial.println("Key2");
				break;
			case 'C':    // your hand is on the sensor
				Serial.println("Key3");
				break;
			case 'D':    // your hand is close to the sensor
				Serial.println("Key4");
				break;
			case 'E':    // your hand is close to the sensor
				Serial.println("Key5");
				break;
			case 'F':    // your hand is close to the sensor
				Serial.println("Key6");
				break;
			case 'G':    // your hand is close to the sensor
				Serial.println("Key7");
				break;
			case 'H':    // your hand is close to the sensor
				Serial.println("Key8");
				break;
			}


		}
	}
	
	
	Trim1_val = analogRead(Trim1pin);
	Trim1_val = map(Trim1_val, 0, 1023, 0, 179);
//	Serial.print(Trim1_val);
	//delay(250);

	delay(300);
}

byte shiftIn(int myDataPin, int myClockPin) {
	int i;
	int temp = 0;
	int pinState;
	byte myDataIn = 0;
	pinMode(myClockPin, OUTPUT);
	pinMode(myDataPin, INPUT);
	for (i = 7; i >= 0; i--)
	{
		digitalWrite(myClockPin, 0);
		delayMicroseconds(0.2);
		temp = digitalRead(myDataPin);
		if (temp) {
			pinState = 1;
			myDataIn = myDataIn | (1 << i);
		}
		else {
			pinState = 0;
		}
		digitalWrite(myClockPin, 1);
	}
	return myDataIn;
}
