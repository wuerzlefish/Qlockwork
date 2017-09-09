/******************************************************************************
Settings.cpp
******************************************************************************/

#include "Settings.h"

Settings::Settings()
{
	loadFromEEPROM();
}

uint8_t Settings::getLanguage()
{
	return mySettings.language;
}

void Settings::setLanguage(uint8_t language)
{
	mySettings.language = language;
}

boolean Settings::getUseLdr()
{
	return mySettings.useLdr;
}

void Settings::toggleUseLdr()
{
	mySettings.useLdr = !mySettings.useLdr;
}

int16_t Settings::getBrightness()
{
	return mySettings.brightness;
}

void Settings::setBrightness(int16_t ledBrightness)
{
	mySettings.brightness = ledBrightness;
}

uint8_t Settings::getColor()
{
	return mySettings.color;
}

void Settings::setColor(uint8_t color)
{
	mySettings.color = color;
}

uint8_t Settings::getColorChange()
{
	return mySettings.colorChange;
}

void Settings::setColorChange(uint8_t colorChange)
{
	mySettings.colorChange = colorChange;
}

uint8_t Settings::getTransition()
{
	return mySettings.transition;
}

void Settings::setTransition(uint8_t transition)
{
	mySettings.transition = transition;
}

boolean Settings::getShowTemp()
{
	return mySettings.showTemp;
}

void Settings::toggleShowTemp()
{
	mySettings.showTemp = !mySettings.showTemp;
}

uint8_t Settings::getTimeout()
{
	return mySettings.timeout;
}

void Settings::setTimeout(uint8_t timeout)
{
	mySettings.timeout = timeout;
}

boolean Settings::getItIs()
{
	return mySettings.itIs;
}

void Settings::toggleItIs()
{
	mySettings.itIs = !mySettings.itIs;
}

boolean Settings::getAlarm1()
{
	return mySettings.alarm1;
}

void Settings::toggleAlarm1()
{
	mySettings.alarm1 = !mySettings.alarm1;
}

time_t Settings::getAlarmTime1()
{
	return mySettings.alarmTime1;
}

void Settings::setAlarmTime1(time_t alarmTime)
{
	mySettings.alarmTime1 = alarmTime;
}

boolean Settings::getAlarm2()
{
	return mySettings.alarm2;
}

void Settings::toggleAlarm2()
{
	mySettings.alarm2 = !mySettings.alarm2;
}

time_t Settings::getAlarmTime2()
{
	return mySettings.alarmTime2;
}

void Settings::setAlarmTime2(time_t alarmTime)
{
	mySettings.alarmTime2 = alarmTime;
}

time_t Settings::getNightOffTime()
{
	return mySettings.nightOffTime;
}

void Settings::setNightOffTime(time_t nightOffTime)
{
	mySettings.nightOffTime = nightOffTime;
}

time_t Settings::getDayOnTime()
{
	return mySettings.dayOnTime;
}

void Settings::setDayOnTime(time_t dayOnTime)
{
	mySettings.dayOnTime = dayOnTime;
}

// Set all defaults.
void Settings::resetToDefault()
{
	DEBUG_PRINTLN("*** Settings set to defaults in EEPROM. ***");
	mySettings.magicNumber = SETTINGS_MAGIC_NUMBER;
	mySettings.version = SETTINGS_VERSION;
#ifdef LANGUAGE_ENGLISH
	mySettings.language = LANGUAGE_EN;
#endif
#ifdef LANGUAGE_GERMAN
	mySettings.language = LANGUAGE_DE_DE;
#endif
#ifdef LANGUAGE_SPANISH
	mySettings.language = LANGUAGE_ES;
#endif
#ifdef LANGUAGE_FRENCH
	mySettings.language = LANGUAGE_FR;
#endif
#ifdef LANGUAGE_ITALIEN
	mySettings.language = LANGUAGE_IT;
#endif
#ifdef LANGUAGE_NETHERLANDS
	mySettings.language = LANGUAGE_NL;
#endif
	mySettings.useLdr = DEFAULT_USELDR;
	mySettings.brightness = DEFAULT_BRIGHTNESS;
	mySettings.color = DEFAULT_COLOR;
	mySettings.colorChange = DEFAULT_COLORCHANGE;
	mySettings.transition = DEFAULT_TRANSITION;
	mySettings.timeout = DEFAULT_TIMEOUT;
	mySettings.showTemp = DEFAULT_SHOWTEMP;
	mySettings.itIs = DEFAULT_SHOWITIS;
	mySettings.alarm1 = false;
	mySettings.alarmTime1 = 0;
	mySettings.alarm2 = false;
	mySettings.alarmTime2 = 0;
	mySettings.nightOffTime = 0;
	mySettings.dayOnTime = 0;
	saveToEEPROM();
}

// Load settings from EEPROM.
void Settings::loadFromEEPROM()
{
	DEBUG_PRINTLN("Settings loaded from EEPROM.");
	EEPROM.begin(512);
	EEPROM.get(0, mySettings);
	if ((mySettings.magicNumber != SETTINGS_MAGIC_NUMBER) || (mySettings.version != SETTINGS_VERSION))
		resetToDefault();
	EEPROM.end();
}

// Write settings to EEPROM.
void Settings::saveToEEPROM()
{
	DEBUG_PRINTLN("Settings saved to EEPROM.");
	EEPROM.begin(512);
	EEPROM.put(0, mySettings);
	//EEPROM.commit();
	EEPROM.end();
}
