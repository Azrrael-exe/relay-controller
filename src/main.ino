#include <Arduino.h>
#include <Keypad.h>
#include <KeypadConfig.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#include <relay.h>

#define N 8

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Relay relays[N];

long wd;
uint8_t action = NON_ACTION;

void setup() {
  // --- Screen Init ---
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Hello, world!");

  // --- Modbus Init ---
  Serial.begin(9600);
  for(int i = 0; i<N ; i++){
    relays[i] = Relay(i);
  }
}

bool status = true;

void loop() {

  char key = keypad.getKey();

  if(isDigit(key)){
    uint8_t index = String(key).toInt();
    uint8_t buf[8];
    relays[index].getRelay(Serial, buf);
    lcd.clear();
    lcd.setCursor(0, 0);
    for(int i=0;i<6;i++){
      lcd.print(buf[i], HEX);
      lcd.print(" ");
    }
  }

  if(key == '#'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Write!: ");
  }

  if(key == '*'){

    while (Serial.available()) {
      Serial.read();
    }

    uint8_t frame[8] = {0x01, 0x01, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00};
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < 6; i++) {
      crc ^= (uint16_t)frame[i];
      for (int i = 8; i != 0; i--) {    // Loop over each bit
        if ((crc & 0x0001) != 0) {      // If the LSB is set
          crc >>= 1;                    // Shift right and XOR 0xA001
          crc ^= 0xA001;
        }
        else                            // Else LSB is not set
         crc >>= 1;                    // Just shift right
       }
     }
     // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    frame[6] = crc & 0x00FF;
    frame[7] = crc >> 8;
    for(int i=0; i<8; i++){
      Serial.write(frame[i]);
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    while (Serial.available()) {
      lcd.print(Serial.read(), HEX);
      lcd.print(" ");
    }

  }

}
