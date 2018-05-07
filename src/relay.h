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
  bool getState();
  bool setState();
private:
  bool state;
};

#endif
