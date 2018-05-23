/*
Rafael González - 09/05/2015
*/

#ifndef TWODIGITNUMBER_H
#define TWODIGITNUMBER_H

/*Interface*/

class TwoDigitNumber {
  private:
  int mostSignificantDigit, lessSignificantDigit;
  int maxNumber;
  public:
  TwoDigitNumber(int maxNumber = 99);

  virtual int getMSD() const; // Most significand digit.
  virtual int getLSD() const; // Less significant digit.
  
  virtual bool add(int amount);
  virtual void set(int number);
  
  virtual bool operator==(const TwoDigitNumber &other) const;
  virtual bool operator!=(const TwoDigitNumber &other) const;
};

#endif
