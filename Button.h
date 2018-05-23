/*
Rafael González - 07/05/2015
*/

#ifndef BUTTON_H
#define BUTTON_H

class Button{
  private:
  int pin;
  bool lastState;
  unsigned long previousMillis;
  int minDelay;
  
  public:
  Button(int pin, int minDelay = 0);

  /**
   * This method should be called from the loop().
   * It returns true when the state of the button
   * has changed.
   */
  bool update();
  
  bool isPressed();
};

#endif
