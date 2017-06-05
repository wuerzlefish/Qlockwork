/******************************************************************************
   LedDriver_LPD8806RGBW.cpp
******************************************************************************/

#include "LedDriver_LPD8806RGBW.h"

LedDriver_LPD8806RGBW::LedDriver_LPD8806RGBW() {
	strip = new LPD8806RGBW(NUM_LEDS, PIN_LEDS_DATA, PIN_LEDS_CLOCK);
	strip->begin();
}

String LedDriver_LPD8806RGBW::getSignature() {
	return "LPD8806RGBW";
}

void LedDriver_LPD8806RGBW::clear() {
	for (uint8_t i = 0; i < NUM_LEDS; i++) strip->setPixelColor(i, 0);
}

void LedDriver_LPD8806RGBW::show() {
	strip->show();
}

void LedDriver_LPD8806RGBW::setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness) {
#ifdef LED_LAYOUT_HORIZONTAL
	setPixel(x + (y * 11), color, brightness);
#endif
#ifdef LED_LAYOUT_VERTICAL
	setPixel(y + (x * 10), color, brightness);
#endif
}

void LedDriver_LPD8806RGBW::setPixel(uint8_t num, uint8_t color, uint8_t brightness) {
	uint8_t red = map(brightness, 0, 255, 0, defaultColors[color].red / 2); // LPD8806 only uses 7 bit for color
	uint8_t green = map(brightness, 0, 255, 0, defaultColors[color].green / 2);
	uint8_t blue = map(brightness, 0, 255, 0, defaultColors[color].blue / 2);
	uint32_t ledColor = (blue << 16) + (red << 8) + green;
#ifdef LED_LAYOUT_HORIZONTAL
	if (num < 110) {
		if (num / 11 % 2 == 0) strip->setPixelColor(num, ledColor);
		else strip->setPixelColor(num / 11 * 11 + 10 - (num % 11), ledColor);
	}
	else {
		switch (num) {
		case 110: // upper-left
			strip->setPixelColor(111, ledColor);
			break;
		case 111: // upper-right
			strip->setPixelColor(112, ledColor);
			break;
		case 112: // bottom-right
			strip->setPixelColor(113, ledColor);
			break;
		case 113: // bottom-left
			strip->setPixelColor(110, ledColor);
			break;
		case 114: // alarm
			strip->setPixelColor(114, ledColor);
			break;
		default:
			break;
		}
	}
#endif // LED_LAYOUT_HORIZONTAL
#ifdef LED_LAYOUT_VERTICAL
	uint8_t ledNum;
	if (num < 110) {
		if (num / 10 % 2 == 0) ledNum = num;
		else ledNum = num / 10 * 10 + 9 - (num % 10);
		if (ledNum < 10) strip->setPixelColor(ledNum + 1, ledColor);
		else if (ledNum < 100) strip->setPixelColor(ledNum + 2, ledColor);
		else strip->setPixelColor(ledNum + 3, ledColor);
	}
	else {
		switch (num) {
		case 110: // upper-left
			strip->setPixelColor(0, ledColor);
			break;
		case 111: // upper-right
			strip->setPixelColor(102, ledColor);
			break;
		case 112: // bottom-right
			strip->setPixelColor(113, ledColor);
			break;
		case 113: // bottom-left
			strip->setPixelColor(11, ledColor);
			break;
		case 114: // alarm
			strip->setPixelColor(114, ledColor);
			break;
		default:
			break;
		}
	}
#endif // LED_LAYOUT_VERTICAL
}
