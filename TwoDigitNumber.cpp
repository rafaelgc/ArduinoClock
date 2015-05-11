#include "TwoDigitNumber.h"

TwoDigitNumber::TwoDigitNumber(int maxNumber){
  this->maxNumber = maxNumber;
  mostSignificantDigit = 0;
  lessSignificantDigit = 0;
}
int TwoDigitNumber::getMSD() const{
  return mostSignificantDigit;
}
int TwoDigitNumber::getLSD() const{
  return lessSignificantDigit;
}

bool TwoDigitNumber::add(int amount){
  bool ret = false;
  int num = mostSignificantDigit*10+lessSignificantDigit;
  num+=amount;
  
  if (num>maxNumber){
    ret = true;
    num = 0;
  }
  set(num);
  
  return ret;
  
}
void TwoDigitNumber::set(int number){
  lessSignificantDigit = number%10;
  mostSignificantDigit = (number/10)%10;
}

bool TwoDigitNumber::operator==(const TwoDigitNumber &other) const{
  return other.getMSD()==mostSignificantDigit && other.getLSD()==lessSignificantDigit;
} 
bool TwoDigitNumber::operator!=(const TwoDigitNumber &other) const{
  return (*this)==other;
}
