//********************************************
//* Created by Igor Vinokur
//* Email: igorvin2@gmail.com
// Project:RF 433 Remote Control
//*****

//Include the necessary libraries
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Keeloq.h>
#include <SoftEasyTransfer.h>

SoftwareSerial RCSerial(2, 3); // RX, TX

Keeloq k(0x01325324, 0x05064718);        /// Encryption Key
unsigned int count = 65535;

SoftEasyTransfer ET;

struct SEND_DATA {
	unsigned long enc;                    // Counter 
	byte id = 1;
	int pin = 0;                          // Pin number 
	int type = 0;							// Analog/Digital/Text
	int val = 0;                         // Pin Value 
	int cmd = 0;
	int key = 0;
};

SEND_DATA MyData;

#define CMD_DIGIO 10
#define CMD_ANALOGIO 11
#define CMD_TEXT 12
#define PIN_HIGH 3
#define PIN_LOW 2

long key1_Debounce_Time = 0;  // the last time the output pin was toggled
long key2_Debounce_Time = 0;  // the last time the output pin was toggled
long key3_Debounce_Time = 0;  // the last time the output pin was toggled
long key4_Debounce_Time = 0;  // the last time the output pin was toggled
long key5_Debounce_Time = 0;  // the last time the output pin was toggled
long key6_Debounce_Time = 0;  // the last time the output pin was toggled
long key7_Debounce_Time = 0;  // the last time the output pin was toggled
long key8_Debounce_Time = 0;  // the last time the output pin was toggled
long debounceDelay = 400;    // the debounce time; increase if the output flickers

//Keys Value
int Key1_value = 0;
int Key2_value = 0;
int Key3_value = 0;
int Key4_value = 0;
int Key5_value = 0;
int Key6_value = 0;
int Key7_value = 0;
int Key8_value = 0;



//define where your pins are
int latchPin = 8;
int dataPin = 9;
int clockPin = 7;

#define casevalue

#define DEBUG 1


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

int Trim1_val_status;
int Trim2_val_status;
int Trim3_val_status;


String inText;
////////////////////////////////////////////////


void setup() {
	delay(200);
	RCSerial.begin(9600);
	Serial.begin(9600);
	Serial.println("Arduino Transmitter Ver.0.1");
	
	RCSerial.flush();

	ET.begin(details(MyData), &RCSerial);

	//define pin modes
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, INPUT);
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
			Serial.println(switchVar1, BIN);
			switch ((note2sing[n])) {
			case 'A':    // your hand is on the sensor
				if ((millis() - key1_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key1");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 1;

					if (Key1_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key1_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key1_value = PIN_LOW;
					}

					key1_Debounce_Time = millis();

				}
				break;
			case 'B':    // your hand is close to the sensor

				if ((millis() - key2_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key2");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 2;

					if (Key2_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key2_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key2_value = PIN_LOW;
					}

					key2_Debounce_Time = millis();


				}
				break;

			case 'C':    // your hand is on the sensor
				if ((millis() - key3_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key3");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 3;

					if (Key3_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key3_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key3_value = PIN_LOW;
					}

					key3_Debounce_Time = millis();


				}
				break;

			case 'D':    // your hand is close to the sensor
				if ((millis() - key4_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key4");
					// More debug code...
					#endif
					
					MyData.type = CMD_DIGIO;
					MyData.key = 4;

					if (Key4_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key4_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key4_value = PIN_LOW;
					}

					key4_Debounce_Time = millis();

				}
				break;
			case 'E':    // your hand is close to the sensor
				if ((millis() - key5_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key5");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 5;

					if (Key5_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key5_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key5_value = PIN_LOW;
					}

					key5_Debounce_Time = millis();

				}
				break;
			case 'F':    // your hand is close to the sensor
				if ((millis() - key6_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key6");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 6;

					if (Key6_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key6_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key6_value = PIN_LOW;
					}

					key6_Debounce_Time = millis();

				}
				break;
			case 'G':    // your hand is close to the sensor
				if ((millis() - key7_Debounce_Time) > debounceDelay) {
				#ifdef DEBUG
					Serial.println("Key7");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 7;

					if (Key7_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key7_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key7_value = PIN_LOW;
					}

					key7_Debounce_Time = millis();
					
				}
				break;
			case 'H':    // your hand is close to the sensor
				if ((millis() - key8_Debounce_Time) > debounceDelay) {
					#ifdef DEBUG
					Serial.println("Key8");
					// More debug code...
					#endif
					MyData.type = CMD_DIGIO;
					MyData.key = 8;

					if (Key8_value < PIN_HIGH) {
						MyData.val = PIN_HIGH;
						Key8_value = PIN_HIGH;
					}
					else {
						MyData.val = PIN_LOW;
						Key8_value = PIN_LOW;
					}

					key8_Debounce_Time = millis();

				}
				break;
			}
			SendData(); //Send Data to Remote unit
		}
	}

	//Read Analog Data
	Trim1_val = analogRead(Trim1pin);
	Trim1_val = map(Trim1_val, 0, 1023, 0, 179);
	Trim2_val = analogRead(Trim2pin);
	Trim2_val = map(Trim2_val, 0, 1023, 0, 179);
	Trim3_val = analogRead(Trim3pin);
	Trim3_val = map(Trim3_val, 0, 1023, 0, 179);
	
	if (Trim1_val != Trim1_val_status) {
		MyData.type = CMD_ANALOGIO;
		MyData.key = 1;
		MyData.val = Trim1_val;
		Trim1_val_status = Trim1_val;

		SendData(); //Send Data to Remote unit
	}
	if (Trim2_val != Trim2_val_status) {
		MyData.type = CMD_ANALOGIO;
		MyData.key = 2;
		MyData.val = Trim2_val;
		Trim1_val_status = Trim1_val;

		SendData(); //Send Data to Remote unit
	}
	if (Trim3_val != Trim3_val_status) {
		MyData.type = CMD_ANALOGIO;
		MyData.key = 3;
		MyData.val = Trim3_val;
		Trim3_val_status = Trim3_val;

		SendData(); //Send Data to Remote unit
	}
	}

	void SendData() {
		//Serial.println(MyData.type);
		EEPROM.get(0, count);                          // Get from EEPROM int 
		count--;                                       // take away 1
		MyData.enc = k.encrypt(count);                   // Encrypting data 
		ET.sendData();                                 // Send Data 
		EEPROM.put(0, count);                          // Save int to EEPROM
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
