/******************************************************************************
Settings.cpp
******************************************************************************/

#include "Settings.h"

Settings::Settings()
{
	loadFromEEPROM();
}

boolean Settings::getUseAbc()
{
	return mySettings.useAbc;
}

void Settings::setUseAbc(boolean onOff)
{
	mySettings.useAbc = onOff;
}

void Settings::toggleUseAbc()
{
	mySettings.useAbc = !mySettings.useAbc;
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

boolean Settings::getModeChange()
{
	return mySettings.modeChange;
}

void Settings::setModeChange(boolean onOff)
{
	mySettings.modeChange = onOff;
}

void Settings::toggleModeChange()
{
	mySettings.modeChange = !mySettings.modeChange;
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

void Settings::setItIs(boolean onOff)
{
	mySettings.itIs = onOff;
}

void Settings::toggleItIs()
{
	mySettings.itIs = !mySettings.itIs;
}

boolean Settings::getAlarm1()
{
	return mySettings.alarm1;
}

void Settings::setAlarm1(boolean onOff)
{
	mySettings.alarm1 = onOff;
}

void Settings::toggleAlarm1()
{
	mySettings.alarm1 = !mySettings.alarm1;
}

time_t Settings::getAlarm1Time()
{
	return mySettings.alarm1Time;
}

void Settings::setAlarm1Time(time_t alarmTime)
{
	mySettings.alarm1Time = alarmTime;
}

uint8_t Settings::getAlarm1Weekdays()
{
	return mySettings.alarm1Weekdays;
}

void Settings::setAlarm1Weekdays(uint8_t alarmWeekdays)
{
	mySettings.alarm1Weekdays = alarmWeekdays;
}

boolean Settings::getAlarm2()
{
	return mySettings.alarm2;
}

void Settings::setAlarm2(boolean onOff)
{
	mySettings.alarm2 = onOff;
}

void Settings::toggleAlarm2()
{
	mySettings.alarm2 = !mySettings.alarm2;
}

time_t Settings::getAlarm2Time()
{
	return mySettings.alarm2Time;
}

void Settings::setAlarm2Time(time_t alarmTime)
{
	mySettings.alarm2Time = alarmTime;
}

uint8_t Settings::getAlarm2Weekdays()
{
	return mySettings.alarm2Weekdays;
}

void Settings::setAlarm2Weekdays(uint8_t alarmWeekdays)
{
	mySettings.alarm2Weekdays = alarmWeekdays;
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
	Serial.println("*** Settings set to defaults in EEPROM. ***");
	mySettings.magicNumber = SETTINGS_MAGIC_NUMBER;
	mySettings.version = SETTINGS_VERSION;
	mySettings.useAbc = DEFAULT_USEABC;
	mySettings.brightness = DEFAULT_BRIGHTNESS;
	mySettings.color = DEFAULT_COLOR;
	mySettings.colorChange = DEFAULT_COLORCHANGE;
	mySettings.transition = DEFAULT_TRANSITION;
	mySettings.timeout = DEFAULT_TIMEOUT;
	mySettings.modeChange = DEFAULT_MODECHANGE;
	mySettings.itIs = DEFAULT_SHOWITIS;
	mySettings.alarm1 = DEFAULT_ALARM1;
	mySettings.alarm1Time = DEFAULT_ALARM1TIME;
	mySettings.alarm1Weekdays = DEFAULT_ALARM1WEEKDAYS;
	mySettings.alarm2 = DEFAULT_ALARM2;
	mySettings.alarm2Time = DEFAULT_ALARM2TIME;
	mySettings.alarm2Weekdays = DEFAULT_ALARM2WEEKDAYS;
	mySettings.nightOffTime = DEFAULT_NIGHTOFF;
	mySettings.dayOnTime = DEFAULT_DAYON;
	saveToEEPROM();
}

// Load settings from EEPROM.
void Settings::loadFromEEPROM()
{
	Serial.println("Settings loaded from EEPROM.");
	EEPROM.begin(512);
	EEPROM.get(0, mySettings);
	if ((mySettings.magicNumber != SETTINGS_MAGIC_NUMBER) || (mySettings.version != SETTINGS_VERSION))
		resetToDefault();
	EEPROM.end();
}

// Write settings to EEPROM.
void Settings::saveToEEPROM()
{
	Serial.println("Settings saved to EEPROM.");
	EEPROM.begin(512);
	EEPROM.put(0, mySettings);
	//EEPROM.commit();
	EEPROM.end();
}
