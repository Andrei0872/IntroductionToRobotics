#include <LiquidCrystal.h>
#include <EEPROM.h>
const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

const byte LCD_CONTRAST_PIN = 3;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

byte lcdContrast;

void setup() {
  Serial.begin(9600);
  pinMode(LCD_CONTRAST_PIN, OUTPUT);
  
// set up the LCD's number of columns and rows:
lcd.begin(16, 2);
// // Print a message to the LCD.
lcd.print("hello, world!");

// delay(1000);
// lcd.clear();

  lcdContrast = EEPROM.read(0);
  analogWrite(LCD_CONTRAST_PIN, lcdContrast);
}

String s;
String cmd;

bool isLastCmdEnded = false;

void loop() {

  if (Serial.available()) {
    lcdContrast = Serial.parseInt();
    Serial.read();
    Serial.println(lcdContrast);

    analogWrite(LCD_CONTRAST_PIN, lcdContrast);
    EEPROM.update(0, lcdContrast);
  }

  return;
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
// lcd.setCursor(0, 1);
// print the number of seconds since reset:
// lcd.print(millis() / 1000);


  // Cate caract sunt necitite.
  // Serial.available();
  // if (Serial.available() > 0) {
    // Serial.println(Serial.available());
    // Serial.println((char)Serial.read());

    // Serial.println(Serial.parseInt());
    // Read the next thing.
    // Serial.read();

    // while (Serial.available() > 0 && (Serial.peek() == '\n' || Serial.peek() == '\r')) {
    //   Serial.read();
    // }

    // char c = Serial.read();
    // if (c == '\n') {
    //   // Use the string `s`.
    // } else {
    //   s += c;
    // }
    
  // }

  if (Serial.available() > 0) {
    // char c = Serial.read();
    // if (c == '\n') {
    //   lcd.clear();
    //   lcd.print(cmd);
    //   cmd = "";
    // } else {
    //   cmd += c;
    // }
  
    // cmd = Serial.readStringUntil('\n');
    // lcd.clear();
    // lcd.print(cmd);

    char c = Serial.read();
    if (c == '\n') {
      isLastCmdEnded = true;

    } else {
      if (isLastCmdEnded) {
        lcd.clear();
        isLastCmdEnded = false;
      }
      
      // Write a single byte.
      lcd.write(c);
      
    }
  }
}