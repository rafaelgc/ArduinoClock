#ifndef CLOCKDISPLAYCONTROLLER_H
#define CLOCKDISPLAYCONTROLLER_H

#include "Chrono.h"

class TwoDigitNumber;
class BCDDecoder;

class ClockDisplayController {
private:
	int lessSignificantPin, currentPin;
	Chrono displayChrono, blinkChrono;

	//El estado se almacena en un char para ahorrar memoria.
	char mode;

	bool hourOn, minuteOn;
	inline void manageBlink();
	inline void updateNumber(const TwoDigitNumber &hour, const TwoDigitNumber &minute, BCDDecoder &decoder);
public:
	static const char NORMAL = 0, HOUR_BLINK = 1, MINUTE_BLINK = 2;

	ClockDisplayController(int lessSignificantPin);
	
	void update(const TwoDigitNumber &hour, const TwoDigitNumber &minute, BCDDecoder &decoder);
	void setMode(char mode);

};

#endif
