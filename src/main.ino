#include <Arduino.h>
#include <Keypad.h>
#include <KeypadConfig.h>

#include <relay.h>

#define N 5

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <lcdConfig.h>

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Relay relays[N];

long wd;
uint8_t status = NON_ACTION;
uint8_t pos = 0;
uint8_t queue[N];

void clearQueue(){
  for(int i = 0; i<N ; i++){
    queue[i] = 0;
  }
  pos = 0;
}

void setup() {
  // --- Screen Init ---
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0,0); //Start at character 4 on line 0

  // --- Modbus Init ---
  Serial.begin(9600);
  for(int i = 0; i<N ; i++){
    uint8_t buf[8];
    relays[i] = Relay(i);
    relays[i].getRelay(Serial, buf);
    delay(100);
  }
  showStatus(lcd, relays);
}

void loop() {

  char key = keypad.getKey();

  if(isDigit(key)){
    if(status == WRITING){
      uint8_t index = String(key).toInt();
      if(index != 0 && index < N){
        queue[pos] = index;
        if(pos == 0){
          lcd.setCursor(0, 1);
          lcd.print(index);
        }
        else {
          lcd.print(",");
          lcd.print(index);
        }
        pos++;
      }
    }
  }

  if(key == 'A' && status == WRITING){
    turnOnAll(Serial);
    for(int i=0;i<N; i++){
      relays[i].setLocalStatus(true);
    }
    clearQueue();
    status = SEND;
  }

  if(key == 'B' && status == WRITING){
    turnOffAll(Serial);
    for(int i=0;i<N; i++){
      relays[i].setLocalStatus(false);
    }
    clearQueue();
    status = SEND;
  }

  if(key == '*'){
    if(status == NON_ACTION){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Status: Writing");
      status = WRITING;
      wd = millis();
    }
    else if(status == WRITING){
      status = SEND;
    }
  }

  if(millis() - wd > 10000 && status == WRITING) {
    status = NON_ACTION;
    clearQueue();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Status: Time Out!");
    delay(2000);
    clearQueue();
    showStatus(lcd, relays);
  }

  if(status == SEND){
    status = NON_ACTION;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Status: Sending");
    for(int i = 0; i<pos; i++){
      relays[queue[i]].swapRelay(Serial);
      delay(200);
    }
    clearQueue();
    showStatus(lcd, relays);
  }
  
}
