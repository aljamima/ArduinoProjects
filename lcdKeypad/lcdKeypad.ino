/*  Arduino TFT Tutorial
 *  Program made by Dejan Nedelkovski,
 *  www.HowToMechatronics.com 
 */

/*  This program uses the UTFT and URTouch libraries
 *  made by Henning Karlsen. 
 *  You can find and download them at:
 *  www.RinkyDinkElectronics.com
 */

#include <UTFT.h> 
#include <URTouch.h>

// Macro to cast string literals to char* so myGLCD.print compiles without warnings
#define PRINT(s, x, y) myGLCD.print((char*)(s), x, y)

//==== Creating Objects
UTFT    myGLCD(SSD1289, 38, 39, 40, 41); // Parameters should be adjusted to your Display/Shield model
URTouch myTouch(6, 5, 4, 3, 2);

//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

extern unsigned int bird01[0x41A];

int x, y;

char currentPage, selectedUnit;

// Ultrasonic Sensor
const int VCC = 13;
const int trigPin = 11;
const int echoPin = 12;

long duration;
int distanceInch, distanceCm;

// RGB LEDs
const int redLed = 10;
const int greenLed = 9;
const int blueLed = 8;
int xR = 38;
int xG = 38;
int xB = 38;

// Floppy Bird
int xP = 319;
int yP = 100;
int yB = 30;
int fallRateInt = 0;
float fallRate = 0;
int score = 0;
const int button = 14;
int buttonState = 0;

void setup() {
  // Initial setup
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  // Defining Pin Modes
  pinMode(VCC, OUTPUT);     // VCC
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(VCC, HIGH);  // +5V - Pin 13 as VCC

  drawHomeScreen();  // Draws the Home Screen
  currentPage = '0'; // Indicates that we are at Home Screen
  selectedUnit = '0'; // Default unit: centimeters
}

void loop() { 
  // Home Screen
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen was pressed
      y = myTouch.getY(); // Y coordinate where the screen was pressed
      // If we press the Distance Sensor Button 
      if ((x >= 35) && (x <= 285) && (y >= 90) && (y <= 130)) {
        drawFrame(35, 90, 285, 130); // Highlights the button when pressed
        currentPage = '1'; // First example
        myGLCD.clrScr(); // Clears the screen
        drawDistanceSensor(); // Draws the Distance Sensor screen
      }
      // If we press the RGB LED Control Button 
      if ((x >= 35) && (x <= 285) && (y >= 140) && (y <= 180)) {
        drawFrame(35, 140, 285, 180);
        currentPage = '2';
        myGLCD.clrScr();
        drawLedControl();
      }  
      // If we press the Birduino Game Button
      if ((x >= 35) && (x <= 285) && (y >= 190) && (y <= 230)) {
        drawFrame(35, 190, 285, 230);
        currentPage = '3';
        myGLCD.clrScr();
        myGLCD.setColor(114, 198, 206);
        myGLCD.fillRect(0, 0, 319, 239);
        drawGround();
        drawPilars(xP, yP);
        drawBird(30);
        delay(1000);
      }
    }
  }
  // Distance Sensor Example
  if (currentPage == '1') {    
      getDistance(); // Continuously updates distance reading
      if (myTouch.dataAvailable()) {
        myTouch.read();
        x = myTouch.getX();
        y = myTouch.getY();
       
        // If we press the Centimeters Button
        if ((x >= 10) && (x <= 135) && (y >= 90) && (y <= 163)) {
          selectedUnit = '0';
        }
        // If we press the Inches Button
        if ((x >= 10) && (x <= 135) && (y >= 173) && (y <= 201)) {
          selectedUnit = '1';
        }
        // If we press the Back Button
        if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0'; // Back to home screen
          myGLCD.clrScr();
          drawHomeScreen(); // Redraws home screen
        }
      }
  }
  
  // RGB LED Control 
  if (currentPage == '2') {
    setLedColor();
    if (myTouch.dataAvailable()) {
        myTouch.read();
        x = myTouch.getX();
        y = myTouch.getY();
        
        // Back button
        if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          drawHomeScreen();
          // Turn off LEDs
          analogWrite(redLed, 0);
          analogWrite(greenLed, 0);
          analogWrite(blueLed, 0);
        }
    }
  }
  
  // Birduino Game
  if (currentPage == '3') {
    xP = xP - 3;
    drawPilars(xP, yP);
    
    yB += fallRateInt;
    fallRate = fallRate + 0.4;
    fallRateInt = int(fallRate);
    if (yB >= 220) {
      yB = 220;
    }
    if (yB >= 180 || yB <= 0) {
      restartGame();
    }
    if ((xP <= 85) && (xP >= 30) && (yB <= yP - 2)) {
      restartGame();
    }
    if ((xP <= 85) && (xP >= 30) && (yB >= yP + 60)) {
      restartGame();
    }
    drawBird(yB);
    
    if (xP <= -51) {
      xP = 319;
      yP = rand() % 100 + 20;
      score++;
    }
    if (myTouch.dataAvailable()) {
        myTouch.read();
        x = myTouch.getX();
        y = myTouch.getY();
        if ((x >= 0) && (x <= 319) && (y >= 50) && (y <= 239)) {
          fallRate = -5;
        }
    }
    buttonState = digitalRead(button);
    if (buttonState == HIGH) {
      fallRate = -5;
    }
  }
  if (myTouch.dataAvailable()) {
        myTouch.read();
        x = myTouch.getX();
        y = myTouch.getY();
        if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          drawHomeScreen();
          analogWrite(redLed, 0);
          analogWrite(greenLed, 0);
          analogWrite(blueLed, 0);
        }
  }
}

