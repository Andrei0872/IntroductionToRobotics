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
const int JOY_TOP_TRESHOLD = 400;
const int JOY_BOTTOM_TRESHOLD = 600;

const int SEGMENT_LEGTH = 8;
bool commonAnode = false; 

byte segmentOn = HIGH;
int segments[SEGMENT_LEGTH] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

const int BLINK_INTERVAL = 250;
const int DIRECTIONS_COUNT = 4;

const int DEBOUNCE_TIME_MS = 400;

const int LONG_PRESS_DURATION_MS = 3000;

enum FlowStates {
  State1 = 1,
  State2 = 2,
};

enum Directions {
  LEFT = 0,
  RIGHT = 1,
  UP = 2,
  DOWN = 3,
};

unsigned long blinkTimestamp = millis();
bool isLEDActive = false;

int crtSelectedSegment = pinDP;
int previousSelectedSegment = crtSelectedSegment;
unsigned long lastDebouncedTimestamp = millis();

int DIRECTION_MATRIX[SEGMENT_LEGTH][DIRECTIONS_COUNT];
bool activeSegments[SEGMENT_LEGTH];

bool isJoystickNeutral = true;
bool prevJoystickNeutralValue = true;

int currentJoystickSwitchState = LOW;

FlowStates crtFlowState = State1;

unsigned long longPressTimestamp = millis();

int lastJoySwState = 0;

