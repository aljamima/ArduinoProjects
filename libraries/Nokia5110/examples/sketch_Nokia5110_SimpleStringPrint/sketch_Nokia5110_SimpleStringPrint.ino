//
// AUTHOR: Nitin Sharma
// PURPOSE: Example to showcase the use of Nokia5110 lcd library
//
#include <Nokia5110.h>
#include <SoftwareSerial.h>

// LCD object
Nokia5110 lcd;

void setup() {
  //
  // Initialize LCD with your custom pin configurations
  // initLCD(CLK,DIN,DC,CE,RST)
  // CLK needs to be PWM enabled
  //
  lcd.initLCD(6,2,3,4,5);
  
  // Alternatively use the default lcd.initLCD(). In that
  // case default arduino configuration would be used which
  // is as follows:
  //
  // RST -> pin 6 arduino
  // CE -> pin 5 arduino 
  // DC -> pin 4 arduino
  // DIN -> pin 3 arduino
  // CLK -> pin 2 arduino
  // 
  // lcd.initLCD();

  // Mover cursor to X,Y and print a string. If the string is too
  // long it would extend to further rows automatically
  lcd.LcdXY(0,0);
  lcd.LcdWriteString("Nokia 5110");
  lcd.LcdXY(0,1);
  lcd.LcdWriteString("is a pretty");
  lcd.LcdXY(0,2);
  lcd.LcdWriteString("cool LCD");
  lcd.LcdXY(0,3);
  lcd.LcdWriteString("to use :)");

  // Clear LCD after 30 seconds
  delay(30000);
  lcd.clearLCD();
}

void loop() {
  
}
