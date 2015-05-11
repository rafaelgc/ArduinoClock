#include "BCDDecoder.h"
#include "Arduino.h"

BCDDecoder::BCDDecoder(int pinA, int pinB, int pinC, int pinD){
  this->pinA = pinA;
  this->pinB = pinB;
  this->pinC = pinC;
  this->pinD = pinD;
  
  pinMode(pinA,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinC,OUTPUT);
  pinMode(pinD,OUTPUT);
  
  number = 0;
}

void BCDDecoder::setNumber(int number){
  this->number = number;
  
  number = number<0?0:number;
  number = number>9?number%10:number;
  
  if (number==0){
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,LOW);
    digitalWrite(pinC,LOW);
    digitalWrite(pinD,LOW);
  }
  else if (number==1){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
    digitalWrite(pinC,LOW);
    digitalWrite(pinD,LOW);
  }
  else if (number==2){
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,HIGH);
    digitalWrite(pinC,LOW);
    digitalWrite(pinD,LOW);
  }
  else if (number==3){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,HIGH);
    digitalWrite(pinC,LOW);
    digitalWrite(pinD,LOW);
  }
  else if (number==4){
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,LOW);
    digitalWrite(pinC,HIGH);
    digitalWrite(pinD,LOW);
  }
  else if (number==5){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
    digitalWrite(pinC,HIGH);
    digitalWrite(pinD,LOW);
  }
  else if (number==6){
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,HIGH);
    digitalWrite(pinC,HIGH);
    digitalWrite(pinD,LOW);
  }
  else if (number==7){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,HIGH);
    digitalWrite(pinC,HIGH);
    digitalWrite(pinD,LOW);
  }
  else if (number==8){
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,LOW);
    digitalWrite(pinC,LOW);
    digitalWrite(pinD,HIGH);
  }
  else if (number==9){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
    digitalWrite(pinC,LOW);
    digitalWrite(pinD,HIGH);
  }
}

inline int BCDDecoder::getNumber(){
  return number;
}
