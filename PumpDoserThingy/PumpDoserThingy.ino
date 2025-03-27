#include <Wire.h>
#include <U8g2lib.h>
#include <Encoder.h>

// OLED I2C (SH1106)
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// Rotary encoder pins
Encoder encoder(2, 3);  // CLK = D2, DT = D3

#define ENCODER_BTN 4
#define CONFIRM_BTN 5
#define BACK_BTN 6

#define RELAY_PINK   9
#define RELAY_PURPLE 8
#define RELAY_GREEN  7

// Menu state
const char* modeItems[] = {"AllPurpose", "Grow", "Bloom", "Test Pumps"};
const int modeCount = sizeof(modeItems) / sizeof(modeItems[0]);
int modeIndex = 0;
int gallonAmount = 1;

enum ScreenState {
  MODE_SELECT,
  GALLON_SELECT,
  CONFIRM_SCREEN
};
ScreenState screenState = MODE_SELECT;

long lastEncoderPos = 0;

struct Dosing {
  int pink;
  int purple;
  int green;
};

Dosing dosingProfiles[] = {
  {5, 5, 5},    // AllPurpose
  {5, 10, 15},  // Grow
  {15, 10, 5}   // Bloom
};

void setup() {
  Serial.begin(115200);

  display.begin();
  display.setFont(u8g2_font_6x10_tr);

  pinMode(ENCODER_BTN, INPUT_PULLUP);
  pinMode(CONFIRM_BTN, INPUT_PULLUP);
  pinMode(BACK_BTN, INPUT_PULLUP);

  pinMode(RELAY_PINK, OUTPUT);
  pinMode(RELAY_PURPLE, OUTPUT);
  pinMode(RELAY_GREEN, OUTPUT);

  digitalWrite(RELAY_PINK, LOW);
  digitalWrite(RELAY_PURPLE, LOW);
  digitalWrite(RELAY_GREEN, LOW);

  encoder.write(0); // Reset position
  lastEncoderPos = 0;
  updateDisplay();
}

void loop() {
  long newPos = encoder.read() / 4;
  int delta = newPos - lastEncoderPos;

  if (delta != 0) {
    lastEncoderPos = newPos;

    if (screenState == MODE_SELECT) {
      modeIndex -= delta;
      if (modeIndex < 0) modeIndex = modeCount - 1;
      if (modeIndex >= modeCount) modeIndex = 0;

    } else if (screenState == GALLON_SELECT) {
      gallonAmount -= delta;
      if (gallonAmount < 1) gallonAmount = 100;
      if (gallonAmount > 100) gallonAmount = 1;
    }

    updateDisplay();
  }

  if (digitalRead(ENCODER_BTN) == LOW || digitalRead(CONFIRM_BTN) == LOW) {
    delay(150);

    if (screenState == MODE_SELECT) {
      screenState = GALLON_SELECT;
      encoder.write(0);
      lastEncoderPos = 0;
    } 
    else if (screenState == GALLON_SELECT) {
      screenState = CONFIRM_SCREEN;
      encoder.write(0);
      lastEncoderPos = 0;
    } 
    else if (screenState == CONFIRM_SCREEN) {
      if (modeIndex == 3) {
        Serial.println(">> TESTING PUMPS <<");
        testPumps();
      } else {
        Serial.println(">> DISPENSING STARTED <<");
        runDosing(modeIndex, gallonAmount);
      }
    }
    updateDisplay();
  }

  if (digitalRead(BACK_BTN) == LOW) {
    delay(150);
    if (screenState == GALLON_SELECT) {
      screenState = MODE_SELECT;
    } else if (screenState == CONFIRM_SCREEN) {
      screenState = GALLON_SELECT;
    }
    encoder.write(0);
    lastEncoderPos = 0;
    updateDisplay();
  }

  delay(10);
}

void runDosing(int modeIndex, int gallons) {
  Dosing d = dosingProfiles[modeIndex];

  unsigned long pinkTime = d.pink * gallons * 1000UL;
  unsigned long purpleTime = d.purple * gallons * 1000UL;
  unsigned long greenTime = d.green * gallons * 1000UL;

  Serial.println("Starting dosing...");

  unsigned long start = millis();
  digitalWrite(RELAY_PINK, HIGH);
  while (millis() - start < pinkTime) delay(1);
  digitalWrite(RELAY_PINK, LOW);

  start = millis();
  digitalWrite(RELAY_PURPLE, HIGH);
  while (millis() - start < purpleTime) delay(1);
  digitalWrite(RELAY_PURPLE, LOW);

  start = millis();
  digitalWrite(RELAY_GREEN, HIGH);
  while (millis() - start < greenTime) delay(1);
  digitalWrite(RELAY_GREEN, LOW);

  Serial.println("Dosing complete.");
}

void testPumps() {
  const int delayTime = 2000;

  Serial.println("Testing Pink Pump...");
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    display.drawStr(0, 12, "Testing Pink Pump...");
  } while (display.nextPage());
  digitalWrite(RELAY_PINK, HIGH);
  delay(delayTime);
  digitalWrite(RELAY_PINK, LOW);

  Serial.println("Testing Purple Pump...");
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    display.drawStr(0, 12, "Testing Purple Pump...");
  } while (display.nextPage());
  digitalWrite(RELAY_PURPLE, HIGH);
  delay(delayTime);
  digitalWrite(RELAY_PURPLE, LOW);

  Serial.println("Testing Green Pump...");
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    display.drawStr(0, 12, "Testing Green Pump...");
  } while (display.nextPage());
  digitalWrite(RELAY_GREEN, HIGH);
  delay(delayTime);
  digitalWrite(RELAY_GREEN, LOW);

  Serial.println("Test complete.");
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    display.drawStr(0, 12, "Test Complete.");
  } while (display.nextPage());
  delay(1500);
  screenState = MODE_SELECT;
  encoder.write(0);
  lastEncoderPos = 0;
  updateDisplay();
}

void updateDisplay() {
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    if (screenState == MODE_SELECT) {
      display.drawStr(0, 12, "Select Mode:");
      for (int i = 0; i < modeCount; i++) {
        char line[32];
        sprintf(line, "%c %s", (i == modeIndex ? '>' : ' '), modeItems[i]);
        display.drawStr(0, 24 + i * 10, line);
      }

    } else if (screenState == GALLON_SELECT) {
      display.drawStr(0, 12, "How many gallons?");
      char g[10];
      sprintf(g, "%d", gallonAmount);
      display.drawStr(30, 32, g);

    } else if (screenState == CONFIRM_SCREEN) {
      char line1[32], line2[32];
      sprintf(line1, "Mode: %s", modeItems[modeIndex]);
      sprintf(line2, "Gallons: %d", gallonAmount);
      display.drawStr(0, 12, "Confirm:");
      display.drawStr(0, 24, line1);
      display.drawStr(0, 36, line2);
      display.drawStr(0, 54, "Press confirm to run");
    }
  } while (display.nextPage());
}