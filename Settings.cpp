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
	mySettings.magicNumber = SETTINGS_MAGIC_NUMBER;
	mySettings.version = SETTINGS_VERSION;
	mySettings.language = LANGUAGE_EN;
	mySettings.useLdr = false;
	mySettings.brightness = MAX_BRIGHTNESS;
	mySettings.color = WHITE;
	mySettings.colorChange = COLORCHANGE_NO;
	mySettings.transition = TRANSITION_FADE;
	mySettings.timeout = 5;
	mySettings.itIs = true;
	mySettings.alarm1 = false;
	mySettings.alarmTime1 = 0;
	mySettings.alarm2 = false;
	mySettings.alarmTime2 = 0;
	mySettings.nightOffTime = 0;
	mySettings.dayOnTime = 0;
	saveToEEPROM();
}

// Load settings.
void Settings::loadFromEEPROM()
{
	EEPROM.begin(512);
	EEPROM.get(0, mySettings);
	if ((mySettings.magicNumber != SETTINGS_MAGIC_NUMBER) || (mySettings.version != SETTINGS_VERSION))
		resetToDefault();
}

// Write settings.
void Settings::saveToEEPROM()
{
	EEPROM.begin(512);
	EEPROM.put(0, mySettings);
	EEPROM.commit();
}
