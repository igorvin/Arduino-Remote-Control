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
#define CMD_ANALOGWRITE 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255

long key1_Debounce_Time = 0;  // the last time the output pin was toggled
long debounceDelay = 300;    // the debounce time; increase if the output flickers


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

String inText;
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
				#ifdef DEBUG
				//Serial.println("Key1");
				// More debug code...
				#endif
				if ((millis() - key1_Debounce_Time) > debounceDelay) {
					Serial.print(START_CMD_CHAR);
					Serial.print(CMD_DIGITALWRITE);
					Serial.print(DIV_CMD_CHAR);
					Serial.print(1);
					Serial.print(END_CMD_CHAR);
					Serial.print("\t");
					key1_Debounce_Time = millis();
				}
				break;
			case 'B':    // your hand is close to the sensor
					#ifdef DEBUG
				//Serial.println("Key2");
				// More debug code...
					#endif
				Serial.print(START_CMD_CHAR);
				Serial.print(CMD_DIGITALWRITE);
				Serial.print(DIV_CMD_CHAR);
				Serial.print(2);
				Serial.print(END_CMD_CHAR);
				Serial.print("\t");
				
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
	Trim2_val = analogRead(Trim2pin);
	Trim2_val = map(Trim2_val, 0, 1023, 0, 179);
	Trim3_val = analogRead(Trim3pin);
	Trim3_val = map(Trim3_val, 0, 1023, 0, 179);
//	Serial.print(Trim1_val);
	//delay(250);

	int ard_command = 0;
	int pin_num = 0;
	int pin_value = 0;

	char get_char = ' ';  //read serial

						  // wait for incoming data
	if (Serial.available() < 1) return; // if serial empty, return to loop().

										// parse incoming command start flag 
	get_char = Serial.read();
	#ifdef DEBUG
			Serial.println("get_char: ");
			Serial.println(get_char);
	// More debug code...
	#endif
	if (get_char != START_CMD_CHAR) return; // if no command start flag, return to loop().

											// parse incoming command type
	ard_command = Serial.parseInt(); // read the command
	#ifdef DEBUG
		Serial.println("ard_command: ");
		Serial.println(ard_command);
	// More debug code...
	#endif

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
		Serial.println("Received Key:");
		Serial.println(pin_value);
		//if (pin_value == PIN_LOW) pin_value = LOW;
		//else if (pin_value == PIN_HIGH) pin_value = HIGH;
		//else return; // error in pin value. return. 
		//set_digitalwrite(pin_num, pin_value);  // Uncomment this function if you wish to use 
		//return;  // return from start of loop()
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
