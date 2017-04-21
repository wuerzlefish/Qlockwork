/******************************************************************************
   Debug.h
******************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <TimeLib.h>

#ifdef DEBUG
#define DEBUG_PRINT(x)   Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

class Debug {
public:
	Debug();
	void debugScreenBuffer(word ScreenBuffer[]);
	void debugTime(String label, time_t time);
	void debugFps();

private:
	uint16_t frames = 0;
	uint32_t lastFpsCheck = 0;
};

#endif
