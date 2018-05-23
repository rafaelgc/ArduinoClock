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

  digitalWrite(pinA, number & 1);
  digitalWrite(pinB, number & 2);
  digitalWrite(pinC, number & 4);
  digitalWrite(pinD, number & 8);
}

inline int BCDDecoder::getNumber(){
  return number;
}
