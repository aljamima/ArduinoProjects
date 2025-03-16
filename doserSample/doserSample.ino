#include <DS1307RTC.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Time.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define doser1 0

void setup() {
  lcd.begin(16, 2);
  pinMode(doser1, OUTPUT);
  digitalWrite(doser1, HIGH);
  
}

void loop() {
  tmElements_t time;

  if (RTC.read(time)) {
    
    //CALCIUM DOSE #1 - SETUP
    if(time.Second == 45) {
      lcd.clear();
      lcd.print("DOSING (CALCIUM)");
      lcd.setCursor(0,1);  
      dose(2 /*Dosage amount in ml*/);
    }
       
    else {
    digitalWrite(doser1, HIGH);
    print2digits(time.Hour);
    lcd.write(':');
    print2digits(time.Minute);
    lcd.write(':');
    print2digits(time.Second);
    lcd.setCursor(0,0);
  
  delay(1000);
}}}


void dose(int amount) {
  digitalWrite(doser1, LOW);
  lcd.setCursor(0,1);
  lcd.print(amount);
  lcd.print("ml");
  delay(amount*1000); //TIME SPENT DOSING
  digitalWrite(doser1, HIGH);
  lcd.setCursor(0,1);
  lcd.print("COMPLETE!");
  delay(6000);
  lcd.clear();   
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}