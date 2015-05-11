/*
Rafael González - 10/05/2015
*/

#ifndef TONE_H
#define TONE_H

#include "Chrono.h"

class Tone{
private:
	static const unsigned int SIZE = 156;
	static const int notes[SIZE];
	static const int duration[SIZE];
	static int silence[SIZE];

	int currentNote, buzzerPin;
	Chrono chrono;
	bool loop;
public:
	Tone(int buzzerPin);
	void update();
	void play(bool loop=false);
	void stop();
	bool isPlaying();
	bool hasFinished();

};

#endif