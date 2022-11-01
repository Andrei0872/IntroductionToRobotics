const int BLUE_PIN = 11;
const int GREEN_PIN = 10;
const int RED_PIN = 9;

void setup() {
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
}

void loop() {
  analogWrite(BLUE_PIN, 80);
  analogWrite(GREEN_PIN, 55);
  analogWrite(RED_PIN, 150);
}
