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
#include <EasyTransfer.h>

//Define Serial port pins
SoftwareSerial RCSerial(2, 3); // RX, TX

Keeloq k(0x01325324, 0x05064718);        /// Encryption Key
unsigned int oldCount = 65535;
unsigned int count; 
unsigned int countTX = 75535; //ID sent to transmitter

EasyTransfer ETin, ETout;
struct RECEIVE_DATA_STRUCTURE {
	unsigned long enc;                    // Counter 
	byte id = 1;
	byte keysStat = 0;
	byte AnalogStat1 = 0;	//Analog Trimmer value
	byte AnalogStat2 = 0;	//Analog Trimmer value
	byte AnalogStat3 = 0;	//Analog Trimmer value
};

struct SEND_DATA_STRUCTURE {
	unsigned long enc;
	byte id = 2;
	int outStat = 0;
	int batV = 0;	//Battary Volt
	int batA = 0;	//Battary Amper
	int commstat = 0; //communication status

};

// Status codes for comunications
#define CommInit 50 // Initilize communication on boot
#define CommOK 55 //Communication OK code

long commstat_val = 0; //Test communication status
long commstat_check_time = 10000;	//Test communication Interval
long delaycomminit = 3000;
long comminit_val = 0;

byte OutputsState;

RECEIVE_DATA_STRUCTURE RXData;
SEND_DATA_STRUCTURE TXData;

#define DEBUG 1 //Enable or Disable Debug option

int ledPins [] = {12, 11, 10, 9, 8, 7, 6, 5}; // GPIO for diffiren uses (like relay, switches)

// create servo object to control a servo
Servo Servo1;
Servo Servo2;
Servo Servo3;

// the setup function runs once when you press reset or power the board
void setup() {

for (int index = 0; index <= 7; index++) { pinMode(ledPins[index], OUTPUT);} // Define GPIO to OUTPUT mode
	
	//Set Servo pin
	Servo1.attach(13);
	Servo2.attach(4);
	//Servo3.attach(4);
		
	Serial.begin(9600);
	RCSerial.begin(9600);
	Serial.println("ArduDroid Receiver 0.1");
	Serial.flush();
	
	ETin.begin(details(RXData), &RCSerial);
	ETout.begin(details(TXData), &RCSerial);
	TXData.commstat = CommInit;

//EEPROM.put(0, 4233); // Atantion!!!! Only if need reset ID
}

// the loop function runs over and over again until power down or reset
void loop() {

	
	if (ETin.receiveData()) {                      // если пришел пакет   
		Serial.println(RXData.id);
		if (RXData.id == 1) { // и совпал id
           	EEPROM.get(0, oldCount);               // достаем из EEPROM счетчик
			count = k.decrypt(RXData.enc);           // декодируем 
#ifdef DEBUG
			Serial.print("oldcount:"); Serial.println(oldCount);
			
#endif // !DEBUG
			if (count >= oldCount) {                // если счетчик больше сохраненного   
				count--;                             // отнимаем 1
				EEPROM.put(0, count);                // пишим в еепром
				Serial.print("count:"); Serial.println(count);
				Serial.println("Data Recived is correct!!!!");
				TXData.commstat = 52;

				if ((millis() - commstat_val) > commstat_check_time) {
					TXData.commstat = CommOK;
					commstat_val = millis();
					Serial.println(TXData.commstat);
					}

			

		for (byte n=0; n<=7; n++)
		{
			//Serial.println(n);
			
			switch (n) {
			case 0:
				#ifdef DEBUG
				Serial.println("Key1 detected");
				#endif
				if (RXData.keysStat & (1 << n)) 
					digitalWrite(12, HIGH);
				else
					digitalWrite(12, LOW);
				break;
			case 1:
				#ifdef DEBUG
				Serial.println("Key2 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(11, HIGH);
				else
					digitalWrite(11, LOW);
				break;
			case 2:
				#ifdef DEBUG
				Serial.println("Key3 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(10, HIGH);
				else
					digitalWrite(10, LOW);
				break;
			case 3:
				#ifdef DEBUG
				Serial.println("Key4 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(9, HIGH);
				else
					digitalWrite(9, LOW);
				break;
			case 4:
				#ifdef DEBUG
				Serial.println("Key5 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(8, HIGH);
				else
					digitalWrite(8, LOW);
				break;
			case 5:
				#ifdef DEBUG
				Serial.println("Key6 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(7, HIGH);
				else
					digitalWrite(7, LOW);
				break;
			case 6:
				#ifdef DEBUG
				Serial.println("Key7 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(6, HIGH);
				else
					digitalWrite(6, LOW);
				break;
			case 7:
				#ifdef DEBUG
				Serial.println("Key8 detected");
				#endif
				if (RXData.keysStat & (1 << n))
					digitalWrite(5, HIGH);
				else
					digitalWrite(5, LOW);
				break;
			}
		}

		for (int n = 0; n <= 7; n++)
		{
			OutputsState & (1 << digitalRead(ledPins[n]));
		}
		//Serial.print("OutputsState:"); Serial.println(OutputsState);
		TXData.outStat = OutputsState;
		//Serial.println(TXData.outStat,BIN);
		delay(1000);
	

		SendData(); //Send Data to Remote unit
	}

}
}
	else 
	//	Serial.println("ALARM!!! Received Wrong Data");            // received previus packet
	if ((millis() - comminit_val) > delaycomminit) {
		//Serial.print("ID:"); Serial.println(TXData.id);
		// Serial.print("CommStat ID:"); Serial.println(TXData.commstat);
		if (TXData.commstat == CommInit)
			for (int n = 0; n <= 7; n++)
			{
				OutputsState & (1 << digitalRead(ledPins[n]));
			}
			TXData.outStat = OutputsState;
			Serial.println("Send Init data to transmitter");
		SendData();
		comminit_val = millis();
	}
		}

void SendData() {
	//Serial.println(MyData.type);
	EEPROM.get(1, countTX);                          // Get from EEPROM int 
	countTX--;                                       // take away 1
	RXData.enc = k.encrypt(countTX);                   // Encrypting data 
	ETout.sendData();                                 // Send Data 
	EEPROM.put(1, countTX);                          // Save int to EEPROM
}

