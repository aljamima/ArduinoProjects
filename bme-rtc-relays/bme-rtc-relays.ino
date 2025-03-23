#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_BME280.h>


/*

also get Co2 sensor just to chart the co2 buildup.
host long does it take, how high does it get?
how effective is air exchange? maybe run fan til co2 level drops to normal/baseline.

BME and RTC BOTH use same i2c pins!
VCC	5V
GND	GND
SDA	A4 (on Uno/Nano)
SCL	A5 (on Uno/Na
  
Fan-Relay	D4
Humidifier-Relay	D5
*/

RTC_DS3231 rtc;
Adafruit_BME280 bme;

// Relay pins
const int fanRelay = 4;
const int humidifierRelay = 5;

// Target humidity
const float targetHumidity = 90.0;

void setup() {
    Serial.begin(9600);
    Wire.begin();

    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting default time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Initialize BME280
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor!");
        while (1);
    }

    // Set relay pins as outputs and turn them off initially
    pinMode(fanRelay, OUTPUT);
    pinMode(humidifierRelay, OUTPUT);
    digitalWrite(fanRelay, LOW);
    digitalWrite(humidifierRelay, LOW);
}

void loop() {
    DateTime now = rtc.now();

    // Check if it's 12:00 or 00:00 (midnight)
    if ((now.hour() == 12 || now.hour() == 0) && now.minute() == 0) {
        runFanAndHumidifier(); // Start the fan/humidifier cycle
    }

    delay(5000); // Check every 5 seconds (adjust as needed)
}

void runFanAndHumidifier() {
    Serial.println("Turning on fan...");
    digitalWrite(fanRelay, HIGH);
    delay(60000); // Run fan for 60 seconds
    digitalWrite(fanRelay, LOW);
    Serial.println("Fan off. Checking humidity...");

    Serial.println("Turning on humidifier...");
    digitalWrite(humidifierRelay, HIGH);

    // Wait until humidity reaches 90%
    while (bme.readHumidity() < targetHumidity) {
        Serial.print("Current Humidity: ");
        Serial.print(bme.readHumidity());
        Serial.println("% - Humidifier ON");
        delay(5000); // Check every 5 seconds
    }

    Serial.println("Humidity reached 90%. Turning off humidifier...");
    digitalWrite(humidifierRelay, LOW);
}
