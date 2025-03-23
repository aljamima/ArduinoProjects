/*
Fresh air and RH relay cycling for mushroom fruiting chamber 20/1/2020
A basic program to run two different relays with a delay in between, the fan relay with a set run time, the RH relay ON time dependent on potentiometer postion and temperature.
  Programmer:  Arduino as ISP, Board:  Arduino Uno (Actually a Nano, depends on firmware on nano board as to what it needs to be programmed as)
  
  Uses a potentiometer to change percent RH time bias along with Temperature change using DS18B20

  DS18B20 Temperature sensor with digital signal input into pin4
  Potentiometer analogue signal into pin A0
  Fan relay as Output on pin D2
  RH relay as Output on pin D3
  

*/


#include <OneWire.h>              //library dependencies are a pain, you have to install these before trying to compiling and uploading the code
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4  //sensor data pin 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int Tsignal = 4;                              //This is the DS180BS20 Temperature signal input pin on the Arduino
float Tx;                                       //This is the temperature bias factor for percent humidify 0.05-1.0

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 2&3 as outputs, Tsignal (pin4) as input
  pinMode(Tsignal, INPUT);                     //Sets the T pin as an input
  pinMode(2, OUTPUT);       //D2 fan
  pinMode(3, OUTPUT);       //D3 RH
  Serial.begin(9600);                          //Start Serial
  sensors.begin();    //DS28B20 sensor initiate
}

// the loop function runs over and over again forever
void loop() {

sensors.requestTemperatures(); 
float Tpv = sensors.getTempCByIndex(1);                                    // measure T

Serial.println("freshair & delay & RH ver1.5");

Serial.print("Tpv =  ");
Serial.print(Tpv,1);                        //Print the temperature value to Serial

int potValue = analogRead(A0);  //get the pot position to determine humidity percent run time
Serial.print("  pot =  ");
Serial.print(potValue);                        //Print the potValue% value to Serial

if (Tpv < 16)                 // Tx is temperature bias, so if Tpv is less than 16, Tx =0.05 5%
  {Tx = 0.05;}
  else if (Tpv>21)            // if Tpv is greater than 21, Tx=100%
  {Tx = 1.0;}
  else
  {Tx = 1-(21-Tpv)*0.2;}       // Tpv between 16 and 21deg gives a Tx changing by 20% each degree C

Serial.print("  Tx =  ");
Serial.println(Tx);                        //Print the Tx value to Serial
  



//  CHANGE "cycle_time", and "percent_fan" below to alter the pulse period and ON width //
float cycle_time = 300;   //cycle time in seconds (300s)
float percent_fan = 20;  //percentage on for fan (20)

float percent_humidify = Tx*potValue*(75/1023.0);  //maximum of 75% run time, fan is 20%, want minimum of 5% nothing
float percent_none = 100 - percent_fan - percent_humidify;  //percentage nothing (calculated)

//working
float cycle_ms = cycle_time*1000;               // convert s to ms
float fanON_time = cycle_ms*percent_fan*0.01;
float none_time = cycle_ms*percent_none*0.01;
float RH_time = cycle_ms*percent_humidify*0.01;
  
  
  Serial.print("cycle time(s)  :");           //debug serial printing
  Serial.print(cycle_time);
  Serial.print("   RH run %  :");
  Serial.print(percent_humidify);
  Serial.print("   fan run %  :");
  Serial.print(percent_fan);
  Serial.print("  idle %  :");
  Serial.println(percent_none);
  
  Serial.print("fanON  ");
  Serial.println(fanON_time/1000);
  digitalWrite(2, LOW);   // FAN ON.  For 4-ch relay LOW is ON, HIGH is OFF on NO connection to make LED indicate correctly
   digitalWrite(3, HIGH);   // 
  delay(fanON_time);                       // wait fanON Time with FAN ON
//
Serial.print("nothing  ");
   Serial.println(none_time/4000 );
  digitalWrite(2, HIGH);    //              // FAN OFF
  digitalWrite(3, HIGH);    //              // RH OFF
  delay(none_time/4);                       // wait none (delay) time/4

  Serial.print("RH ON  ");
   Serial.println(RH_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, LOW);                   // RH ON
  delay(RH_time/4);                       // wait RH time/4
//
Serial.print("nothing  ");
   Serial.println(none_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, HIGH);    // 
  delay(none_time/4);                       // wait none time/4

  Serial.print("RH ON  ");
   Serial.println(RH_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, LOW);    // 
  delay(RH_time/4);                       // wait RH time/4
//
Serial.print("nothing  ");
   Serial.println(none_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, HIGH);    // 
  delay(none_time/4);                       // wait  time/4

  Serial.print("RH ON  ");
   Serial.println(RH_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, LOW);    // 
  delay(RH_time/4);                       // wait OFF time/4
//
Serial.print("nothing  ");
   Serial.println(none_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, HIGH);    // 
  delay(none_time/4);                       // wait  time/4

  Serial.print("RH ON  ");
   Serial.println(RH_time/4000);
  digitalWrite(2, HIGH);    // 
  digitalWrite(3, LOW);    // 
  delay(RH_time/4);                       // wait OFF time/4

  

  
 
}
