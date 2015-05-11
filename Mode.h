/*
Rafael González - 09/05/2015
*/

#ifndef MODE_H
#define MODE_H

class Mode{
  private:
  int currentMode;
  public:
  enum {CLOCK, SET_HOUR, SET_MINUTE, SET_ALARM_HOUR, SET_ALARM_MINUTE};
  
  Mode();
  void operator=(int mode);
  bool operator!=(int mode);
  void operator++();
  bool operator==(int mode);
  
  bool isEditingHour();
  bool isEditingMinute();
  
  bool isClockVisible();
  bool isAlarmVisible();
  
};

#endif
