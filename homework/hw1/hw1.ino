const int BLUE_INPUT_PIN = A0;
const int GREEN_INPUT_PIN = A1;
const int RED_INPUT_PIN = A2;

const int MIN_POT_VALUE = 0;
const int MAX_POT_VALUE = 1023;
const int MIN_PWM_VALUE = 0;
const int MAX_PWM_VALUE = 255;

const int BLUE_PIN = 11;
const int GREEN_PIN = 10;
const int RED_PIN = 9;

void setup() {
  Serial.begin(9600);

  pinMode(BLUE_INPUT_PIN, INPUT);
  pinMode(GREEN_INPUT_PIN, INPUT);
  pinMode(RED_INPUT_PIN, INPUT);

  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
}

void loop() {
  int bluePotValue = analogRead(BLUE_INPUT_PIN);
  int bluePWMValue = convertPotValueToPWM(bluePotValue);
  analogWrite(BLUE_PIN, bluePWMValue);

  int greenPotValue = analogRead(GREEN_INPUT_PIN);
  int greenPWMValue = convertPotValueToPWM(greenPotValue);
  analogWrite(GREEN_PIN, greenPWMValue);

  int redPotValue = analogRead(RED_INPUT_PIN);
  int redPWMValue = convertPotValueToPWM(redPotValue);
  analogWrite(RED_PIN, redPWMValue);
}

int convertPotValueToPWM (int potValue) {
  // https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
  return map(potValue, MIN_POT_VALUE, MAX_POT_VALUE, MIN_PWM_VALUE, MAX_PWM_VALUE);
}
