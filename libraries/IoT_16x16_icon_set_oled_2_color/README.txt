Icon Set Name: IoT Icon set 16x16 bi color
Version: 1.0.0
Origin URL:  www.engsta.com
Author: Artur Funk
License: GNU General Public License v3


Description: 
This icon set with icons of 16x16 pixels was especialy created for low 
resolution display which are often used in display enabled IoT projects.
It supports LCD's or OLED Displays.

Each icon will use 32 bytes of microcontroller flash. If you dont want 
to waste you programm space, just delete or comment unused icons.

Tested on:	
Used Library: Adafruit_SSD1306		https://github.com/adafruit/Adafruit_SSD1306
			  Adafruit-GFX-Library	https://github.com/adafruit/Adafruit-GFX-Library
	
	
How to use:	
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "#include "iot_iconset_16x16.h"

// Drawing an icon
display.drawBitmap(0, 34, arrow_down_icon16x16 =, 16, 16, 1);

Use it to make something cool, have fun, 
and share what you've learned with others.
