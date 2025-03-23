//
// AUTHOR: Nitin Sharma
// PURPOSE: Nokia 5110 lcd library for Arduino
//
// HISTORY:
// see Nokia5110.cpp
//
// inspired by Julian Ilett's youtube tutorials
//
//

--> How to install library:

1. Zip the Nokia5110.h and Nokia5110.cpp in a single file named Nokia5110Lib.zip
2. Import the zip file in Arduino IDE
3. See examples folder to understand how to import and use

--> Arduino Pin Configurations:

*Default* PIN settings (If init() is used to initialise the LCD)
	RST -> pin 6 arduino
	CE -> pin 5 arduino
	DC -> pin 4 arduino
	DIN -> pin 3 arduino
	CLK -> pin 2 arduino
	VCC -> 3.3v
	BL	-> 3.3v
	GND	-> GND
	
*Custom* PIN settings (For this you can use init(CLK,DIN,DC,CE,RST) to initialise LCD. See examples folder)
	For this configuration, the CLK pin has to be a PWM pin. Rest can be any digital pin
	RST -> Any digital pin
	CE -> Any digital pin
	DC -> Any digital pin
	DIN -> Any digital pin
	CLK -> PWM enabled digital pin
	VCC -> 3.3v
	BL	-> 3.3v
	GND	-> GND