// ====== Custom Functions ======

// drawHomeScreen - Custom Function
void drawHomeScreen() {
  // Title
  myGLCD.setBackColor(0, 0, 0); // Black background for text
  myGLCD.setColor(255, 255, 255); // White text color
  myGLCD.setFont(BigFont); // Big font
  PRINT("Arduino TFT Tutorial", CENTER, 10); // Title text
  myGLCD.setColor(255, 0, 0); // Red color for line
  myGLCD.drawLine(0, 32, 319, 32); // Draw red line
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont); // Small font for subtitle
  PRINT("by HowToMechatronics.com", CENTER, 41);
  myGLCD.setFont(BigFont);
  PRINT("Select Example", CENTER, 64);
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Green button
  myGLCD.fillRoundRect(35, 90, 285, 130);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(35, 90, 285, 130);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  PRINT("DISTANCE SENSOR", CENTER, 102);
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect(35, 140, 285, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(35, 140, 285, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  PRINT("RGB LED CONTROL", CENTER, 152);
  
  // Button - Birduino Game
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect(35, 190, 285, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(35, 190, 285, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  PRINT("BIRDUINO GAME", CENTER, 202);
}

// Highlights the button when pressed
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect(x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(x1, y1, x2, y2);
}

void drawDistanceSensor() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect(10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  PRINT("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  PRINT("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  PRINT("Ultrasonic Sensor", CENTER, 50);
  PRINT("HC-SR04", CENTER, 76);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0, 100, 319, 100);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  PRINT("Select Unit", 10, 114);
  myGLCD.setFont(BigFont);
  PRINT("Distance:", 130, 120);
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect(10, 135, 90, 163);
  myGLCD.setColor(225, 255, 255);
  myGLCD.drawRoundRect(10, 135, 90, 163);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  PRINT("cm", 33, 140);
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect(10, 173, 90, 201);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(10, 173, 90, 201);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  PRINT("inch", 17, 180);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  PRINT("Source code at: HowToMechatronics.com", CENTER, 220);  
}

void getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returning travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distanceCm = duration * 0.034 / 2;
  distanceInch = distanceCm / 2.53;
  // Prints the distance in centimeters
  if (selectedUnit == '0' && distanceCm <= 400) {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(distanceCm, 130, 145, 3, '0');
    myGLCD.setFont(BigFont);
    PRINT("cm  ", 235, 178);
  }
  // Prints the distance in inches
  if (selectedUnit == '1' && distanceCm <= 160) {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(distanceInch, 130, 145, 3, '0');
    myGLCD.setFont(BigFont);
    PRINT("inch", 235, 178);
  } 
  delay(10);
}

void drawLedControl() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect(10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  PRINT("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  PRINT("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  PRINT("RGB LED Control", CENTER, 50);
  PRINT("LED Color:", 10, 95);
  PRINT("R", 10, 135);
  PRINT("G", 10, 175);
  PRINT("B", 10, 215);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0, 75, 319, 75); 
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(30, 138, 310, 148); // R - Slider
  myGLCD.drawRect(30, 178, 310, 188);
  myGLCD.drawRect(30, 218, 310, 228);  
}

void setLedColor() {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();        
    // Red slider area
    if ((y >= 130) && (y <= 156)) {
      xR = x;
      if (xR <= 38) { 
        xR = 38;
      }
      if (xR >= 303) {
        xR = 303;
      }
    }
    // Green slider area
    if ((y >= 170) && (y <= 196)) {
      xG = x;
      if (xG <= 38) {
        xG = 38;
      }
      if (xG >= 303) {
        xG = 303;
      }
    }
    // Blue slider area
    if ((y >= 210) && (y <= 236)) {
      xB = x;
      if (xB <= 38) {
        xB = 38;
      }
      if (xB >= 303) {
        xB = 303;
      }
    }    
  }
  // Map slider positions to PWM values (0-255)
  int xRC = map(xR, 38, 310, 0, 255);
  int xGC = map(xG, 38, 310, 0, 255);
  int xBC = map(xB, 38, 310, 0, 255);
  
  // Send PWM signal to LED pins
  analogWrite(redLed, xRC);
  analogWrite(greenLed, xGC);
  analogWrite(blueLed, xBC);
  
  // Draw rectangle with current color
  myGLCD.setColor(xRC, xGC, xBC);
  myGLCD.fillRoundRect(175, 87, 310, 119);
  
  // Draw red slider positioner
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xR, 139, xR + 4, 147);
  myGLCD.setColor(xRC, 0, 0);
  myGLCD.fillRect(31, 139, xR - 1, 147);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(xR + 5, 139, 309, 147);
  
  // Draw green slider positioner
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xG, 179, xG + 4, 187);
  myGLCD.setColor(0, xGC, 0);
  myGLCD.fillRect(31, 179, xG - 1, 187);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(xG + 5, 179, 309, 187);
  
  // Draw blue slider positioner
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xB, 219, xB + 4, 227);
  myGLCD.setColor(0, 0, xBC);
  myGLCD.fillRect(31, 219, xB - 1, 227);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(xB + 5, 219, 309, 227);
}

