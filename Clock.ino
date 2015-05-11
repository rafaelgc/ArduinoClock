#include "Arduino.h"
#include "BCDDecoder.h"
#include "Button.h"
#include "Chrono.h"
#include "Hour.h"
#include "Minute.h"
#include "Mode.h"
#include "ClockDisplayController.h"
#include "Tone.h"

const int BUZZER = 6;
const int ALARM_LED = 11;
const int POINT_LED = 12;

bool alarmOn = false;

Tone alarmTone(6); //Controla el tono de la alarma.
ClockDisplayController clockDisplayController(2); //Controla los transistores que activan/desactivan los displays.

BCDDecoder decoder(7,8,9,10); //Sirve para seleccionar el número que mostrará el display activo.
Button modeButton(A0,100), incrementButton(A1,100), alarmButton(A2,100);


Hour hour, alarmHour;
Minute minute, alarmMinute;

Hour *visibleHour = &hour; //Puntero a la hora que se muestra en el display.
Minute *visibleMinute = &minute;

Mode mode; //Para gestionar los estados en los que está el reloj: NORMAL, modificar HORA/MINUTO, etc.

Chrono clockChrono; //Controla el paso del tiempo para ir aumentando los minutos del reloj.
Chrono point; //Controla el parpadeo del led que marca los segundos.

inline void manageAlarm();
inline void manageInput();
inline void refreshDisplays();
inline void updateTime();

void setup() {
  Serial.begin(9600);
  
  pinMode(BUZZER, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);
  pinMode(POINT_LED, OUTPUT);
  
}

void loop() {

  manageAlarm();
  
  manageInput();
  
  refreshDisplays();
  
  updateTime();
  
}

inline void manageInput(){
  if (modeButton.update()){
    //El método update() devuelve true sólo cuando el botón ha cambiado de estado.

    if (modeButton.isPressed()){
      tone(BUZZER,200,50);
      ++mode; //Se cambia el modo. (El operador ++ está sobrecargado).
      
      //Si estamos en modo alarma...
      if (mode.isClockVisible()){
        //Cambiamos al modo reloj.
        visibleHour = &hour;
        visibleMinute = &minute;
      }
      //Y si estamos en modo reloj...
      else if (mode.isAlarmVisible()){
        //Cambiamos al modo alarma.
        visibleHour = &alarmHour;
        visibleMinute = &alarmMinute;
      }

      //Dependiendo de si estamos o no editando la hora o el minuto, se
      //le indica al clockDisplayController que haga parpadear los displays.
      if (mode.isEditingHour()){
        clockDisplayController.setMode(ClockDisplayController::HOUR_BLINK);
      }
      else if (mode.isEditingMinute()){
        clockDisplayController.setMode(ClockDisplayController::MINUTE_BLINK);
      }
      else{
        clockDisplayController.setMode(ClockDisplayController::NORMAL);
      }
      
    }
  }
  
  if (incrementButton.update()){
    if (incrementButton.isPressed()){
      
      if (mode==Mode::SET_HOUR) {
        hour.add(1);
      }
      else if (mode==Mode::SET_MINUTE) {
        minute.add(1);
      }
      else if (mode==Mode::SET_ALARM_HOUR) {
        alarmHour.add(1);
      }
      else if (mode==Mode::SET_ALARM_MINUTE){
        alarmMinute.add(1);
      }
      
    }
  }
  
  if (alarmButton.update()){
    if (alarmButton.isPressed()){

      /*
      El botón de alarma sirve para cambiar el estado de la alarma
      (encendido/apagado) y, si está sonando la alarma también sirve
      para apagarla.
      */

      //Si la alarma no está sonando...
      if (!alarmTone.isPlaying()){
        //Se cambia su estado.
        alarmOn=!alarmOn;
        
        //Y se encienden/apagan el led que lo indica.
        if (alarmOn) { digitalWrite(ALARM_LED, HIGH); }
        else { digitalWrite(ALARM_LED, LOW); }
        
      }
      else{
        //Si la alarma está sonando, se para.
        alarmTone.stop();
      }
    }
  }
}

inline void refreshDisplays(){
  clockDisplayController.update(*visibleHour,*visibleMinute,decoder);

  //PARPADEO DEL LED QUE MARCA LOS SEGUNDOS:
  if (point.getElapsedTime()<1000){
    digitalWrite(POINT_LED,HIGH);
  }
  else if (point.getElapsedTime()<2000){
    digitalWrite(POINT_LED,LOW);
  }
  else{
    point.restart();
  }
}

inline void manageAlarm(){

  alarmTone.update();
  
}

inline void updateTime(){
  if (clockChrono.getElapsedTime()>=60000){
    if (minute.add(1)){
      //Overflow
      hour.add(1);
    }


    if (alarmOn && hour==alarmHour && minute==alarmMinute && !alarmTone.isPlaying()){
      alarmTone.play(true);
    }

    clockChrono.restart();
    
  }
}

