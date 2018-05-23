/*
Rafael González - 07/05/2015
*/

#ifndef BCDDECODER_H
#define BCDDECODER_H

class BCDDecoder{
  private:
  int pinA, pinB, pinC, pinD;
  int number;
  
  public:
  /*
  pinA -> LSB
  pinD -> MSB
  */
  BCDDecoder(int pinA, int pinB, int pinC, int pinD);
  void setNumber(int number);
  inline int getNumber();
};

#endif
