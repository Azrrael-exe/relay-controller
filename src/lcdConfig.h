#ifndef lcdConfig_h
#define lcdConfig_h

#include <Arduino.h>

void showStatus(LiquidCrystal_I2C &lcd, Relay (&relay)[N]){
  lcd.clear();
  for(int i=1; i<N; i++){
    if(i<5){
      lcd.setCursor(0, i-1);
    }
    else {
      lcd.setCursor(10, i-5);
    }
    char stringBuf[10];
    sprintf (stringBuf,"Relay%i: %i", i, relay[i].getLocalStatus());
    lcd.print(stringBuf);
  }
}

#endif
