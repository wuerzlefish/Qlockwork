/******************************************************************************
Settings.h
******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Colors.h"
#include "Configuration.h"
#include "Debug.h"
#include "Languages.h"
#include "Modes.h"

#define SETTINGS_MAGIC_NUMBER 0x2A
#define SETTINGS_VERSION 20

class Settings
{
public:
	Settings();

	uint8_t getLanguage();
	void setLanguage(uint8_t language);

	boolean getUseLdr();
	void toggleUseLdr();

	int16_t getBrightness();
	void setBrightness(int16_t brightness);

	uint8_t getColor();
	void setColor(uint8_t color);

	uint8_t getColorChange();
	void setColorChange(uint8_t colorChange);

	uint8_t getTransition();
	void setTransition(uint8_t transition);

	uint8_t getTimeout();
	void setTimeout(uint8_t timeout);

	boolean getShowTemp();
	void toggleShowTemp();

	boolean getItIs();
	void toggleItIs();

	boolean getAlarm1();
	void toggleAlarm1();
	time_t getAlarmTime1();
	void setAlarmTime1(time_t alarmTime);

	boolean getAlarm2();
	void toggleAlarm2();
	time_t getAlarmTime2();
	void setAlarmTime2(time_t alarmTime);

	time_t getNightOffTime();
	void setNightOffTime(time_t nightOffTime);

	time_t getDayOnTime();
	void setDayOnTime(time_t dayOnTime);

	void saveToEEPROM();

private:
	struct MySettings
	{
		uint8_t magicNumber;
		uint8_t version;
		uint8_t language;
		boolean useLdr;
		int16_t brightness;
		uint8_t color;
		uint8_t colorChange;
		uint8_t transition;
		uint8_t timeout;
		boolean showTemp;
		boolean itIs;
		boolean alarm1;
		time_t alarmTime1;
		boolean alarm2;
		time_t alarmTime2;
		time_t nightOffTime;
		time_t dayOnTime;
	} mySettings;

	void resetToDefault();
	void loadFromEEPROM();
};

#endif