void drawGround() {
  myGLCD.setColor(221, 216, 148);
  myGLCD.fillRect(0, 215, 319, 239);
  myGLCD.setColor(47, 175, 68);
  myGLCD.fillRect(0, 205, 319, 214);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(221, 216, 148);
  myGLCD.setFont(BigFont);
  PRINT("Score:", 5, 220);
  myGLCD.setFont(SmallFont);
  PRINT("HowToMechatronics.com", 140, 220); 
}

void drawPilars(int x, int y) {
  if (x >= 270) {
      myGLCD.setColor(0, 200, 20);
      myGLCD.fillRect(318, 0, x, y - 1);
      myGLCD.setColor(0, 0, 0);
      myGLCD.drawRect(319, 0, x - 1, y);

      myGLCD.setColor(0, 200, 20);
      myGLCD.fillRect(318, y + 81, x, 203);
      myGLCD.setColor(0, 0, 0);
      myGLCD.drawRect(319, y + 80, x - 1, 204); 
  } else if (x <= 268) {
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x + 51, 0, x + 53, y);
    myGLCD.setColor(0, 200, 20);
    myGLCD.fillRect(x + 49, 1, x + 1, y - 1);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(x + 50, 0, x, y);
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x - 1, 0, x - 3, y);

    myGLCD.fillRect(x + 51, y + 80, x + 53, 204);
    myGLCD.setColor(0, 200, 20);
    myGLCD.fillRect(x + 49, y + 81, x + 1, 203);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(x + 50, y + 80, x, 204);
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x - 1, y + 80, x - 3, 204);
  }
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(221, 216, 148);
  myGLCD.setFont(BigFont);
  myGLCD.printNumI(score, 100, 220);
}

void drawBird(int y) {
  if (y <= 219) {
    myGLCD.drawBitmap(50, y, 35, 30, bird01);
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRoundRect(50, y, 85, y - 6);
    myGLCD.fillRoundRect(50, y + 30, 85, y + 36);
  } else if (y >= 200) {
    myGLCD.drawBitmap(50, 200, 35, 30, bird01);
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRoundRect(50, 200, 85, 200 - 6);
    myGLCD.fillRoundRect(50, 200 + 30, 85, 200 + 36);
  }
}

void gameOver() {
  myGLCD.clrScr();
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  PRINT("GAME OVER", CENTER, 40);
  PRINT("Score:", 100, 80);
  myGLCD.printNumI(score, 200, 80, 3, '0');
  PRINT("Restarting...", CENTER, 120);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.printNumI(2, CENTER, 150, 1, '0');
  delay(1000);
  myGLCD.printNumI(1, CENTER, 150, 1, '0');
  delay(1000);
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRect(0, 0, 319, 239);
  drawBird(30);
  drawGround();
  delay(1000);
}

void restartGame() {
  delay(1000);
  gameOver();
  xP = 319;
  yB = 30;
  fallRate = 0;
  score = 0;
}
