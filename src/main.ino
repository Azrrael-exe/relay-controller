#include <Arduino.h>
#include <Keypad.h>
#include <KeypadConfig.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#include <ModbusMaster.h>
#include <relay.h>

#define N 8

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
ModbusMaster node;

long wd;
uint8_t action = NON_ACTION;
bool state = 0;

void setup() {
  // --- Screen Init ---
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Hello, world!");

  // --- Modbus Init ---
  Serial.begin(9600);
  node.begin(1, Serial);
}

void loop() {

  char key = keypad.getKey();

  if(key == '1'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Adr 1");
    lcd.setCursor(0, 1);
    uint8_t frame[8] = {0x00, 0x06, 0x40, 0x00, 0x00, 0x01, 0x5c, 0x1b};
    for(int i=0; i<8; i++){
      Serial.write(frame[i]);
    }
    delay(300);
    while(Serial.available()){
      lcd.print(Serial.read(), HEX);
      lcd.print(" ");
    }
  }
  if(key == '2'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Read Relay");
    lcd.setCursor(0, 1);
    uint8_t frame[8] = {0x00, 0x03, 0x40, 0x00, 0x00, 0x01, 0x90, 0x1b};
    for(int i=0; i<8; i++){
      Serial.write(frame[i]);
    }
    delay(300);
    while(Serial.available()){
      lcd.print(Serial.read(), HEX);
      lcd.print(" ");
    }
  }

  if(key == '4'){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Read Relay");
    lcd.setCursor(0, 1);
    uint8_t frame[8] = {0x01, 0x05, 0x00, 0x01, 0x01, 0x00, 0x9d, 0x9a};
    for(int i=0; i<8; i++){
      Serial.write(frame[i]);
    }
    delay(300);
    while(Serial.available()){
      lcd.print(Serial.read(), HEX);
      lcd.print(" ");
    }
  }



  if(key == '#'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Write!: ");
    uint8_t result = node.writeSingleCoil(0x0002, state);
    state = !state;
    lcd.print(result);
  }

  if(key == '*'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Read!");
  }

}
