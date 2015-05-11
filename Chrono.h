/*
Rafael González
09/05/2015
*/

#ifndef CHRONO_H
#define CHRONO_H

class Chrono{
  private:
  unsigned long init;

  public:
  Chrono();
  inline void start();
  unsigned long restart();
  unsigned long getElapsedTime();
};

#endif
