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
	byte AnalogStat3 = 0;

};

struct RECEIVE_DATA_STRUCTURE {
	unsigned long enc;
	byte id = 0;
	int outState = 0;
	int batV = 0;	//Battary Volt
	int batA = 0;	//Battary Amper
	int commstat = 0;
};


SEND_DATA_STRUCTURE MyData;
RECEIVE_DATA_STRUCTURE RXData;

#define CMD_DIGIO 10
#define CMD_ANALOGIO 11
#define CMD_TEXT 12
#define PIN_HIGH 3
#define PIN_LOW 2

byte olddigcmd = 0; // Old Digital keys command status


long debounceDelay = 400;    // the debounce time; increase if the output flickers
long key_Debounce_Time[] = { 0,0,0,0,0,0,0,0 };
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

	ETout.begin(details(MyData), &RCSerial);
	ETin.begin(details(RXData), &RCSerial);

	//define pin modes
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, INPUT);
	EEPROM.put(5, 32763);
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Initial Display
}

void loop() {

	if (ETin.receiveData()) {                      // если пришел пакет   
		Serial.println("ID: "); Serial.println(RXData.id);
		if (RXData.id == 2) {                       // и совпал id
			EEPROM.get(5, oldcountRX);               // достаем из EEPROM счетчик
			Serial.println("oldcountRX: "); Serial.println(oldcountRX);
			countRX = k.decrypt(RXData.enc);           // декодируем 
			Serial.println("countRX: "); Serial.println(countRX);
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
			//print the value of the array location
			//Serial.println(note2sing[n]);
				
			if ((millis() - key_Debounce_Time[n]) > debounceDelay) {
				#ifdef DEBUG
				Serial.println("Key");
				Serial.println(n);
				#endif
				// More debug code...
				Serial.println("Input command");
				Serial.println(switchVar1, BIN);
				//Togle for status
				MyData.keysStat ^= 1 << n;
				Serial.println("Status");
				Serial.println(MyData.keysStat, BIN);
				key_Debounce_Time[n] = millis();
							}
						}
			
		

			displaydata();
		}
	//}
	
	if ((olddigcmd != MyData.keysStat) | (RXData.outState != MyData.keysStat))
	{
		Serial.print("OLDCommand"); Serial.println(olddigcmd, BIN);
		Serial.print("MyData.keystatus"); Serial.println(MyData.keysStat, BIN);
		Serial.print("RXData.outState"); Serial.println(RXData.outState, BIN);

		SendData(); //Send Data to Remote unit
		olddigcmd = MyData.keysStat;
	}

	//Read Analog Data
	Trim1_val = analogRead(Trim1pin);
	Trim1_val = map(Trim1_val, 0, 1023, 0, 179);
	Trim2_val = analogRead(Trim2pin);
	Trim2_val = map(Trim2_val, 0, 1023, 0, 179);
	Trim3_val = analogRead(Trim3pin);
	Trim3_val = map(Trim3_val, 0, 1023, 0, 179);
	
	if (Trim1_val != Trim1_val_status) {
		MyData.AnalogStat1 = Trim1_val;
		Trim1_val_status = Trim1_val;

			}
	if (Trim2_val != Trim2_val_status) {
		MyData.AnalogStat2 = Trim2_val;
		Trim1_val_status = Trim2_val;

			}
	if (Trim3_val != Trim3_val_status) {
		MyData.AnalogStat3 = Trim3_val;
		Trim3_val_status = Trim3_val;

		
	}
	}

	void SendData() {
		
		//Serial.println(MyData.type);
		EEPROM.get(0, count);                          // Get from EEPROM int 
		count--; 		// take away 1
		Serial.println("Count ID:"); Serial.println(count);

		MyData.enc = k.encrypt(count);                   // Encrypting data 
		ETout.sendData();                                 // Send Data 
		EEPROM.put(0, count);
		Serial.println("Count from EPROM:"); Serial.println(EEPROM.get(0, oldcount));
		                          // Save int to EEPROM
		//Serial.println(millis());
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

void displaydata() {
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1.1);
	display.setCursor(0, 0);
	display.println("Communication:");
	display.setCursor(95, 0);
	display.println("OK");
	//display.setCursor(0, 10);
	//display.setCursor(65, 10);
	//display.println("mW");
	//display.setCursor(0, 20);
	//display.println(energy);
	//display.setCursor(65, 20);
	//display.println("mWh");
	display.display();
}
