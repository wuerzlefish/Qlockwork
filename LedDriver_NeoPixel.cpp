/******************************************************************************
   LedDriver_NeoPixel.cpp
******************************************************************************/

#include "LedDriver_NeoPixel.h"

LedDriver_NeoPixel::LedDriver_NeoPixel() {
#ifdef LED_DRIVER_NEO_800KHZ_GRB
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_GRB + NEO_KHZ800);
#endif
#ifdef LED_DRIVER_NEO_800KHZ_RGB
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_RGB + NEO_KHZ800);
#endif
#ifdef LED_DRIVER_NEO_800KHZ_RGBW
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_RGBW + NEO_KHZ800);
#endif
#ifdef LED_DRIVER_NEO_400KHZ_GRB
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_GRB + NEO_KHZ400);
#endif
#ifdef LED_DRIVER_NEO_400KHZ_RGB
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_RGB + NEO_KHZ400);
#endif
	strip->begin();
}

String LedDriver_NeoPixel::getSignature() {
	return "NeoPixel";
}

void LedDriver_NeoPixel::clear() {
	for (uint8_t i = 0; i < NUM_LEDS; i++) strip->setPixelColor(i, 0);
}

void LedDriver_NeoPixel::show() {
	strip->show();
}

void LedDriver_NeoPixel::setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness) {
#ifdef LED_LAYOUT_HORIZONTAL
	setPixel(x + (y * 11), color, brightness);
#endif
#ifdef LED_LAYOUT_VERTICAL
	setPixel(y + (x * 10), color, brightness);
#endif
}

void LedDriver_NeoPixel::setPixel(uint8_t num, uint8_t color, uint8_t brightness) {
	uint8_t red = map(brightness, 0, 255, 0, defaultColors[color].red);
	uint8_t green = map(brightness, 0, 255, 0, defaultColors[color].green);
	uint8_t blue = map(brightness, 0, 255, 0, defaultColors[color].blue);
	uint32_t ledColor = (red << 16) + (green << 8) + blue;
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
