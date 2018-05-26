#include "ClockDisplayController.h"

#include "BCDDecoder.h"
#include "TwoDigitNumber.h"
#include "Arduino.h"

ClockDisplayController::ClockDisplayController(int mostSignificantHourPin, int lessSignificantHourPin, int mostSignificantMinutePin, int lessSignificantMinutePin) {
  pinMode(mostSignificantHourPin, OUTPUT);
  pinMode(lessSignificantHourPin, OUTPUT);
  pinMode(mostSignificantMinutePin, OUTPUT);
  pinMode(lessSignificantMinutePin, OUTPUT);

  this->mostSignificantHourPin = mostSignificantHourPin;
  this->lessSignificantHourPin = lessSignificantHourPin;
  this->mostSignificantMinutePin = mostSignificantMinutePin;
  this->lessSignificantMinutePin = lessSignificantMinutePin;
  
  currentPin = lessSignificantMinutePin;
  
  turnOn();
  setMode(NORMAL);
}

void ClockDisplayController::update(const TwoDigitNumber &hour, const TwoDigitNumber &minute, BCDDecoder &decoder) {
  
  manageBlink();

  // We change display on every 2ms.
  if (displayChrono.getElapsedTime()>2){
    // The transistor that was conducting now is in cut-off.
    digitalWrite(currentPin, LOW);

    currentPin = nextPin();

    // Update the visible number.
    updateNumber(hour, minute, decoder);

    // Turn the next transistor on.
    if ((currentPin==mostSignificantHourPin || currentPin==lessSignificantHourPin) && hourOn && on){
      digitalWrite(currentPin,HIGH);
    }
    else if ((currentPin==mostSignificantMinutePin || currentPin==lessSignificantMinutePin) && minuteOn && on){
      digitalWrite(currentPin,HIGH);
    }

    displayChrono.restart();

  }
}

int ClockDisplayController::nextPin() {
  if (currentPin == lessSignificantMinutePin) return mostSignificantMinutePin;
  if (currentPin == mostSignificantMinutePin) return lessSignificantHourPin;
  if (currentPin == lessSignificantHourPin)   return mostSignificantHourPin;
  if (currentPin == mostSignificantHourPin)   return lessSignificantMinutePin;
}

inline void ClockDisplayController::manageBlink(){
  if (mode == HOUR_BLINK){
    if (hourOn == false && blinkChrono.getElapsedTime() > 300){
      hourOn = true;
      blinkChrono.restart();
    }
    else if (hourOn == true && blinkChrono.getElapsedTime() > 300){
      hourOn = false;
      blinkChrono.restart();
    }
  }
  else if (mode == MINUTE_BLINK){
    if (minuteOn == false && blinkChrono.getElapsedTime() > 300){
      minuteOn = true;
      blinkChrono.restart();
    }
    else if (minuteOn == true && blinkChrono.getElapsedTime() > 300){
      minuteOn = false;
      blinkChrono.restart();
    }
  }
}

inline void ClockDisplayController::updateNumber(const TwoDigitNumber &hour, const TwoDigitNumber &minute, BCDDecoder &decoder){
  if (currentPin==lessSignificantMinutePin){
    decoder.setNumber(minute.getLSD());
  }
  else if (currentPin==mostSignificantMinutePin){
    decoder.setNumber(minute.getMSD());
  }
  else if (currentPin==lessSignificantHourPin){
    decoder.setNumber(hour.getLSD());
  }
  else if (currentPin==mostSignificantHourPin){
    decoder.setNumber(hour.getMSD());
  }
}

void ClockDisplayController::setMode(char mode){
  this->mode = mode;
  hourOn = true;
  minuteOn = true;

  blinkChrono.restart();
}

void ClockDisplayController::turnOn() {
    on = true;
}

void ClockDisplayController::turnOff() {
    on = false;
}

void ClockDisplayController::switchDisplay() {
    on = !on;
}