void setup() {
  for (int i = 0; i < SEGMENT_LEGTH; i++) {
    pinMode(segments[i], OUTPUT);
  }

  if (commonAnode == true) {
    segmentOn = !segmentOn;
  }

  pinMode(JOY_SW_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  initializeDirectionMatrix();
}

void loop() {
  if (crtFlowState == State1) {
    showActiveSegments();
    blinkCrtSelectedSegment();

    int joySwitchValue = !digitalRead(JOY_SW_PIN);
    if (joySwitchValue && lastJoySwState != joySwitchValue) {
      crtFlowState = State2;
      isJoystickNeutral = true;

      lastJoySwState = joySwitchValue;

      longPressTimestamp = millis();
      return;
    }

    if (!joySwitchValue) {
      lastJoySwState = 0;
    }

    // Adding debounce so that certain segments are not 'skipped'.
    unsigned long passedDebouncedTime = millis() - lastDebouncedTimestamp;
    if (passedDebouncedTime <= DEBOUNCE_TIME_MS) {
      return;
    }

    int nextDirection = getDirectionFromJoystick();
    if (nextDirection == -1) {
      return;
    }

    int segmentToBeSelected = getNextSegment(nextDirection);
    bool isValidMove = !!segmentToBeSelected;
    if (!isValidMove) {
      return;
    }

    digitalWrite(crtSelectedSegment, 0);
    crtSelectedSegment = segmentToBeSelected;
    lastDebouncedTimestamp = millis();
  } else if (crtFlowState == State2) {

    int joySwitchValue = !digitalRead(JOY_SW_PIN);
    if (joySwitchValue && lastJoySwState != joySwitchValue) {
      crtFlowState = State1;
      prevJoystickNeutralValue = isJoystickNeutral = true;
      lastJoySwState = joySwitchValue;

      return;
    }

    if (!joySwitchValue) {
      lastJoySwState = 0;
    } else {

      unsigned long joyBtnPressDuration = millis() - longPressTimestamp;
      if (joyBtnPressDuration > LONG_PRESS_DURATION_MS) {
        resetActiveSegments();
        crtSelectedSegment = pinDP;

        crtFlowState = State1;
        prevJoystickNeutralValue = isJoystickNeutral = true;
        return;
      }
    }

    int mappedPin = getMappedSegmentPin(crtSelectedSegment);
    int isSegmentActive = activeSegments[mappedPin];
    digitalWrite(crtSelectedSegment, isSegmentActive);

    int shouldToggleValue = getToggledValueFromJoystick();
    if (!shouldToggleValue) {
      return;
    }

    activeSegments[mappedPin] = !activeSegments[mappedPin];    
  }
}

void blinkCrtSelectedSegment () {
  unsigned long timePassed = millis() - blinkTimestamp;
  if (timePassed > BLINK_INTERVAL) {
    isLEDActive = !isLEDActive;
    blinkTimestamp = millis();
  }

  digitalWrite(crtSelectedSegment, isLEDActive);
}

void initializeDirectionMatrix () {
  // Trying to replicate the directions table from the HW document.

  int pinAMapped = getMappedSegmentPin(pinA);
  DIRECTION_MATRIX[pinAMapped][DOWN] = pinG;
  DIRECTION_MATRIX[pinAMapped][LEFT] = pinF;
  DIRECTION_MATRIX[pinAMapped][RIGHT] = pinB;

  int pinBMapped = getMappedSegmentPin(pinB);
  DIRECTION_MATRIX[pinBMapped][UP] = pinA;
  DIRECTION_MATRIX[pinBMapped][DOWN] = pinG;
  DIRECTION_MATRIX[pinBMapped][LEFT] = pinF;

  int pinCMapped = getMappedSegmentPin(pinC);
  DIRECTION_MATRIX[pinCMapped][UP] = pinG;
  DIRECTION_MATRIX[pinCMapped][DOWN] = pinD;
  DIRECTION_MATRIX[pinCMapped][LEFT] = pinE;
  DIRECTION_MATRIX[pinCMapped][RIGHT] = pinDP;

  int pinDMapped = getMappedSegmentPin(pinD);
  DIRECTION_MATRIX[pinDMapped][UP] = pinG;
  DIRECTION_MATRIX[pinDMapped][LEFT] = pinE;
  DIRECTION_MATRIX[pinDMapped][RIGHT] = pinC;

  int pinEMapped = getMappedSegmentPin(pinE);
  DIRECTION_MATRIX[pinEMapped][UP] = pinG;
  DIRECTION_MATRIX[pinEMapped][DOWN] = pinD;
  DIRECTION_MATRIX[pinEMapped][RIGHT] = pinC;

  int pinFMapped = getMappedSegmentPin(pinF);
  DIRECTION_MATRIX[pinFMapped][UP] = pinA;
  DIRECTION_MATRIX[pinFMapped][DOWN] = pinG;
  DIRECTION_MATRIX[pinFMapped][RIGHT] = pinB;

  int pinGMapped = getMappedSegmentPin(pinG);
  DIRECTION_MATRIX[pinGMapped][UP] = pinA;
  DIRECTION_MATRIX[pinGMapped][DOWN] = pinD;

  int pinDPMapped = getMappedSegmentPin(pinDP);
  DIRECTION_MATRIX[pinDPMapped][LEFT] = pinC;
}

int getMappedSegmentPin (int segmentPin) {
  return map(segmentPin, pinA, pinDP, 0, SEGMENT_LEGTH - 1);
}

int getNextSegment (int nextDirection) {
  return DIRECTION_MATRIX[getMappedSegmentPin(crtSelectedSegment)][nextDirection];
}

int getDirectionFromJoystick () {
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

  if (joyYValue < JOY_TOP_TRESHOLD && isJoystickNeutral) {
    isJoystickNeutral = false;
    return UP;
  }

  if (joyYValue > JOY_BOTTOM_TRESHOLD && isJoystickNeutral) {
    isJoystickNeutral = false;
    return DOWN;
  }

  bool isNeutralHorizontally = JOY_LEFT_TRESHOLD <= joyXValue && joyXValue <= JOY_RIGHT_TRESHOLD;
  bool isNeutralVeritcally = JOY_TOP_TRESHOLD <= joyYValue && joyYValue <= JOY_BOTTOM_TRESHOLD;  
  if (isNeutralHorizontally && isNeutralVeritcally) {
    isJoystickNeutral = true;
    return -1;
  }
}

int getToggledValueFromJoystick () {
  int joyYValue = analogRead(JOY_Y_PIN);

  if (joyYValue < JOY_TOP_TRESHOLD && isJoystickNeutral) {
    isJoystickNeutral = false;
    prevJoystickNeutralValue = false;

    return 0;
  }

  if (joyYValue > JOY_BOTTOM_TRESHOLD && isJoystickNeutral) {
    isJoystickNeutral = false;
    prevJoystickNeutralValue = false;
    return 0;
  }

  bool isNeutralVeritcally = JOY_TOP_TRESHOLD <= joyYValue && joyYValue <= JOY_BOTTOM_TRESHOLD;  
  if (isNeutralVeritcally && prevJoystickNeutralValue == false && !isJoystickNeutral) {
    isJoystickNeutral = true;
    prevJoystickNeutralValue = true;

    return 1;
  }

  return 0;
}

void showActiveSegments () {
  for (int i = 0; i < SEGMENT_LEGTH; i++) {
    int segment = segments[i];
    if (segment == crtSelectedSegment) {
      continue;
    }
    

    int mappedSegment = getMappedSegmentPin(segment);
    if (!activeSegments[mappedSegment]) {
      continue;
    }

    digitalWrite(segment, 1);
  }
}

void resetActiveSegments () {
  for (int i = 0; i < SEGMENT_LEGTH; i++) {
    int segment = segments[i];
    int mappedSegment = getMappedSegmentPin(segment);

    digitalWrite(segment, activeSegments[mappedSegment] = 0);
  }
}