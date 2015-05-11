/*
Rafael González - 09/05/2015
*/

#ifndef TWODIGITNUMBER_H
#define TWODIGITNUMBER_H

/*Interface*/

class TwoDigitNumber{
  private:
  int mostSignificantDigit, lessSignificantDigit;
  int maxNumber;
  public:
  TwoDigitNumber(int maxNumber = 99);

  //Devuelve el dígito más significativo.
  virtual int getMSD() const;
  //Devuelve el dígito menos significativo.
  virtual int getLSD() const;
  
  virtual bool add(int amount);
  virtual void set(int number);
  
  virtual bool operator==(const TwoDigitNumber &other) const;
  virtual bool operator!=(const TwoDigitNumber &other) const;
};

#endif
