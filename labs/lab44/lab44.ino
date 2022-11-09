// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int JOY_X_PIN = A0;
const int JOY_Y_PIN = A1;
const int JOY_SW_PIN = 12;
const int JOY_LEFT_TRESHOLD = 400; 
const int JOY_RIGHT_TRESHOLD = 600; 

const int segSize = 8;
int index = 0;
// modify if you have common anode
bool commonAnode = false; 

byte segmentOn = HIGH;
int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

const int NR_DIGITS = 10;
const int NR_SEGMENTS = 7;

int crtDigit = 0;

bool isJoystickNeutral = true;
bool joystickSwitchState = HIGH;
bool joystickPrevSwitchState = HIGH;
bool DPState = false;

bool digitMatrix[NR_DIGITS][NR_SEGMENTS] = {
  // a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  if (commonAnode == true) {
    segmentOn = !segmentOn;
  }

  pinMode(JOY_SW_PIN, INPUT_PULLUP);
}

void loop() {
  // for (int i = 0; i < segSize; i++) {
    // digitalWrite(segments[i], segmentOn);
    // delay(300);
    // digitalWrite(segments[i], !segmentOn);
  // }

  // for (int i = 0; i < 10; i++) {
  //   setDigit(i, false);
  //   delay(300);
  // }

  int joyY = analogRead(JOY_Y_PIN);
  if (joyY < JOY_LEFT_TRESHOLD && isJoystickNeutral) {
    crtDigit = constrain(crtDigit - 1, 0, 9);
    isJoystickNeutral = false;
  }

  if (joyY > JOY_RIGHT_TRESHOLD && isJoystickNeutral) {
    crtDigit = constrain(crtDigit + 1, 0, 9);
    isJoystickNeutral = false;
  }

  // Check if joystick is in neutral position.
  if (JOY_LEFT_TRESHOLD <= joyY && joyY <= JOY_RIGHT_TRESHOLD ) {
    isJoystickNeutral = true;
  }

  joystickSwitchState = digitalRead(JOY_SW_PIN);  
  if (joystickSwitchState != joystickPrevSwitchState) {
    if (joystickSwitchState == LOW) {
      // Pressed.
      DPState = !DPState;
    }
  }

  joystickPrevSwitchState = joystickSwitchState;
  

  setDigit(crtDigit, DPState);  

  // delay(50);
}

void setDigit (byte digit, bool dpState) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i] ^ commonAnode);
  }

  digitalWrite(pinDP, dpState ^ commonAnode);
}

