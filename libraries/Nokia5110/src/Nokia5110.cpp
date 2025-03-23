//
// FILE: Nokia5110.cpp
// AUTHOR: Nitin Sharma
// VERSION: 0.1.00
// PURPOSE: Nokia 5110 lcd library for Arduino. Print simple strings on the lcd using
// this library
//
// HISTORY:
// 0.1.00 by Nitin Sharma
//
// inspired by Julian Ilett's youtube tutorials
//
// Released to the public domain
//

#include "Nokia5110.h"

// Default pins for the LCD connections if not overridden using the init constructor
int RST=6;
int CE=5;
int DC=4;
int DIN=3;
int CLK=2;

/////////////////////////////////////////////////////
//
// PUBLIC
//

//
// Function to initialize the LCD. This function must be called as the first call
// to Nokia5110 library. Use this function if you want to initialize with the default
// pin configuration, else use the overridden function below
//
// RST -> pin 6 arduino
// CE -> pin 5 arduino
// DC -> pin 4 arduino
// DIN -> pin 3 arduino
// CLK -> pin 2 arduino
//
void Nokia5110::initLCD() {
  pinMode(RST, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  digitalWrite(RST, LOW);
  digitalWrite(RST, HIGH);
  
  LcdWriteCmd(0x21);  // LCD extended commands
  LcdWriteCmd(0xbf);  // set LCD Vop (contrast)
  LcdWriteCmd(0x04);  // set temp coefficent
  LcdWriteCmd(0x10);  // LCD bias mode 1:40
  LcdWriteCmd(0x20);  // LCD basic commands
  LcdWriteCmd(0x0C);  // LCD normal video
  
  for(int i=0; i<504; i++) LcdWriteData(0x00); // clear LCD
}

//
// The overridden function of the initLCD(). Using this function you can use your own
// custom arduino pin configrations
//
// NOTE : CLK pin must be connected to a PWM digital pin. Rest can be any digital pin
//
void Nokia5110::initLCD(int CLKin, int DINin, int DCin, int CEin, int RSTin) {
  RST=RSTin;
  CE=CEin;
  DC=DCin;
  DIN=DINin;
  CLK=CLKin;

  initLCD();
}

//
// Write a string on the selected XY position of the LCD. Use the function LcdXY to
// choose the XY location. See examples for details
//
void Nokia5110::LcdWriteString(char *characters)
{
  while(*characters) LcdWriteCharacter(*characters++);
}

//
// Clear all the LCD contents
//
void Nokia5110::clearLCD() {
  for(int i=0; i<504; i++) LcdWriteData(0x00); // clear LCD
}

//
// Choose the XY position of the cursor. Any further commands would start writing at
// this XY location
//
void Nokia5110::LcdXY(int x, int y)
{
  LcdWriteCmd(0x80 | x);  // Column.
  LcdWriteCmd(0x40 | y);  // Row.
}

/////////////////////////////////////////////////////
//
// PRIVATE
//
void Nokia5110::LcdWriteCharacter(char character)
{
  for(int i=0; i<5; i++) LcdWriteData(ASCII[character - 0x20][i]);
  LcdWriteData(0x00);
}

void Nokia5110::LcdWriteData(byte dat)
{
  digitalWrite(DC, HIGH); //DC pin is low for commands
  digitalWrite(CE, LOW);
  shiftOut(DIN, CLK, MSBFIRST, dat); //transmit serial data
  digitalWrite(CE, HIGH);
}

void Nokia5110::LcdWriteCmd(byte cmd)
{
  digitalWrite(DC, LOW); //DC pin is low for commands
  digitalWrite(CE, LOW);
  shiftOut(DIN, CLK, MSBFIRST, cmd); //transmit serial data
  digitalWrite(CE, HIGH);
}

