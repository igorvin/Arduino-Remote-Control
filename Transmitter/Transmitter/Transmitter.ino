//********************************************
//* Created by Igor Vinokur
//* Email: igorvin2@gmail.com
// Project:RF 433 Remote Control
//*****

//Include the necessary libraries
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Keeloq.h>
#include <EasyTransfer.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>

SoftwareSerial RCSerial(2, 3); // RX, TX


//Set Display 
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_INA219 ina219;

Keeloq k(0x01325324, 0x05064718);        /// Encryption Key
unsigned int count = 65535;		//ID sent to Receiver
unsigned int oldcountRX = 75535;  //ID Received from Receiver
unsigned int countRX;  //ID 
unsigned int oldcount;
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
	byte id = 0;
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

//define where your pins are
int latchPin = 5; //was 8
int dataPin = 6; //was9
int clockPin = 4; //was 7

#define casevalue
#define CommOK 55 //Communication code status
char commstat; //Coomunicatiom Status
#define DEBUG 1


byte switchVar1 = 72;  //01001000
char note2sing[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

// Set Trimmer Pin
int Trim1pin = A1;
int Trim2pin = A2;
//int Trim3pin = A3;

int Trim1_val;
int Trim2_val;
//int Trim3_val;

byte Trim1_val_status;
byte Trim2_val_status;
//byte Trim3_val_status;


String inText;
////////////////////////////////////////////////

void setup() {
	delay(200);
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
	EEPROM.get(10, TXData.keysStat);
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Initial Display
}

void loop() {
	//delay(200);
	if (ETin.receiveData()) {                      // если пришел пакет   
		Serial.println("Data Received!!!, ID: "); Serial.println(RXData.id);
		if (RXData.id == 2) {                       // и совпал id
			Serial.println("ID Received OK!!! ");
			EEPROM.get(5, oldcountRX);               // достаем из EEPROM счетчик
			countRX = k.decrypt(RXData.enc);           // декодируем 
#ifdef DEBUG
		//	Serial.println("oldcountRX: "); Serial.println(oldcountRX);
		//	Serial.println("countRX: "); Serial.println(countRX);
#endif
			if (countRX >= oldcountRX) {                // если счетчик больше или равен сохраненного        
				countRX--;                             // отнимаем 1
				EEPROM.put(5, countRX);                // пишим в еепром
				Serial.println("Data Recived is correct!!!!");

			}
			else Serial.println("ALARM!!! Received Wrong Data");            // received previus packet
		}
	}

	digitalWrite(latchPin, 1);
	delayMicroseconds(20);
	digitalWrite(latchPin, 0);
	switchVar1 = shiftIn(dataPin, clockPin);
	//Serial.println(switchVar1, BIN);
	for (int n = 0; n <= 7; n++)
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
			displaydata();
		}
	//Read Analog Data
	Trim1_val = analogRead(Trim1pin);
	TXData.AnalogStat1 = map(Trim1_val, 0, 1023, 0, 179);
	Trim2_val = analogRead(Trim2pin);
	TXData.AnalogStat2 = map(Trim2_val, 0, 1023, 0, 179);
	
	if ((olddigcmd != TXData.keysStat)) // || (TXData.AnalogStat1 != Trim1_val_status) || (TXData.AnalogStat2 != Trim2_val_status))
	{
		Serial.print("OLDCommand"); Serial.println(olddigcmd, BIN);
		Serial.print("TXData.keystatus"); Serial.println(TXData.keysStat, BIN);
		//Serial.print("RXData.outState"); Serial.println(RXData.outState, BIN);

		if (RXData.commstat == CommOK)
			commstat = 'OK';
		else  commstat = 'Err';


		SendData(); //Send Data to Remote unit
		olddigcmd = TXData.keysStat;
		EEPROM.put(10, TXData.keysStat);
		TXData.AnalogStat1 = Trim1_val_status;
		TXData.AnalogStat2 = Trim2_val_status;

	}

	}

	void SendData() {	
		//Serial.println(TXData.type);
		EEPROM.get(0, count);                          // Get from EEPROM int 
		count--; 		// take away 1
		//Serial.println("Count ID:"); Serial.println(count);
		TXData.enc = k.encrypt(count);                   // Encrypting data 
		ETout.sendData();                                 // Send Data 
		EEPROM.put(0, count);
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
	
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1.1);
	display.setCursor(0, 0);
	display.println("Communication:");
	display.setCursor(95, 0);
	display.println(commstat);
	//display.setCursor(0, 10);
	//display.setCursor(65, 10);
	//display.println("mW");
	//display.setCursor(0, 20);
	//display.println(energy);
	//display.setCursor(65, 20);
	//display.println("mWh");
	display.display();
}
