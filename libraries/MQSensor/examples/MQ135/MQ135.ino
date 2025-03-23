#include <MQSensor.h>

MQSensor mq135(A2, 4.7);

void setup()
{
  Serial.begin(9600);
  Serial.println("-----------------------------------------");
  Serial.println("Calculating R0... leave the sensor in clean air!");
 
  float r0 = mq135.calculateR0(100); //Calculate 100 times.

  Serial.print("R0: ");  
  Serial.println(r0);
  Serial.println("-----------------------------------------");
}

void loop()
{
  delay(1000);
  Serial.println("\n-----------------------------------------");
  Serial.print(" V: ");  Serial.println(mq135.readVoltage());
  Serial.print(" RS: ");  Serial.println(mq135.readRS());
  Serial.print(" RS/R0: ");  Serial.println(mq135.readRSR0());  
  Serial.print(" PPM: ");  Serial.println(mq135.readPPM(3.90719312430419, -1.99513126467406));  
  Serial.println("-----------------------------------------");
}
