#include <LiquidCrystal.h>
#include <EEPROM.h>

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

const byte lcdContrastPin = 3;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte lcdContrast;

void setup() {
  Serial.begin(9600);
  pinMode(lcdContrastPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Hello world!");

  lcdContrast = EEPROM.read(0);
  analogWrite(lcdContrastPin, lcdContrast);
}

void loop() {
  
  if(Serial.available() > 0) {
    lcdContrast = Serial.parseInt();
    Serial.read();
    Serial.println(lcdContrast);

    analogWrite(lcdContrastPin, lcdContrast);
    EEPROM.update(0, lcdContrast);
  }
  
}



