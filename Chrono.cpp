#include "Chrono.h"
#include "Arduino.h"

Chrono::Chrono(){
  init = 0;
}

inline void Chrono::start(){
  init = millis();
}

unsigned long Chrono::restart(){
  unsigned long ret = getElapsedTime();
  init = millis();
  return ret;
}

unsigned long Chrono::getElapsedTime(){
  return (unsigned long)(millis() - init);
}
