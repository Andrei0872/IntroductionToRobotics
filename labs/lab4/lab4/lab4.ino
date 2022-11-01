const int BUTTON_PIN = 5;
const int LED_PIN = 6;

int buttonPressCount = 0;
byte lastButtonState = LOW;

// long: [-2mil, 2mil]; int[-32000, 32000]
unsigned long lastReading;
// The amount of time we want the button to be pressed.
unsigned int debounceTime = 50;

void setup() {
  Serial.begin(9600);

  // pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // PULLDOWN
  // byte buttonState = digitalRead(BUTTON_PIN);
  // Serial.println(buttonState);
  // digitalWrite(LED_PIN, buttonState);
  // delay(1);

  
  // PULLUP
  // byte buttonState = digitalRead(BUTTON_PIN);
  // byte isButtonOn = !buttonState;
  // Serial.println(isButtonOn);
  // digitalWrite(LED_PIN, isButtonOn);
  // delay(1);

  // INPUT_PULLUP
  byte buttonState = digitalRead(BUTTON_PIN);
  byte isButtonOn = !buttonState;
  
  // if (buttonState != lastButtonState) {
  //   // Fie a fost apasat, fie a fost ridicat.
  //   // Just pressed the button.
  //   if (isButtonOn) {
  //     buttonPressCount++;
  //   } else {
  //     // Just released the button.
  //   }

  //   lastButtonState = buttonState;
  // }

  if (buttonState != lastButtonState) {
    // The state has changed.

    lastButtonState = buttonState;
    lastReading = millis();
  }
  
  

  // Starea s a pastrat in ultimele 50ms(`debounceTime`).
  if (millis() - lastButtonState >= debounceTime) {
    // Just pressed.
    if (buttonState == LOW) {
      buttonPressCount++;
    } else {
      // Just released.
    }
  }
  digitalWrite(LED_PIN, !isButtonOn);


  digitalWrite(LED_PIN, isButtonOn);
  Serial.println(buttonPressCount);
  delay(1);
}
