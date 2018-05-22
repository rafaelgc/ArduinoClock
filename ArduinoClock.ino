#include "Arduino.h"
#include "BCDDecoder.h"
#include "Button.h"
#include "Chrono.h"
#include "Hour.h"
#include "Minute.h"
#include "Mode.h"
#include "ClockDisplayController.h"
#include "Tone.h"

/////////////////////////////////////////////////////
///                CONFIGURATION                  ///
/////////////////////////////////////////////////////

const int BUZZER = 10;
const int MODE_BUTTON = 11;
const int INCREMENT_BUTTON = 12;
const int ALARM_BUTTON = 13;

// Four transistors controls which digit is visible
// at a time.
const int MOST_SIGNIFICANT_MINUTE_DIGIT_TRANSISTOR = 9;
const int LESS_SIGNIFICANT_MINUTE_DIGIT_TRANSISTOR = 8;

const int MOST_SIGNIFICANT_HOUR_DIGIT_TRANSISTOR = 7;
const int LESS_SIGNIFICANT_HOUR_DIGIT_TRANSISTOR = 6;

// We use four bits to represent a digit. Those bits are
// outputted to the decoder and then sent to the displays.
const int PIN_A = 2;
const int PIN_B = 3;
const int PIN_C = 4;
const int PIN_D = 5;

// Uncomment next line to enable the alarm led.
//#define ENABLE_ALARM_LED
const int ALARM_LED = 13;

// Uncomment next line to make a led blink every second.
//#define ENABLE_SECONDS_BLINK
const int POINT_LED = 12;

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

bool alarmOn = false;

Tone alarmTone(BUZZER); //Controla el tono de la alarma.

ClockDisplayController clockDisplayController(
                                              MOST_SIGNIFICANT_HOUR_DIGIT_TRANSISTOR,
                                              LESS_SIGNIFICANT_HOUR_DIGIT_TRANSISTOR,
                                              MOST_SIGNIFICANT_MINUTE_DIGIT_TRANSISTOR,
                                              LESS_SIGNIFICANT_MINUTE_DIGIT_TRANSISTOR);

BCDDecoder decoder(PIN_A, PIN_B, PIN_C, PIN_D); //Sirve para seleccionar el número que mostrará el display activo.
Button modeButton(MODE_BUTTON,100), incrementButton(INCREMENT_BUTTON,100), alarmButton(ALARM_BUTTON,100);


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
  #ifdef ENABLE_ALARM_LED
  pinMode(ALARM_LED, OUTPUT);
  #endif

  #ifdef ENABLE_SECONDS_BLINK
  pinMode(POINT_LED, OUTPUT);
  #endif
  
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
      tone(BUZZER, 200, 50);
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

        if (alarmOn) {
          tone(BUZZER, 600, 50);
        }
        else {
          tone(BUZZER, 400, 50);
        }
        
        //Y se encienden/apagan el led que lo indica.
        #ifdef ENABLE_ALARM_LED
        if (alarmOn) { digitalWrite(ALARM_LED, HIGH); }
        else { digitalWrite(ALARM_LED, LOW); }
        #endif
        
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

  #ifdef ENABLE_SECONDS_BLINK
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
  #endif
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

