#include "Mode.h"

Mode::Mode(){
  currentMode = CLOCK;
}

void Mode::operator=(int mode){
  currentMode = mode;
}


void Mode::operator++(){
  if (currentMode==SET_ALARM_HOUR){
    currentMode = CLOCK;
  }
  else{
    ++currentMode;
  }
}

bool Mode::operator==(int mode){
  return currentMode==mode;
}

bool Mode::operator!=(int mode){
  return currentMode!=mode;
}

bool Mode::isEditingHour(){
  return currentMode==SET_HOUR || currentMode==SET_ALARM_HOUR;
}

bool Mode::isEditingMinute(){
  return currentMode==SET_MINUTE || currentMode==SET_ALARM_MINUTE;
}

bool Mode::isClockVisible(){
  return currentMode==CLOCK || currentMode==SET_HOUR || currentMode==SET_HOUR;
}

bool Mode::isAlarmVisible(){
  return currentMode==SET_ALARM_HOUR || currentMode==SET_ALARM_MINUTE;
}
