const int JOY_X = A0;
const int JOY_Y = A1;
const int JOY_BTN = 12;

void setup() {
  Serial.begin(9600);
  
  // The others(X, Y) are not necessary as they implicitly are analog.
  pinMode(JOY_BTN, INPUT_PULLUP);
}

int swState;
int xValue;
int yValue;

void loop() {
  swState = digitalRead(JOY_BTN);
  xValue = analogRead(JOY_X);
  yValue = analogRead(JOY_Y);

  Serial.print("Switch:  ");
  Serial.print(swState);
  Serial.print("  |  ");
  Serial.print("X-axis: ");
  Serial.print(xValue);
  Serial.print("  |  ");
  Serial.print("Y-axis: ");
  Serial.print(yValue);
  Serial.println("  |  ");
  delay(200);
}