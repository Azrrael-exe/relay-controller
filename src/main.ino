#include <Arduino.h>
#include <Keypad.h>
#include <KeypadConfig.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ModbusMaster.h>
#include <relay.h>

#define N 8

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long wd;
uint8_t action = NON_ACTION;

void setup() {

}

void loop() {
  char key = keypad.getKey();

  if(action == NON_ACTION && key!= NO_KEY){
    if(key == '#'){
      action = read;
      wd = millis();
    }
    else if(key == '*'){
      action = write;
      wd = millis();
    }
  }

  if(action == WRITE){

  }

  if((millis() - wd > 10000) && action != NON_ACTION){
    action = NON_ACTION;
  }
}
