const int BUZZ_PIN = 5;

void setup() {
  // put your setup code here, to run once:
  tone(BUZZ_PIN, 1000);

  delay(500);

  noTone(BUZZ_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:

}
