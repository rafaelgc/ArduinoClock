#include "ClockDisplayController.h"

#include "BCDDecoder.h"
#include "TwoDigitNumber.h"
#include "Arduino.h"

ClockDisplayController::ClockDisplayController(int lessSignificantPin){
  pinMode(lessSignificantPin, OUTPUT);
  pinMode(lessSignificantPin+1, OUTPUT);
  pinMode(lessSignificantPin+2, OUTPUT);
  pinMode(lessSignificantPin+3, OUTPUT);
  
  this->lessSignificantPin = lessSignificantPin;
  currentPin = lessSignificantPin;
  
  setMode(NORMAL);
}

void ClockDisplayController::update(const TwoDigitNumber &hour, const TwoDigitNumber &minute, BCDDecoder &decoder) {
  
  manageBlink();

  //Cada 2ms se cambia el display que se activa.
  if (displayChrono.getElapsedTime()>2){
    //El transistor que estuvo conduciendo hasta ahora pasará a estar
    //en corte, por lo que se apagará el display.
    digitalWrite(currentPin, LOW);

    currentPin++;
    if (currentPin>=lessSignificantPin+4){
      currentPin = lessSignificantPin;
    }

    //Se cambia el número que representa el decodificador.
    updateNumber(hour, minute, decoder);

    //Por último, se enciende el transistor del siguiente display.
    if ((currentPin==lessSignificantPin+2 || currentPin==lessSignificantPin+3) && hourOn){
      digitalWrite(currentPin,HIGH);
    }
    else if ((currentPin==lessSignificantPin || currentPin==lessSignificantPin+1) && minuteOn){
      digitalWrite(currentPin,HIGH);
    }

    displayChrono.restart();

  }
}

inline void ClockDisplayController::manageBlink(){
  if (mode==HOUR_BLINK){
    if (hourOn==false && blinkChrono.getElapsedTime()>300){
      hourOn = true;
      blinkChrono.restart();
    }
    else if (hourOn==true && blinkChrono.getElapsedTime()>300){
      hourOn = false;
      blinkChrono.restart();
    }
  }
  else if (mode==MINUTE_BLINK){
    if (minuteOn==false && blinkChrono.getElapsedTime()>300){
      minuteOn = true;
      blinkChrono.restart();
    }
    else if (minuteOn==true && blinkChrono.getElapsedTime()>300){
      minuteOn = false;
      blinkChrono.restart();
    }
  }
}

inline void ClockDisplayController::updateNumber(const TwoDigitNumber &hour, const TwoDigitNumber &minute, BCDDecoder &decoder){
  if (currentPin==lessSignificantPin){
    decoder.setNumber(minute.getLSD());
  }
  else if (currentPin==lessSignificantPin+1){
    decoder.setNumber(minute.getMSD());
  }
  else if (currentPin==lessSignificantPin+2){
    decoder.setNumber(hour.getLSD());
  }
  else if (currentPin==lessSignificantPin+3){
    decoder.setNumber(hour.getMSD());
  }
}

void ClockDisplayController::setMode(char mode){
  this->mode = mode;
  hourOn = true;
  minuteOn = true;

  blinkChrono.restart();
}

