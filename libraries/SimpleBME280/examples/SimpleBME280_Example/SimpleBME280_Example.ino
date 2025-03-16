#include "SimpleBME280.h"

#define PLOTTER_ENABLE 01


/*******************************************************************************/

uint32_t timestamp;

void timeMeasureStart() {
  timestamp = micros();
}

void timeMeasureStop() {
  timestamp = micros() - timestamp;

  Serial.print(F(" [wait time = "));
  Serial.print(1e-3 * timestamp, 3);
  Serial.print(F(" ms]"));
}

uint32_t timeMeasureValue() {
  return timestamp;
}

/*******************************************************************************/


SimpleBME280 bme280;
float pbase, t, p, h;


void setup() {
  Serial.begin(115200);

#if (PLOTTER_ENABLE == 0)
  Serial.print(F("BME280 Test.\n"));

  if (bme280.begin() == 0)
    Serial.print(F("BME280 initialized.\n"));
#else
  bme280.begin();
  delay(100);
  Serial.println(F("t,p,h"));
#endif

  //  bme280.mode(bmemNormal);
  //  bme280.period(bmep250ms);
  //  bme280.filter(bmef8);
  //
  //  bme280.oversampling(bmeo2, 't');
  //  bme280.oversamplingT(bmeo2);
  //  bme280.oversamplingP(bmeo16);
  //  bme280.oversamplingH(bmeo4);

  bme280.update();
  pbase = bme280.getP();
}

void loop() {
#if (PLOTTER_ENABLE == 0)
  textDbg();
#else
  plotterDbg();
#endif
}

void textDbg() {
  Serial.print(F("Get data "));
  timeMeasureStart();

  bme280.update();
  t = bme280.getT();
  p = bme280.getP();
  h = bme280.getH();

  timeMeasureStop();
  Serial.println();

  Serial.print(F("  T = "));  Serial.print(t, 2);
  Serial.print(F("  P = "));  Serial.print(p, 2);
  Serial.print(F("  H = "));  Serial.print(h, 2);
  Serial.print(F("\n\n"));
  delay(1000);
}

void plotterDbg() {
  bme280.update();
  t = bme280.getT();
  p = bme280.getP();
  h = bme280.getH();

  static float f = p;
  f = p = p * 0.1 + f * 0.9; // доп. фильтрация
  float pd = p - pbase + 50;

  delay(10);  Serial.print(t,  2);   Serial.print(',');
  delay(10);  Serial.print(pd, 2);   Serial.print(',');
  delay(10);  Serial.print(h,  2);
  Serial.println();

  delay(10);  Serial.print(F("   T="));  Serial.print(t, 2);
  delay(10);  Serial.print(F("   P="));  Serial.print((uint32_t)p);
  delay(10);  Serial.print(F("   H="));  Serial.print(h, 2);
  Serial.println();
  delay(1000);
}
