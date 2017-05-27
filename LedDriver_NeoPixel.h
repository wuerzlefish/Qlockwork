/******************************************************************************
   LedDriver_NeoPixel.h
******************************************************************************/

#ifndef LEDDRIVER_NEOPIXEL_H
#define LEDDRIVER_NEOPIXEL_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "Colors.h"
#include "Configuration.h"

class LedDriver_NeoPixel {

public:
	LedDriver_NeoPixel();
	void writeScreenBufferToLEDs(word matrix[16], uint8_t color, uint8_t brightness);
	String getSignature();

private:
	Adafruit_NeoPixel *strip;
	void setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness);
};

#endif
