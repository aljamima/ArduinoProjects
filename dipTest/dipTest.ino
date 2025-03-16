// Control 4 LEDs with a DIP switch
const int LED1 = 8;
const int LED2 = 9;

const int S1 = 2;
const int S2 = 3;


void switch_led(int switchPin, int ledPin) {
  int state = digitalRead(switchPin);
  //Serial.println(state);
  digitalWrite(ledPin, state == HIGH ? LOW : HIGH);
}

void setup() {
  Serial.begin(9600);   
  Serial.println("STARTING...");
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);

}

void loop() {
  switch_led(S1, LED1);
  switch_led(S2, LED2);

  int pstate = digitalRead(S1);
  Serial.print("Dip Pin1 is: " );
  Serial.println(pstate);

  delay(500);
}