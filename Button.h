/*
Rafael González - 07/05/2015
*/

/*
Nota: se da por hecho que +5V implican que el botón está presionado y 0V.
Se consigue con una resistencia PULL-UP.
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
  
  /*Se debe llamar desde el bucle de la aplicación.
  Devuelve true si el estado ha cambiado.
  */
  bool update();
  /*
  Devuelve PRESSED si el botón está presionado.
  Devuelve UNPRESSED si no está presionado.
  */
  bool isPressed();
};

#endif
