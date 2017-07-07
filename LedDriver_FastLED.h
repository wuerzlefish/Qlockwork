/******************************************************************************
   LedDriver_FastLED.h
******************************************************************************/

#ifndef LEDDRIVER_FASTLED_H
#define LEDDRIVER_FASTLED_H

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include "Colors.h"
#include "Configuration.h"

class LedDriver_FastLED
{
public:
	LedDriver_FastLED();
	String getSignature();
	void clear();
	void show();
	void setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness);
	void setPixel(uint8_t num, uint8_t color, uint8_t brightness);

private:
	CRGB leds[NUM_LEDS];
};

#endif
