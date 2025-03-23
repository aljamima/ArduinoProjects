//
// AUTHOR: Nitin Sharma
// PURPOSE: Example to showcase the use of Nokia5110 lcd library
//
#include <Nokia5110.h>
#include <SoftwareSerial.h>

// LCD object
Nokia5110 lcd;

// For showing time since program started
unsigned long time;
char string[8];

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
  lcd.LcdXY(0,2);
  lcd.LcdWriteString("Seconds");
}

void loop() {
  time = millis();
  lcd.LcdXY(45, 2);
  Serial.println(time / 1000);
  lcd.LcdWriteString(dtostrf((time / 1000), 3, 0, string));
  delay(1000);
}
