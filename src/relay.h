#ifndef relay_h
#define relay_h

#include <Arduino.h>

#define NON_ACTION 0
#define PRE_WRITE 1
#define WRITE 2
#define READ 3

class Relay {
public:
  Relay();
  Relay(uint8_t i);
  bool setRelay(Stream& uart, bool state);
  bool getRelay(Stream& uart, uint8_t (&arr)[8]);
  bool swapRelay(Stream& uart);
private:
  uint8_t index;
  bool state;
};

Relay::Relay(){
  ;
}

Relay::Relay(uint8_t i){
  index = i;
}

bool Relay::setRelay(Stream& uart, bool s){

  while (uart.available()) {
    uart.read();
  }

  uint8_t frame[8] = {0x01, 0x05, 0x00, index, s, 0x00, 0x00, 0x00};
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
    uart.write(frame[i]);
  }
  state = s;
  return state;
}

bool Relay::getRelay(Stream& uart, uint8_t (&arr)[8]){

  while (uart.available()) {
    uart.read();
  }

  uint8_t frame[8] = {0x01, 0x01, 0x00, index, 0x00, 0x04, 0x00, 0x00};
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
    uart.write(frame[i]);
  }
  delay(100);
  for(int i = 0; i<6; i++){
    arr[i] = uart.read();
  }
  state = bool(arr[3]);
  return state;
}

bool Relay::swapRelay(Stream& uart){

  while (uart.available()) {
    uart.read();
  }

  state = !state;
  uint8_t frame[8] = {0x01, 0x05, 0x00, index, state, 0x00, 0x00, 0x00};
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
    uart.write(frame[i]);
  }
  return state;
}


#endif
