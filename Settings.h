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
#define SETTINGS_VERSION 24

class Settings
{
public:
	Settings();

	//uint8_t getLanguage();
	//void setLanguage(uint8_t language);

	boolean getUseAbc();
	void setUseAbc(boolean onOff);
	void toggleUseAbc();

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
	void setShowTemp(boolean onOff);
	void toggleShowTemp();

	boolean getItIs();
	void setItIs(boolean onOff);
	void toggleItIs();

	boolean getAlarm1();
	void setAlarm1(boolean onOff);
	void toggleAlarm1();
	time_t getAlarm1Time();
	void setAlarm1Time(time_t alarmTime);
	uint8_t getAlarm1Weekdays();
	void setAlarm1Weekdays(uint8_t alarmWeekdays);

	boolean getAlarm2();
	void setAlarm2(boolean onOff);
	void toggleAlarm2();
	time_t getAlarm2Time();
	void setAlarm2Time(time_t alarmTime);
	uint8_t getAlarm2Weekdays();
	void setAlarm2Weekdays(uint8_t alarmWeekdays);

	time_t getNightOffTime();
	void setNightOffTime(time_t nightOffTime);

	time_t getDayOnTime();
	void setDayOnTime(time_t dayOnTime);

	//boolean getShowDebug();
	//void setShowDebug(boolean onOff);

	void saveToEEPROM();

private:
	struct MySettings
	{
		uint8_t magicNumber;
		uint8_t version;
		boolean useAbc;
		int16_t brightness;
		uint8_t color;
		uint8_t colorChange;
		uint8_t transition;
		uint8_t timeout;
		boolean showTemp;
		boolean itIs;
		boolean alarm1;
		time_t alarm1Time;
		uint8_t alarm1Weekdays;
		boolean alarm2;
		time_t alarm2Time;
		uint8_t alarm2Weekdays;
		time_t nightOffTime;
		time_t dayOnTime;
		//boolean showDebug;
	} mySettings;

	void resetToDefault();
	void loadFromEEPROM();
};

#endif
