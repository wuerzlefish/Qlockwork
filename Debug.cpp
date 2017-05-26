/******************************************************************************
   Debug.cpp
******************************************************************************/

#include "Debug.h"

Debug::Debug() {
}

// write screenbuffer to console
void Debug::debugScreenBuffer(word ScreenBuffer[]) {
	const char buchstabensalat[][17] = {
	  { "ESKISTAFUNF1" },
	  { "ZEHNZWANZIG2" },
	  { "DREIVIERTEL3" },
	  { "VORFUNKNACH4" },
	  { "HALBAELFUNFA "},
	  { "EINSXAMZWEI_ "},
	  { "DREIPMJVIER_ "},
	  { "SECHSNLACHT_ "},
	  { "SIEBENZWOLF_ "},
	  { "ZEHNEUNKUHR_ "}
	};
	Serial.println("\033[0;0H"); // set cursor to 0, 0 position
	Serial.println(" -----------");
	for (uint8_t line = 0; line < 10; line++) {
		word leds = ScreenBuffer[line];
		char row[16];
		for (int8_t i = 15; i >= 0; i--) {
			row[i] = ((leds & 1) ? buchstabensalat[line][i] : ' ');
			leds = leds >> 1;
		}
		Serial.print('|');
		for (uint8_t i = 0; i < 11; i++) Serial.print(row[i]);
		Serial.print('|');
		Serial.println(row[11]); // set corner LEDs
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
		Serial.print("FPS: ");
		Serial.println(frames);
		lastFpsCheck = millis();
		frames = 0;
	}
}
