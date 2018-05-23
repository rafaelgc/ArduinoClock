#include "Arduino.h"
#include "BCDDecoder.h"
#include "Button.h"
#include "Chrono.h"
#include "Hour.h"
#include "Minute.h"
#include "Mode.h"
#include "ClockDisplayController.h"
#include "TonePlayer.h"

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
const int PIN_A = 2; // less significant bit.
const int PIN_B = 3;
const int PIN_C = 4;
const int PIN_D = 5;

// Uncomment next line to enable the alarm led.
//#define ENABLE_ALARM_LED
const int ALARM_LED = 13;

// Uncomment next line to make a led blink every second.
//#define ENABLE_SECONDS_BLINK
const int SECONDS_LED = 12;

// Pick the alarm melody:
#define BASIC_MELODY
//#define MARIO_MELODY

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


/////////////////////////////////////////////////////
///                  MELODIES                     ///
/////////////////////////////////////////////////////

#ifdef MARIO_MELODY
const int MELODY_SIZE = 156;
const short NOTES[MELODY_SIZE] =    {660,660,660,510,660,770,380,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,500,500,500,580,660,500,430,380,500,500,500,500,580,660,870,760,500,500,500,500,580,660,500,430,380,660,660,660,510,660,770,380};
const short DURATION[MELODY_SIZE] = {83,83,83,83,83,83,83,83,83,83,83,67,83,83,83,67,42,83,67,42,67,67,67,67,83,83,83,83,67,83,83,83,67,42,83,67,42,67,67,67,67,83,83,83,83,125,125,83,83,83,83,83,83,83,83,83,83,125,167,67,67,67,83,83,83,83,83,125,125,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,125,125,83,83,83,83,83,83,83,83,83,83,125,167,67,67,67,83,83,83,83,83,125,125,83,83,83,83,83,83,83,83,83,83,83,83,83,50,67,50,67,67,67,67,67,67,50,67,50,67,67,67,67,67,50,67,50,67,67,67,67,67,67,83,83,83,83,83,83,83};
const short SILENCE[MELODY_SIZE]  = {125,250,250,83,250,458,479,375,333,417,250,275,125,250,167,167,125,250,125,292,250,125,125,417,375,333,417,250,275,125,250,167,167,125,250,125,292,250,125,125,417,250,83,125,125,250,250,125,125,250,125,83,183,250,83,125,125,250,250,250,125,250,250,250,83,125,125,250,250,125,125,250,125,83,350,375,350,300,250,250,125,250,250,83,125,125,250,250,125,125,250,125,83,183,250,83,125,125,250,250,250,125,250,250,250,83,125,125,250,250,125,125,250,125,83,350,375,350,300,250,250,125,250,125,250,292,125,292,125,250,125,500,125,250,292,125,125,458,271,500,125,250,292,125,292,125,250,125,500,125,250,250,83,250,458,479};
#endif

#ifdef BASIC_MELODY
const short MELODY_SIZE = 4;
const short NOTES[MELODY_SIZE] = {950, 950, 950, 950};
const short DURATION[MELODY_SIZE] = {100, 100, 100, 100};
const short SILENCE[MELODY_SIZE] =  {50, 50, 50, 600};
#endif

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


bool alarmOn = false;

// TonePlayer controls the alarm melody playback.
TonePlayer alarmPlayer(BUZZER);

// ClockDisplayController controls the transistors.
ClockDisplayController clockDisplayController(
                                              MOST_SIGNIFICANT_HOUR_DIGIT_TRANSISTOR,
                                              LESS_SIGNIFICANT_HOUR_DIGIT_TRANSISTOR,
                                              MOST_SIGNIFICANT_MINUTE_DIGIT_TRANSISTOR,
                                              LESS_SIGNIFICANT_MINUTE_DIGIT_TRANSISTOR);

// BCDDecoder allows us to select which number will be outputted to the
// decoder.
BCDDecoder decoder(PIN_A, PIN_B, PIN_C, PIN_D);

// We have three different buttons.
Button modeButton(MODE_BUTTON,100), incrementButton(INCREMENT_BUTTON,100), alarmButton(ALARM_BUTTON,100);


Hour hour, alarmHour;
Minute minute, alarmMinute;

// Depending on the mode of the clock we may be interested in showing the
// clock hour/minute or the alarm hour/minute:
// ledChronoer to the current visible hour.
Hour *visibleHour = &hour;
// ledChronoer to the current visible minute.
Minute *visibleMinute = &minute;

// This contains the current mode.
Mode mode;

// clockChrono controls the passage of time so that the seconds
// counter gets incremented every second.
Chrono clockChrono;
// ledChrono is used to control the led that blinks every second (if enabled).
Chrono ledChrono; //Controla el parpadeo del led que marca los segundos.

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
  pinMode(SECONDS_LED, OUTPUT);
  #endif
}

void loop() {

  manageAlarm();
  
  manageInput();
  
  refreshDisplays();
  
  updateTime();
  
}

inline void manageInput(){
  // button.update() returns true when the state of the button
  // changes (from pressed to released or vice versa)
  if (modeButton.update()){
    
    if (modeButton.isPressed()){
      tone(BUZZER, 200, 50);
      // Go to the next mode.
      ++mode;
      
      // If we are in clock-mode.
      if (mode.isClockVisible()){
        // We make sure that the visible hour is the clock hour.
        visibleHour = &hour;
        visibleMinute = &minute;
      }

      // If we are in alarm-mode.
      else if (mode.isAlarmVisible()){
        // We make sure that the visible hour is the alarm hour.
        visibleHour = &alarmHour;
        visibleMinute = &alarmMinute;
      }

      // If we are editing the hour/minute we have to tell
      // the ClockDisplayController. It will handle the display
      // blink.
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
      // We use the alarm button to:
      // - Enabling/disabling the alarm.
      // - Stop the melody playback (if it's being played).

      // If it the melody is not being played...
      if (!alarmPlayer.isPlaying()){
        // Enable/disale the alarm.
        alarmOn=!alarmOn;

        // Give some feedback.
        if (alarmOn) {
          tone(BUZZER, 600, 50);
        }
        else {
          tone(BUZZER, 400, 50);
        }
        
        #ifdef ENABLE_ALARM_LED
        if (alarmOn) { digitalWrite(ALARM_LED, HIGH); }
        else { digitalWrite(ALARM_LED, LOW); }
        #endif
        
      }
      else{
        // Stop the alarm melody.
        alarmPlayer.stop();
      }
    }
  }
}

inline void refreshDisplays(){
  clockDisplayController.update(*visibleHour, *visibleMinute, decoder);

  #ifdef ENABLE_SECONDS_BLINK
  if (ledChrono.getElapsedTime()<1000){
    digitalWrite(SECONDS_LED,HIGH);
  }
  else if (ledChrono.getElapsedTime()<2000){
    digitalWrite(SECONDS_LED,LOW);
  }
  else{
    ledChrono.restart();
  }
  #endif
}

inline void manageAlarm(){

  alarmPlayer.update();
  
}

inline void updateTime(){
  if (clockChrono.getElapsedTime()>=60000){
    if (minute.add(1)){
      //Overflow
      hour.add(1);
    }


    if (alarmOn && hour==alarmHour && minute==alarmMinute && !alarmPlayer.isPlaying()){
      alarmPlayer.play(MELODY_SIZE, NOTES, DURATION, SILENCE, true);
    }

    clockChrono.restart();
    
  }
}

