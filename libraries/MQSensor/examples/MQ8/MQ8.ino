#include <MQSensor.h>

MQSensor mq8(A2, 10); //The 10kOhm resistor is required by the manufacturer

void setup()
{
  Serial.begin(9600);
  Serial.println("-----------------------------------------");
  Serial.println("Calculating R0... leave the sensor in clean air!");
  
  delay(1000);

  float r0 = mq8.calculateR0(100); //Calculate 100 times.

  Serial.print("R0: ");  
  Serial.println(r0);
  Serial.println("-----------------------------------------");
}

void loop()
{
  delay(1000);
  Serial.println("\n-----------------------------------------");
  Serial.print(" V: ");  Serial.println(mq8.readVoltage());
  Serial.print(" RS: ");  Serial.println(mq8.readRS());
  Serial.print(" RS/R0: ");  Serial.println(mq8.readRSR0());  
  Serial.print(" PPM: ");  Serial.println(mq8.readPPM(1.20289448669837, -2.80130306899448));  
  Serial.println("-----------------------------------------");
}
