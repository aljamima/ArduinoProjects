#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_BME280.h>

/*
BME and RTC BOTH use same i2c pins!
VCC	5V
GND	GND
SDA	A4 (on Uno/Nano)
SCL	A5 (on Uno/Na
  
Fan-Relay	D4
Humidifier-Relay	D5
*/

RTC_DS3231 rtc;      // RTC object
Adafruit_BME280 bme; // BME280 object

void setup() {
    Serial.begin(9600);
    Wire.begin(); // Initialize I2C bus

    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time to compile time.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Initialize BME280
    if (!bme.begin(0x76)) { // Try 0x77 if 0x76 fails
        Serial.println("Could not find a valid BME280 sensor!");
        while (1);
    }
}

void loop() {
    DateTime now = rtc.now(); // Get current time

    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F; // Convert to hPa
    float humidity = bme.readHumidity();

    // Print Date and Time
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print("  |  ");

    // Print Sensor Data
    Serial.print("Temp: ");
    Serial.print(temperature, 2);
    Serial.print(" C  |  ");

    Serial.print("Pressure: ");
    Serial.print(pressure, 2);
    Serial.print(" hPa  |  ");

    Serial.print("Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");

    delay(1000);
}
