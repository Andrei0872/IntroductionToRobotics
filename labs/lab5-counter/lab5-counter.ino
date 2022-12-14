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
const int encodingsNumber = 16;


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
  
unsigned long lastIncrement = 0;
unsigned long delayCount = 50;
unsigned long number = 0;

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

void loop() {
  // writeNumber(1234); // first test with this and increase the delay inside the writeNumber function

 writeNumber(number);

  if (millis() - lastIncrement > delayCount) {
    number++;
    number %= 10000;
    lastIncrement = millis();
  }
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
  // first, disable all the display digits
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // then enable only the digit you want to use now
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