/*
Rafael González - 10/05/2015
*/

#ifndef TONEPLAYER_H
#define TONEPLAYER_H

#include "Chrono.h"

class TonePlayer {
private:
	int currentNote, buzzerPin, size;
  const short *notes, *duration, *silence;
  
	Chrono chrono;
	bool loop;
public:
	TonePlayer(int buzzerPin);
	void update();
	void play(int size, const short* notes, const short* duration, const short* silence, bool loop = false);
	void stop();
	bool isPlaying();
	bool hasFinished();

};

#endif
