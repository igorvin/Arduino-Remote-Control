/*
//Name:    Receiver.ino
//Created: 16/11/2016 8:26:38 AM
//Author:  Igor Vinokur
//Email: Igorvin2@gmail.com
//Libraries: 
	//EEPROM; Servo; SoftwareSerial; Keeloq; EasyTransfer
*/

#include <EEPROM.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <EasyTransfer.h>

//Define Serial port pins
SoftwareSerial RCSerial(2, 3); // RX, TX


EasyTransfer ETin, ETout;
struct RECEIVE_DATA_STRUCTURE {
	unsigned long enc;                    // Counter 
	byte id = 1;
	byte keysStat = 0;
	byte AnalogStat1 = 0;	//Analog Trimmer value
	byte AnalogStat2 = 0;	//Analog Trimmer value
	//byte AnalogStat3 = 0;	//Analog Trimmer value
};
struct SEND_DATA_STRUCTURE {
	unsigned long enc;
	int id = 578;
	int outStat = 0;
	int batV = 0; //Battary Volt
	int batA = 0; //Battary Amper
	byte commstat = 0;

};
RECEIVE_DATA_STRUCTURE RXData;
SEND_DATA_STRUCTURE TXData;

// Status codes for comunications
#define CommOK 55 //Communication OK code

long commstat_val = 0; //Test communication status
long commstat_check_time = 8000;	//Test communication Interval
long delaycomminit = 10000;
long comminit_val = 0;
long oldAnalogstat1 = 0;
long oldAnalogstat2 = 0;

byte OutputsState = 0;

#define DEBUG 1 //Enable or Disable Debug option

// create servo object to control a servo
Servo Servo1;
Servo Servo2;
//Servo Servo3;

int gpioPins[] = { 12, 11, 10, 9, 8, 7, 6, 5 }; // GPIO for diffiren uses (like relay, switches)
int ledstat;
// the setup function runs once when you press reset or power the board
void setup() {

// Define GPIO to OUTPUT mode	
	for (int index = 0; index <= 7; index++) { pinMode(gpioPins[index], OUTPUT);} 

//Set Servo pins
	Servo1.attach(4);
	Servo2.attach(13);
	Servo1.write(0);
	Servo2.write(0);
	//Servo3.attach(4);
//Set Serials		
	Serial.begin(9600);
	RCSerial.begin(9600);
	
	Serial.println("ArduDroid Receiver 0.1");
	Serial.flush();
//EasyTransfer settings	
	ETin.begin(details(RXData), &RCSerial);
	ETout.begin(details(TXData), &RCSerial);
	
	//TXData.commstat = CommInit;

EEPROM.get(5, RXData.keysStat); 
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (OutputsState != RXData.keysStat) {
		SetGPIO();
	}
	//Send Coomunication status every 10 sec
	if ((millis() - commstat_val) > commstat_check_time) {
		TXData.commstat = CommOK;
		commstat_val = millis();
		Serial.println(TXData.commstat);
		SendData();
	}
	if (ETin.receiveData())  {                      // если пришел пакет   
		Serial.println(RXData.id);
		if (RXData.id == 1) { // и совпал id
          		TXData.commstat = CommOK;
		}
//Send Coomunication status every 10 sec
				if ((millis() - commstat_val) > commstat_check_time) {
					TXData.commstat = CommOK;
					commstat_val = millis();
					Serial.println(TXData.commstat);
				}

				SetGPIO();//Set GPIO function
				
			GetGPIOsat();
			SendData();    //Send Data to Remote unit 
			
			}

	
	}

	void SetGPIO() {
		for (byte n = 0; n <= 7; n++)
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
		OutputsState = RXData.keysStat;
		EEPROM.put(5, RXData.keysStat);
		if ((oldAnalogstat1 != RXData.AnalogStat1) || (oldAnalogstat2 != RXData.AnalogStat2)) {
			Serial.print("AnalogStat1:   "); Serial.println(RXData.AnalogStat1);
			Serial.print("AnalogStat2:   "); Serial.println(RXData.AnalogStat2);
			Servo1.write(RXData.AnalogStat1);
			Servo2.write(RXData.AnalogStat2);
			oldAnalogstat1 == RXData.AnalogStat1;
			oldAnalogstat2 == RXData.AnalogStat2;
				}

	}
	void GetGPIOsat(){
		for (byte n1 = 0; n1 <= 7; n1++)
		{
			if (digitalRead(gpioPins[n1]) == 1)

				TXData.outStat |= 1 << n1;
			else
				TXData.outStat &= ~(1 << n1);
		}
			//Serial.print("Led Pin : "); Serial.println(ledstat);
			//Serial.print("OutputsState:"); Serial.println(OutputsState);
			OutputsState = TXData.outStat;
			Serial.print("TXData: "); Serial.println(TXData.outStat, BIN);
			EEPROM.put(5, TXData.outStat);

		}

	
	void SendData() {

		//for (byte n1 = 0; n1 <= 1; n1++){
			Serial.print("Send ID:   "); Serial.println(TXData.id);
			ETout.sendData();                                 // Send Data 
				delay(200);
	}

