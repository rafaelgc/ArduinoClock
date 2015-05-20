#include "Button.h"
#include "Arduino.h"

Button::Button(int pin, int minDelay){
  lastState = false;
  this->minDelay = minDelay;
  previousMillis = 0;
  this->pin = pin;
  pinMode(pin,INPUT);
}

bool Button::update(){
  //Para que cambie el estado debe pasar un cierto tiempo,
  //minDelay.
  if ((unsigned long)(millis() - previousMillis) >= minDelay){
    
    bool currState = digitalRead(pin);
    
    if (currState!=lastState){
      lastState = currState;
      previousMillis = millis();
      return true;
    }
    else{
      return false;
    }
    
  }
  return false;
}

bool Button::isPressed(){
  return lastState;
}
