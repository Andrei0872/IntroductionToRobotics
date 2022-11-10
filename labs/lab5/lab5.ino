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

const byte regSize = 8; // 1 byte aka 8 bits

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;

int registers[regSize];

const int encodingsNumber = 16;

int encodingArray[encodingsNumber][regSize] = {
  {1, 1, 1, 1, 1, 1, 0, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1, 0},  // 2
  {1, 1, 1, 1, 0, 0, 1, 0},  // 3
  {0, 1, 1, 0, 0, 1, 1, 0},  // 4
  {1, 0, 1, 1, 0, 1, 1, 0},  // 5
  {1, 0, 1, 1, 1, 1, 1, 0},  // 6
  {1, 1, 1, 0, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1, 0},  // 8
  {1, 1, 1, 1, 0, 1, 1, 0},  // 9
  {1, 1, 1, 0, 1, 1, 1, 0},  // A
  {0, 0, 1, 1, 1, 1, 1, 0},  // b
  {1, 0, 0, 1, 1, 1, 0, 0},  // C
  {0, 1, 1, 1, 1, 0, 1, 0},  // d
  {1, 0, 0, 1, 1, 1, 1, 0},  // E
  {1, 0, 0, 0, 1, 1, 1, 0},  // F
};

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(9600);
}

// V2
int idx = 0;

void loop() {
  // V1
  // for (int i = 0; i < encodingsNumber; i++) {
  //   writeReg(encodingArray[i]);
  //   delay(300);
  // }

  // V2
  writeReg(encodingArray[idx]);
  delay(300);
  idx = (++idx) % 16;
  byte a = bitRead(value, bit)

  // for (int i = regSize - 1; i >= 0; i--) {
  //   registers[i] = LOW;
  //   writeReg(registers);
  //   delay(100);
  //   Serial.print(registers[i]);
  // }

  // Serial.println();

  // for (int i = 0; i < regSize; i++) {
  //   registers[i] = HIGH;
  //   writeReg(registers);
  //   delay(100);
  //   Serial.print(registers[i]);
  // }
  // Serial.println();
}

void writeReg(int encoding[]) {
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < regSize; i++) { // MSBFIRST 
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, encoding[i]);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
}