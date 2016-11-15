//********************************************
//*
//* Battary and OLED Monitor http://ndogo.sodaq.net/using_-an_oled_screen_to_display_the_battery_voltage_reading/
//*****

//Include the necessary libraries
#include <AnalogButtons.h>
#include "CyberLib.h"

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10

#define ANALOG_PIN A0


// Set Trimmer Pin
int Trim1pin = A1;
int Trim2pin = A2;
int Trim3pin = A3;

int Trim1_val;
int Trim2_val;
int Trim3_val;

//String inText;
////////////////////////////////////////////////

void Key1Click()
{
	//Serial.print("button 1 clicked");
	Serial.print(START_CMD_CHAR);
	Serial.print(CMD_DIGITALWRITE);
	Serial.print(DIV_CMD_CHAR);
	Serial.print(1);
	Serial.print("\t");
}
void Key2Click()
{
	Serial.print("button 2 clicked");
}
void Key2Hold()
{
	Serial.print("button 2 held");
}

void Key3Click()
{
	Serial.print("button 3 clicked");
}
void Key4Click()
{
	Serial.print("button 4 clicked");
}

void Key5Click()
{
	Serial.print("button 5 clicked");
}

void Key6Click()
{
	Serial.print("button 6 clicked");
}

void Key7Click()
{
	Serial.print("button 7 clicked");
}

void Key8Click()
{
	Serial.print("button 8 clicked");
}

void Key9Click()
{
	Serial.print("button 9 clicked");
}


//Button Activation 
AnalogButtons analogButtons(ANALOG_PIN, 30);
Button Key1 = Button(1017, &Key1Click);
Button Key2 = Button(283, &Key2Click, Key2Hold);
Button Key3 = Button(242, &Key3Click);
Button Key4 = Button(201, &Key4Click);
Button Key5 = Button(171, &Key5Click);
Button Key6 = Button(141, &Key6Click);
Button Key7 = Button(119, &Key7Click);
Button Key8 = Button(91, &Key8Click);
Button Key9 = Button(70, &Key9Click);



void setup() {
	Serial.begin(9600);
	Serial.println("Arduino Transmitter Ver.0.1");
//	Serial.flush();

//Setup Analog Buttons
	analogButtons.add(Key1);
	analogButtons.add(Key2);
	analogButtons.add(Key3);
	analogButtons.add(Key4);
	analogButtons.add(Key5);
	analogButtons.add(Key6);
	analogButtons.add(Key7);
	analogButtons.add(Key8);
	analogButtons.add(Key9);


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

	//Check Analog buttons	
	analogButtons.check();

	
	Trim1_val = analogRead(Trim1pin);
	Trim1_val = map(Trim1_val, 0, 1023, 0, 179);
//	Serial.print(Trim1_val);
	//delay(250);
}

