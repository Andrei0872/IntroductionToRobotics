const int LED_PIN = 11;
const int POT_PIN = A0;

const int BLUE_PIN = 9;
const int GREEN_PIN = 10;
const int RED_PIN = 11;

int currentBrightness = 0;
int deltaBrightness = 10;

void setup() {
  // Vrem sa scriem in pinul de ouput
  // pinMode(LED_PIN, OUTPUT);

  // Serial.begin(9600);
  // pinMode(POT_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
}

void loop() {
  // Trimitem curent la PIN ul redului
  // Scrie o anumita valoare la un PIN.

  // HOMEWORK: Cate secunde au trecut de la pornirea MS;
  // int time = millis();
  
  // DIGITAL
  // digitalWrite(LED_PIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_PIN, LOW);
  // delay(1000);

  // analogWrite(LED_PIN, 20);

  // analogWrite(LED_PIN, currentBrightness);

  // currentBrightness += deltaBrightness;
  // currentBrightness = constrain(currentBrightness, 0, 255);
  // if (currentBrightness >= 255 || currentBrightness <= 0) {
  //   deltaBrightness = -deltaBrightness;
  // }

  // delay(40);

  // int potValue = analogRead(POT_PIN);
  // Serial.println(potValue);
  // delay(1);

  /* 
  if (anodComun) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;      
  }
  
   */

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 0);
}

// Aprindem LED-ul
// Il tinem aprins o secunda
// Stingem LED-ul
// Il tinem stins o secunda

// HOMEWORK: set led
