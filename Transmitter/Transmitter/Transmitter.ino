//********************************************
//* Created by Igor Vinokur
//* Email: igorvin2@gmail.com
// Project:RF 433 Remote Control
//*****

//Include the necessary libraries
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <EasyTransfer.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>

SoftwareSerial RCSerial(2, 3); // RX, TX


//Set Display 
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_INA219 ina219;

EasyTransfer ETin, ETout;

struct SEND_DATA_STRUCTURE {
	unsigned long enc;                    // Counter 
	byte id = 1; //Sent ID
	byte keysStat = 0;
	byte AnalogStat1 = 0;
	byte AnalogStat2 = 0;
	//byte AnalogStat3 = 0;

};

struct RECEIVE_DATA_STRUCTURE {
	unsigned long enc;
	int id = 0;
	int outStat = 0;
	int batV = 0;	//Battary Volt
	int batA = 0;	//Battary Amper
	byte commstat = 0;
};

SEND_DATA_STRUCTURE TXData;
RECEIVE_DATA_STRUCTURE RXData;

byte olddigcmd = 0; // Old Digital keys command status

long debounceDelay = 300;    // the debounce time; increase if the output flickers
long key_Debounce_Time[] = { 0,0,0,0,0,0,0,0 };
long Trim_debonceDelay = 100;
long Trim1_Debounce_Time = 0;
long Trim2_Debounce_Time = 0;
long RXReceivedTime = 0;
long RXReceiveDelay_Time = 10000;

char note2sing[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

//define where your pins are
int latchPin = 5; //was 8
int dataPin = 6; //was9
int clockPin = 4; //was 7

String stCommandStatus = "_ _ _ _ _ _ _ _";

//#define casevalue
#define CommOK 55 //Communication code status
String commstat; //Coomunicatiom Status
#define DEBUG 1


byte switchVar1 = 72;  //01001000

// Set Trimmer Pins
int Trim1pin = A0;
int Trim2pin = A1;
//int Trim3pin = A3;

int Trim1_val;
int Trim2_val;
//int Trim3_val;

byte Trim1_val_status;
byte Trim2_val_status;
//byte Trim3_val_status;

//String inText;
////////////////////////////////////////////////

void setup() {
	//delay(200);
	RCSerial.begin(9600);
	Serial.begin(9600);
	Serial.println("Arduino Transmitter Ver.0.1");
	RCSerial.flush();

	ETout.begin(details(TXData), &RCSerial);
	ETin.begin(details(RXData), &RCSerial);

	//define pin modes
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, INPUT);
	//EEPROM.put(5, 32763); // Atantion!!!! Only if need reset ID
	EEPROM.get(10, TXData.keysStat);  //Get old status from EEPROM
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Initial Display
}

void loop() {

	//delay(200);
	if (ETin.receiveData()) {                      // если пришел пакет   
		Serial.print("Data Received!!!, ID: "); Serial.println(RXData.id);
		if (RXData.id == 578) {
			RXReceivedTime = millis();
			//Serial.print("Data Recived is correct!!!!, Time: "); Serial.println(RXReceivedTime);
		}
		else {
			RXData.id = 0;
			RXData.commstat = 0;
			RXData.outStat = 0;
			//Serial.println("ALARM!!! Received Wrong Data");            // received previus packet
		}
	}
	if ((RXReceivedTime + RXReceiveDelay_Time) < millis()) {
		RXData.commstat = 0;
		//Serial.print("No Communication , Last time was: "); Serial.println(RXReceivedTime);
	}

		digitalWrite(latchPin, 1);
		delayMicroseconds(20);
		digitalWrite(latchPin, 0);
		switchVar1 = shiftIn(dataPin, clockPin);
		//Serial.println(switchVar1, BIN);
		for (byte n = 0; n <= 7; n++)
		{
			if (switchVar1 & (1 << n)) {
				if ((millis() - key_Debounce_Time[n]) > debounceDelay) {
#ifdef DEBUG
					//Serial.println("Key");
					//Serial.println(n);
#endif
		//Togle for status
					TXData.keysStat ^= 1 << n;

					key_Debounce_Time[n] = millis();
				}
			}
		}

		//Read Analog Data
		if ((millis() - Trim1_Debounce_Time) > Trim_debonceDelay) {
			Trim1_val = analogRead(Trim1pin);
			TXData.AnalogStat1 = map(Trim1_val, 1023, 0, 0, 254);
			Trim1_Debounce_Time = millis();
		}
		if ((millis() - Trim2_Debounce_Time) > Trim_debonceDelay) {
			Trim2_val = analogRead(Trim2pin);
			TXData.AnalogStat2 = map(Trim2_val, 1023, 0, 0, 254);
			Trim2_Debounce_Time = millis();
		}

		if ((olddigcmd != TXData.keysStat) ||
			((Trim1_val_status > TXData.AnalogStat1 + 3) || (Trim1_val_status < TXData.AnalogStat1 - 3)) ||
			((Trim2_val_status > TXData.AnalogStat2 + 3) || (Trim2_val_status < TXData.AnalogStat2 - 3)))
		{
			Serial.print("Trim1:  "); Serial.println(TXData.AnalogStat1);
			Serial.print("Trim2:  "); Serial.println(TXData.AnalogStat2);

			SendData(); //Send Data to Remote unit
			olddigcmd = TXData.keysStat;
			EEPROM.put(10, TXData.keysStat);
			Trim1_val_status = TXData.AnalogStat1;
			Trim2_val_status = TXData.AnalogStat2;
		}
		//Update Communication status on Display
		RXOutstat();
		displaydata(); //Display

	}


	void SendData() {	
		//Serial.println(TXData.type);
		////EEPROM.get(0, count);                          // Get from EEPROM int 
		////count--; 		// take away 1
		//Serial.println("Count ID:"); Serial.println(count);
		////TXData.enc = k.encrypt(count);                   // Encrypting data 
		ETout.sendData();                                 // Send Data 
		////EEPROM.put(0, count);
		//Serial.println("Count from EPROM:"); Serial.println(EEPROM.get(0, oldcount));
		                          // Save int to EEPROM
		//Serial.println(millis());
	}
	
byte shiftIn(int TXDataPin, int myClockPin) {
	int i;
	int temp = 0;
	int pinState;
	byte TXDataIn = 0;
	pinMode(myClockPin, OUTPUT);
	pinMode(TXDataPin, INPUT);
	for (i = 7; i >= 0; i--)
	{
		digitalWrite(myClockPin, 0);
		delayMicroseconds(0.2);
		temp = digitalRead(TXDataPin);
		if (temp) {
			pinState = 1;
			TXDataIn = TXDataIn | (1 << i);
		}
		else {
			pinState = 0;
		}
		digitalWrite(myClockPin, 1);
	}
	return TXDataIn;
}

void displaydata() {
	if (RXData.commstat == CommOK)
		commstat = "OK";
	else
		commstat = "Error";
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1.1);
	display.setCursor(0, 0);
	display.println("Network: ");
	display.setCursor(80, 0);
	display.println(commstat);
	display.setCursor(0, 10);
	display.println("KEY:");
	display.setCursor(30, 10);
	display.println(stCommandStatus);

	//display.setCursor(0, 20);
	//display.println(energy);
	//display.setCursor(65, 20);
	//display.println("mWh");
	display.display();
}

	void RXOutstat(){
		for (byte n = 0; n <= 7; n++) {
			if (RXData.outStat & (1 << n)) stCommandStatus.setCharAt(n * 2, char(49 + n));
			else stCommandStatus.setCharAt(n * 2, '_');

		}
	}

