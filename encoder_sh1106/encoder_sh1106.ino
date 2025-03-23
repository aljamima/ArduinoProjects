#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED I2C
Adafruit_SH1106G display(128, 64, &Wire, -1);

// Encoder & Buttons
#define ENCODER_CLK 2
#define ENCODER_DT 3
#define ENCODER_BTN 4
#define CONFIRM_BTN 5
#define BACK_BTN 6

// Encoder handling
volatile int encoderDelta = 0;
int lastClkState = HIGH;
unsigned long lastEncoderTime = 0;

// Menu state
const char* modeItems[] = {"AllPurpose", "Grow", "Bloom"};
const int modeCount = sizeof(modeItems) / sizeof(modeItems[0]);
int modeIndex = 0;
int gallonAmount = 1;

enum ScreenState {
  MODE_SELECT,
  GALLON_SELECT,
  CONFIRM_SCREEN
};
ScreenState screenState = MODE_SELECT;

void setup() {
  Serial.begin(115200);

  if (!display.begin()) {
    Serial.println("Display failed");
    while (1);
  }

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  updateDisplay();

  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  pinMode(CONFIRM_BTN, INPUT_PULLUP);
  pinMode(BACK_BTN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder, CHANGE);
}

void loop() {
  static int lastModeIndex = -1;
  static int lastGallon = -1;
  bool screenChanged = false;

  // Handle encoder movement
  if (encoderDelta != 0) {
    if (screenState == MODE_SELECT) {
      modeIndex += encoderDelta;
      if (modeIndex >= modeCount) modeIndex = 0;
      if (modeIndex < 0) modeIndex = modeCount - 1;
      screenChanged = true;
    } else if (screenState == GALLON_SELECT) {
      gallonAmount += encoderDelta;
      if (gallonAmount > 10) gallonAmount = 1;
      if (gallonAmount < 1) gallonAmount = 50;
      screenChanged = true;
    }
    encoderDelta = 0;
  }

  // Confirm (rotary button or confirm button)
  if (digitalRead(ENCODER_BTN) == LOW || digitalRead(CONFIRM_BTN) == LOW) {
    delay(150);  // debounce
    if (screenState == MODE_SELECT) {
      screenState = GALLON_SELECT;
    } else if (screenState == GALLON_SELECT) {
      screenState = CONFIRM_SCREEN;
    } else if (screenState == CONFIRM_SCREEN) {
      Serial.println(">> DISPENSING STARTED <<");
      // Call your dispensing logic here
    }
    screenChanged = true;
  }

  // Back button
  if (digitalRead(BACK_BTN) == LOW) {
    delay(150);  // debounce
    if (screenState == GALLON_SELECT) {
      screenState = MODE_SELECT;
    } else if (screenState == CONFIRM_SCREEN) {
      screenState = GALLON_SELECT;
    }
    screenChanged = true;
  }

  if (screenChanged || lastModeIndex != modeIndex || lastGallon != gallonAmount) {
    updateDisplay();
    lastModeIndex = modeIndex;
    lastGallon = gallonAmount;
  }

  delay(10);
}

void readEncoder() {
  if (millis() - lastEncoderTime < 100) return; // debounce
  lastEncoderTime = millis();

  int clk = digitalRead(ENCODER_CLK);
  int dt = digitalRead(ENCODER_DT);

  if (clk != lastClkState) {
    encoderDelta = (dt != clk) ? 1 : -1;
  }
  lastClkState = clk;
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);

  if (screenState == MODE_SELECT) {
    display.println("Select Mode:");
    for (int i = 0; i < modeCount; i++) {
      display.setCursor(0, 12 + i * 10);
      display.print((i == modeIndex) ? "> " : "  ");
      display.println(modeItems[i]);
    }
  } else if (screenState == GALLON_SELECT) {
    display.println("How many gallons?");
    display.setTextSize(2);
    display.setCursor(40, 30);
    display.println(String(gallonAmount));
    display.setTextSize(1);
  } else if (screenState == CONFIRM_SCREEN) {
    display.println("Confirm:");
    display.setCursor(0, 15);
    display.println("> Mode: " + String(modeItems[modeIndex]));
    display.setCursor(0, 30);
    display.println("> Gallons: " + String(gallonAmount));
    display.setCursor(0, 50);
    display.println("Press confirm to run");
  }

  display.display();
}
