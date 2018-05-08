#ifndef keyPadConfig_h
#define keyPadConfig_h

#include <Arduino.h>

#define C1 4      // Col 1
#define C2 5      // Col 2
#define C3 6      // Col 3
#define C4 7      // Col 4

#define R1 8      // Row 1
#define R2 9      // Row 2
#define R3 10     // Row 3
#define R4 11     // Row 4

const byte ROWS = 4;    //four rows
const byte COLS = 4;    //four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[4] = {R1, R2, R3, R4};   //connect to the row pinouts of the keypad
byte colPins[4] = {C1, C2, C3, C4};   //connect to the column pinouts of the keypad

#endif
