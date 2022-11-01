const int BUZZER_PIN = 7;

const int CAR_RED_PIN = 6;
const int CAR_YELLOW_PIN = 5;
const int CAR_GREEN_PIN = 4;

const int PEOPLE_GREEN_PIN = 8;
const int PEOPLE_RED_PIN = 9;

const int BUTTON_PIN = 2;

enum FlowState {
  State1 = 1,
  State2 = 2,
  State3 = 3,
  State4 = 4,
};
FlowState currentFlowState = State1;

const int STATE1_TRANSITION_DURATION_MS = 8000;
const int STATE2_TRANSITION_DURATION_MS = 3000;
const int STATE3_TRANSITION_DURATION_MS = 8000;
const int STATE4_TRANSITION_DURATION_MS = 4000;

const int BUZZER_STATE3_INTERVAL_MS = 700;
const int BUZZER_STATE4_INTERVAL_MS = 200;

const int STATE4_PEOPLE_GREEN_BLINK_INTERVAL_MS = 300;

unsigned long currentFlowStateTimpestamp;
unsigned long buzzerStartTimestamp;
unsigned long peopleGreenPinBlinkTimestamp;

bool hasFlowStarted = false;
bool isBuzzerActive = false;
bool isState4PeopleGreenActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(CAR_RED_PIN, OUTPUT);
  pinMode(CAR_YELLOW_PIN, OUTPUT);
  pinMode(CAR_GREEN_PIN, OUTPUT);

  pinMode(PEOPLE_GREEN_PIN, OUTPUT);
  pinMode(PEOPLE_RED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int buttonValue = !digitalRead(BUTTON_PIN);

  if (!hasFlowStarted && buttonValue == LOW && currentFlowState == State1) {
    digitalWrite(CAR_GREEN_PIN, HIGH);
    digitalWrite(CAR_RED_PIN, LOW);
    digitalWrite(PEOPLE_RED_PIN, HIGH);
    digitalWrite(PEOPLE_GREEN_PIN, LOW);

    currentFlowStateTimpestamp = millis();
  } else if (!hasFlowStarted && buttonValue == HIGH && currentFlowState == State1) {
    digitalWrite(CAR_GREEN_PIN, HIGH);
    digitalWrite(PEOPLE_RED_PIN, HIGH);

    hasFlowStarted = true;
  } else if (currentFlowState == State1) {
    unsigned long elapsedTime = millis() - currentFlowStateTimpestamp;
    if (elapsedTime > STATE1_TRANSITION_DURATION_MS) {
      currentFlowState = State2;
      currentFlowStateTimpestamp = millis();
    }
  } else if (currentFlowState == State2) {
      digitalWrite(CAR_YELLOW_PIN, HIGH);
      digitalWrite(CAR_GREEN_PIN, LOW);
      digitalWrite(PEOPLE_RED_PIN, HIGH);

      unsigned long elapsedTime = millis() - currentFlowStateTimpestamp;
      if (elapsedTime > STATE2_TRANSITION_DURATION_MS) {
        currentFlowState = State3;
        currentFlowStateTimpestamp = millis();

        buzzerStartTimestamp = millis();
        isBuzzerActive = true;
      }
  } else if (currentFlowState == State3) {
      digitalWrite(CAR_RED_PIN, HIGH);
      digitalWrite(CAR_YELLOW_PIN, LOW);
      digitalWrite(PEOPLE_GREEN_PIN, HIGH);
      digitalWrite(PEOPLE_RED_PIN, LOW);

      if (isBuzzerActive) {
        tone(BUZZER_PIN, 1000);
      } else {
        noTone(BUZZER_PIN);
      }

      unsigned long elapsedBuzzerTime = millis() - buzzerStartTimestamp;
      if (elapsedBuzzerTime > BUZZER_STATE3_INTERVAL_MS) {
        isBuzzerActive = !isBuzzerActive;
        buzzerStartTimestamp = millis();
      }

      unsigned long elapsedCurrentFlowStateTime = millis() - currentFlowStateTimpestamp;
      if (elapsedCurrentFlowStateTime > STATE3_TRANSITION_DURATION_MS) {

        currentFlowState = State4;
        currentFlowStateTimpestamp = millis();

        isBuzzerActive = true;
        noTone(BUZZER_PIN);
        buzzerStartTimestamp = millis();

        peopleGreenPinBlinkTimestamp = millis();
        isState4PeopleGreenActive = true;
      }
  } else if (currentFlowState == State4) {
    unsigned long elapsedBuzzerTime = millis() - buzzerStartTimestamp;
    if (isBuzzerActive) {
      tone(BUZZER_PIN, 1000);
    } else {
      noTone(BUZZER_PIN);
    }

    if (elapsedBuzzerTime > BUZZER_STATE4_INTERVAL_MS) {
      isBuzzerActive = !isBuzzerActive;
      buzzerStartTimestamp = millis();
    }
        
    bool peopleGreenNextState = isState4PeopleGreenActive ? HIGH : LOW;
    digitalWrite(PEOPLE_GREEN_PIN, peopleGreenNextState);

    unsigned long elapsedPeopleGreenTime = millis() - peopleGreenPinBlinkTimestamp;
    if (elapsedPeopleGreenTime > STATE4_PEOPLE_GREEN_BLINK_INTERVAL_MS) {
      peopleGreenPinBlinkTimestamp = millis();
      isState4PeopleGreenActive = !isState4PeopleGreenActive;
    }

    unsigned long elapsedCurrentFlowStateTime = millis() - currentFlowStateTimpestamp;
    if (elapsedCurrentFlowStateTime > STATE4_TRANSITION_DURATION_MS) {
      currentFlowState = State1;
      currentFlowStateTimpestamp = millis();

      noTone(BUZZER_PIN);

      hasFlowStarted = false;
    }
  }
} 