/******************************************************************************
LedDriver_NeoPixel.cpp
******************************************************************************/

#include "LedDriver_NeoPixel.h"

LedDriver_NeoPixel::LedDriver_NeoPixel()
{
#ifdef LED_DRIVER_NEO_800KHZ_GRB
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_GRB + NEO_KHZ800);
#endif
#ifdef LED_DRIVER_NEO_800KHZ_RGB
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_RGB + NEO_KHZ800);
#endif
#ifdef LED_DRIVER_NEO_800KHZ_GRBW
	strip = new Adafruit_NeoPixel(NUM_LEDS, PIN_LEDS_DATA, NEO_GRBW + NEO_KHZ800);
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

String LedDriver_NeoPixel::getSignature()
{
	return "NeoPixel";
}

void LedDriver_NeoPixel::clear()
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
		strip->setPixelColor(i, 0);
}

void LedDriver_NeoPixel::show()
{
	strip->show();
}

void LedDriver_NeoPixel::setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness)
{
#ifdef LED_LAYOUT_HORIZONTAL
	setPixel(x + (y * 11), color, brightness);
#endif
#ifdef LED_LAYOUT_VERTICAL
	setPixel((x * 10) + y, color, brightness);
#endif
}

void LedDriver_NeoPixel::setPixel(uint8_t num, uint8_t color, uint8_t brightness)
{
	uint8_t red = map(brightness, 0, 255, 0, defaultColors[color].red);
	uint8_t green = map(brightness, 0, 255, 0, defaultColors[color].green);
	uint8_t blue = map(brightness, 0, 255, 0, defaultColors[color].blue);
	uint32_t ledColor = (red << 16) + (green << 8) + blue;

#ifdef LED_LAYOUT_HORIZONTAL

	if (num < 110)
	{
		if (num / 11 % 2 != 0)
		{
			num = num / 11 * 11 + 10 - (num % 11);
		}
	}
	else
	{
		switch (num)
		{
		case 110: // upper-left
			num = 111;
			break;
		case 111: // upper-right
			num = 112;
			break;
		case 112: // bottom-right
			num = 113;
			break;
		case 113: // bottom-left
			num = 110;
			break;
			//case 114: // alarm
			//	num = 114;
			//	break;
		default:
			break;
		}
	}

	strip->setPixelColor(num, ledColor);

#endif // LED_LAYOUT_HORIZONTAL

#ifdef LED_LAYOUT_VERTICAL

	if (num < 110)
	{
		if (num / 10 % 2 != 0)
		{
			num = num / 10 * 10 + 9 - (num % 10);
		}
		if (num < 10)
		{
			num += 1;
		}
		else
		{
			if (num < 100)
			{
				num += 2;
			}
			else
			{
				num += 3;
			}
		}
	}
	else
	{
		switch (num)
		{
		case 110: // upper-left
			num = 0;
			break;
		case 111: // upper-right
			num = 102;
			break;
		case 112: // bottom-right
			num = 113;
			break;
		case 113: // bottom-left
			num = 11;
			break;
			//case 114: // alarm
			//	num = 114;
			//	break;
		default:
			break;
		}
	}

	strip->setPixelColor(num, ledColor);

#endif // LED_LAYOUT_VERTICAL

}
