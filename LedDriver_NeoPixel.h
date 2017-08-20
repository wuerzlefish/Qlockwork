/******************************************************************************
   LedDriver_NeoPixel.h
******************************************************************************/

#ifndef LEDDRIVER_NEOPIXEL_H
#define LEDDRIVER_NEOPIXEL_H

#include <Adafruit_NeoPixel.h>
#include "Colors.h"
#include "Configuration.h"

#if !defined(LED_LAYOUT_DUAL)
#define NUM_LEDS 115
#endif

#if defined(LED_LAYOUT_DUAL)
#define NUM_LEDS 230
#endif

class LedDriver_NeoPixel
{
public:
	LedDriver_NeoPixel();
	String getSignature();
	void clear();
	void show();
	void setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness);
	void setPixel(uint8_t num, uint8_t color, uint8_t brightness);

private:
	Adafruit_NeoPixel *strip;
};

#endif
