//DS= [D]ata [S]torage - data
//STCP= [ST]orage [C]lock [P]in latch
//SHCP= [SH]ift register [C]lock [P]in clock

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int JOY_X_PIN = A0;
const int JOY_Y_PIN = A1;
const int JOY_SW_PIN = 3;
const int JOY_LEFT_TRESHOLD = 400;
const int JOY_RIGHT_TRESHOLD = 600;
const int JOY_TOP_TRESHOLD = 400;
const int JOY_BOTTOM_TRESHOLD = 600;

const byte regSize = 8; // 1 byte aka 8 bits

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;
const int encodingsNumber = 16;

enum Directions {
  LEFT = 0,
  RIGHT = 1,
  UP = 2,
  DOWN = 3,
};

enum FlowStates {
  State1 = 1,
  State2 = 2,
};

const int BLINK_INTERVAL = 250;
const int DP_ENCODING = 1;

const int DEBOUNCE_TIME_MS = 400;

int byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

int displaysValues[displayCount];
  
unsigned long lastIncrement = 0;
unsigned long delayCount = 50;
unsigned long number = 0;
unsigned long blinkTimestamp = millis();
unsigned long lastDebouncedTimestamp = millis();

bool isJoystickNeutral = true;

FlowStates crtFlowState = State1;

int crtSelectedDisplay = 0;

bool isDPActive = true;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(JOY_SW_PIN, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  Serial.begin(9600);

  initDisplaysValues();

  // attachInterrupt(digitalPinToInterrupt(2), modifyDigitValue, CHANGE);
}


bool lastBlinkState = false;
bool shouldBlink = false;
bool foo = true;

// This corresponds to the rightmost digit, so
// `LEFT` means incrementing and `RIGHT` decrementing.
int displaysIdx = 0;

Directions lastJoysticDirection = -1;
int lastJoySwState = -1;

void loop() {
  // int joySwitchValue = !digitalRead(JOY_SW_PIN);
  // Serial.println(joySwitchValue);

  if (crtFlowState == State1) {
    for (int i = 0; i < displayCount; i++) {
      int value = displaysValues[i];
      writeReg(byteEncodings[value]);
      activateDisplay(i);

      if (i == crtSelectedDisplay) {
        blinkCrtSelectedDisplay();
      }
    }

    int joySwitchValue = !digitalRead(JOY_SW_PIN);
    if (joySwitchValue && joySwitchValue != lastJoySwState) {
      crtFlowState = State2;
      lastJoysticDirection = -1;
      lastJoySwState = joySwitchValue;
      isJoystickNeutral = true;

      writeReg(byteEncodings[displaysValues[crtSelectedDisplay]]);
      activateDisplay(crtSelectedDisplay);

      return;
    }

    if (!joySwitchValue) {
      lastJoySwState = 0;
    }

    int direction = getDirectionFromJoystick(false);
    if (direction == lastJoysticDirection) {
      return;
    }
    
    switch (direction) {
      case LEFT: {
        crtSelectedDisplay++;
        break;
      }

      case RIGHT: {
        crtSelectedDisplay--;
        break;
      }

      default: {
        // `UP` and `DOWN` directions or no movement at all.
        break;
      }
    }

    lastJoysticDirection = direction;
    crtSelectedDisplay = constrain(crtSelectedDisplay, 0, displayCount - 1);
  } else if (crtFlowState == State2) {
    for (int i = 0; i < displayCount; i++) {
      int value = displaysValues[i];
      writeReg(byteEncodings[value]);
      activateDisplay(i);

      if (i == crtSelectedDisplay) {
        writeReg(byteEncodings[value] | 1);
      }
    }

    int joySwitchValue = !digitalRead(JOY_SW_PIN);
    if (joySwitchValue && joySwitchValue != lastJoySwState) {
      crtFlowState = State1;
      lastJoysticDirection = -1;
      lastJoySwState = joySwitchValue;
      isJoystickNeutral = true;

      return;
    }

    if (!joySwitchValue) {
      lastJoySwState = 0;
    }

    int direction = getDirectionFromJoystick(true);
    if (direction == lastJoysticDirection) {
      return;
    } else {
      lastJoysticDirection = direction;
    }

    int crtDisplayValue = displaysValues[crtSelectedDisplay];
    
    switch (direction) {
      case UP: {
        crtDisplayValue++;
        break;
      }

      case DOWN: {
        crtDisplayValue--;
        break;
      }

      default: {
        // `UP` and `DOWN` directions or no movement at all.
        break;
      }
    }

    crtDisplayValue = constrain(crtDisplayValue, 0, 15);
    displaysValues[crtSelectedDisplay] = crtDisplayValue;

    writeReg(byteEncodings[crtDisplayValue]);
    activateDisplay(crtSelectedDisplay);
    lastDebouncedTimestamp = millis();
  }
} 

void initDisplaysValues () {
  const int DEFAULT_VALUE = 0;
  
  for (int i = 0; i < displayCount; i++) {
    displaysValues[i] = 0;
  }
}

void modifyDigitValue () {
  Serial.println("digit changed!");
}

void writeReg(int digit) {
    // ST_CP LOW to keep LEDs from changing while reading serial data
    digitalWrite(latchPin, LOW);
    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    /* ST_CP on Rising to move the data from shift register
     * to storage register, making the bits available for output.
     */
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) { 
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);
}


void writeNumber(int number) {
  int currentNumber = number; 
  int displayDigit = 0;
  int lastDigit = 0;

  while (currentNumber != 0) {
    // get the last digit of the number
    lastDigit = currentNumber % 10; // 3
    // send the number to the display
    writeReg(byteEncodings[lastDigit]);
    // enable only the display digit for that 
    activateDisplay(displayDigit);
    // increase the delay to see multiplexing in action
    delay(5);
    // increment the display digit
    displayDigit++;
    // eliminate the last digit of the number
    currentNumber /= 10;
  }
}

int getDirectionFromJoystick (bool allowContinuousOnY) {
  int joyYValue = analogRead(JOY_Y_PIN);
  int joyXValue = analogRead(JOY_X_PIN);

  if (joyXValue < JOY_LEFT_TRESHOLD && isJoystickNeutral) {
    isJoystickNeutral = false;
    return LEFT;
  }

  if (joyXValue > JOY_RIGHT_TRESHOLD && isJoystickNeutral) {
    isJoystickNeutral = false;
    return RIGHT;
  }

  if (joyYValue < JOY_TOP_TRESHOLD && (!allowContinuousOnY && isJoystickNeutral || true)) {
    isJoystickNeutral = false;
    return UP;
  }

  if (joyYValue > JOY_BOTTOM_TRESHOLD && (!allowContinuousOnY && isJoystickNeutral || true)) {
    isJoystickNeutral = false;
    return DOWN;
  }

  bool isNeutralHorizontally = JOY_LEFT_TRESHOLD <= joyXValue && joyXValue <= JOY_RIGHT_TRESHOLD;
  bool isNeutralVeritcally = JOY_TOP_TRESHOLD <= joyYValue && joyYValue <= JOY_BOTTOM_TRESHOLD;  
  if (isNeutralHorizontally && isNeutralVeritcally) {
    isJoystickNeutral = true;
    return -1;
  }

  return -1;
}

void blinkCrtSelectedDisplay () {
  unsigned long timePassed = millis() - blinkTimestamp;
  if (timePassed > BLINK_INTERVAL) {
    isDPActive = !isDPActive;
    blinkTimestamp = millis();
  }

  int value = displaysValues[crtSelectedDisplay];
  writeReg(byteEncodings[value] | (isDPActive ? 1 : 0));
}