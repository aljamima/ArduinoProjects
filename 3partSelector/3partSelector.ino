#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Display setup (SH1106 OLED, I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Relay pins for pumps
const int relayPink = 4;
const int relayPurple = 5;
const int relayGreen = 6;

// Buttons
const int modeButton = 7;
const int startButton = 8;

// Mode selection
enum Mode { ALL_PURPOSE, GROW, BLOOM };
Mode currentMode = ALL_PURPOSE;

// Dosing times (milliseconds)
struct DosingProfile {
    int pinkTime;
    int purpleTime;
    int greenTime;
};

// Define preset dosing times
DosingProfile allPurpose = { 5000, 5000, 5000 };
DosingProfile grow = { 5000, 10000, 15000 };
DosingProfile bloom = { 15000, 10000, 5000 };

// Active dosing profile
DosingProfile selectedProfile;

void setup() {
    Serial.begin(9600);

    // Initialize OLED display
    display.begin(0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);

    // Set relay pins as outputs
    pinMode(relayPink, OUTPUT);
    pinMode(relayPurple, OUTPUT);
    pinMode(relayGreen, OUTPUT);
    
    // Ensure all pumps are off
    digitalWrite(relayPink, LOW);
    digitalWrite(relayPurple, LOW);
    digitalWrite(relayGreen, LOW);

    // Button inputs with pull-ups
    pinMode(modeButton, INPUT_PULLUP);
    pinMode(startButton, INPUT_PULLUP);

    // Set initial mode
    updateMode();
    displayMode();
}

void loop() {
    // Check if mode button is pressed (debounced)
    if (digitalRead(modeButton) == LOW) {
        delay(200); // Debounce delay
        currentMode = static_cast<Mode>((currentMode + 1) % 3);
        updateMode();
        displayMode();
        while (digitalRead(modeButton) == LOW); // Wait for button release
    }

    // Check if start button is pressed
    if (digitalRead(startButton) == LOW) {
        delay(200); // Debounce delay
        dispenseNutrients();
        displayMode();
        while (digitalRead(startButton) == LOW); // Wait for button release
    }
}

// Update selected profile based on mode
void updateMode() {
    switch (currentMode) {
        case ALL_PURPOSE: selectedProfile = allPurpose; break;
        case GROW: selectedProfile = grow; break;
        case BLOOM: selectedProfile = bloom; break;
    }
}

// Display current mode on OLED
void displayMode() {
    display.clearDisplay();
    display.setCursor(10, 10);

    display.println("Mode: ");
    switch (currentMode) {
        case ALL_PURPOSE: display.println("All Purpose"); break;
        case GROW: display.println("Grow Mode"); break;
        case BLOOM: display.println("Bloom Mode"); break;
    }

    display.println("Pink: " + String(selectedProfile.pinkTime / 1000) + "s");
    display.println("Purple: " + String(selectedProfile.purpleTime / 1000) + "s");
    display.println("Green: " + String(selectedProfile.greenTime / 1000) + "s");

    display.display();
}

// Dispense nutrients based on selected mode
void dispenseNutrients() {
    Serial.println("Dispensing Nutrients...");

    digitalWrite(relayPink, HIGH);
    delay(selectedProfile.pinkTime);
    digitalWrite(relayPink, LOW);

    digitalWrite(relayPurple, HIGH);
    delay(selectedProfile.purpleTime);
    digitalWrite(relayPurple, LOW);

    digitalWrite(relayGreen, HIGH);
    delay(selectedProfile.greenTime);
    digitalWrite(relayGreen, LOW);

    Serial.println("Dosing Complete.");
}
