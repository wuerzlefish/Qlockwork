/******************************************************************************
   Debug.cpp
******************************************************************************/

#include "Debug.h"

Debug::Debug() {
}

// write screenbuffer to console
void Debug::debugScreenBuffer(uint16_t screenBuffer[]) {
	const char buchstabensalat[][13] = {
	  { "ESKISTAFUNF1" },
	  { "ZEHNZWANZIG2" },
	  { "DREIVIERTEL3" },
	  { "VORFUNKNACH4" },
	  { "HALBAELFUNFA" },
	  { "EINSXAMZWEI " },
	  { "DREIPMJVIER " },
	  { "SECHSNLACHT " },
	  { "SIEBENZWOLF " },
	  { "ZEHNEUNKUHR " }
	};
	Serial.println("\033[0;0H"); // set cursor to 0, 0 position
	Serial.println(" -----------");
	for (uint8_t y = 0; y <= 9; y++) {
		Serial.print('|');
		for (uint8_t x = 0; x <= 10; x++) {
			Serial.print((bitRead(screenBuffer[y], 15 - x) ? buchstabensalat[y][x] : ' '));
		}
		Serial.print('|');
		Serial.println((bitRead(screenBuffer[y], 4) ? buchstabensalat[y][11] : ' '));
	}
	Serial.println(" -----------");
}

// write time to console
void Debug::debugTime(String label, time_t time) {
	Serial.print(label);
	Serial.printf(" %02d:%02d:%02d %02d.%02d.%04d\n\r", hour(time), minute(time), second(time), day(time), month(time), year(time));
}

// write FPS to console
void Debug::debugFps() {
	frames++;
	if (millis() - lastFpsCheck > 1000) {
		lastFpsCheck = millis();
		Serial.println("FPS: " + String(frames));
		frames = 0;
	}
}
