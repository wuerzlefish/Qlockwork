/******************************************************************************
   LedDriver_LPD8806RGBW.cpp
******************************************************************************/

#include "LedDriver_LPD8806RGBW.h"

LedDriver_LPD8806RGBW::LedDriver_LPD8806RGBW()
{
	strip = new LPD8806RGBW(NUM_LEDS, PIN_LEDS_DATA, PIN_LEDS_CLOCK);
	strip->begin();
}

String LedDriver_LPD8806RGBW::getSignature()
{
	return "LPD8806RGBW";
}

void LedDriver_LPD8806RGBW::clear()
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
		strip->setPixelColor(i, 0);
}

void LedDriver_LPD8806RGBW::show()
{
	strip->show();
}

void LedDriver_LPD8806RGBW::setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness)
{
#ifdef LED_LAYOUT_HORIZONTAL
	setPixel(x + (y * 11), color, brightness);
#endif
#ifdef LED_LAYOUT_VERTICAL
	setPixel((x * 10) + y, color, brightness);
#endif
}

void LedDriver_LPD8806RGBW::setPixel(uint8_t num, uint8_t color, uint8_t brightness)
{
	uint8_t red = map(brightness, 0, 255, 0, defaultColors[color].red / 2); // LPD8806 only uses 7 bit for color
	uint8_t green = map(brightness, 0, 255, 0, defaultColors[color].green / 2);
	uint8_t blue = map(brightness, 0, 255, 0, defaultColors[color].blue / 2);
	uint32_t ledColor = (blue << 16) + (red << 8) + green;

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

#ifdef LED_LAYOUT_DUAL
	strip->setPixelColor(num * 2, ledColor);
	strip->setPixelColor(num * 2 + 1, ledColor);
#else
	strip->setPixelColor(num, ledColor);
#endif

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

#if defined(LED_LAYOUT_VERTICAL) && defined(LED_LAYOUT_DUAL)

#endif // LED_LAYOUT_VERTICAL DUAL

}
