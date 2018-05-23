#include "TonePlayer.h"
#include "Arduino.h"

TonePlayer::TonePlayer(int buzzerPin) {
	this->buzzerPin = buzzerPin;
	currentNote = -1;
 
	loop = false;
}

void TonePlayer::play(int size, const short* notes, const short* duration, const short* silence, bool loop) {
  this->loop = loop;
  this->size = size;
  this->notes = notes;
  this->duration = duration;
  this->silence = silence;
  this->size = size;
  
	currentNote = 0;
	if (currentNote!=size){
		tone(buzzerPin, notes[currentNote],duration[currentNote]);
		chrono.restart();
	}
}

void TonePlayer::update() {
	if (isPlaying()){
		if (chrono.getElapsedTime() > duration[currentNote] + silence[currentNote]){
			++currentNote;
			if (currentNote==size){
				if (loop){
					currentNote = 0;
				}
				else{
					currentNote = -1;
					return;
				}
			}

			tone(buzzerPin, notes[currentNote], duration[currentNote]);
			chrono.restart();
		}
	}
}

bool TonePlayer::hasFinished() {
	return currentNote==-1;
}

void TonePlayer::stop() {
	currentNote = -1;
}

bool TonePlayer::isPlaying() {
	return currentNote>=0;
}
