#include <MQSensor.h>

MQSensor mq135(A2, 4.7);
MQSensor mq8(A1, 10); 

void setup()
{
  Serial.begin(9600);
  mq135.calculateR0(100); //Calculate 100 times.
  mq8.calculateR0(100); //Calculate 100 times.
}

void loop()
{
  delay(1000);
  Serial.println("\n-----------------------------------------");
  Serial.print("MQ8 RS/R0: ");  Serial.println(mq8.readRSR0());  
  Serial.print("MQ135 RS/R0: ");  Serial.println(mq135.readRSR0());    
  Serial.println("-----------------------------------------");
}
