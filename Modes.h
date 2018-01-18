/******************************************************************************
Modes.h
******************************************************************************/

#ifndef MODES_H
#define MODES_H

typedef enum eMode : uint8_t
{
	MODE_TIME,
	MODE_AMPM,
	MODE_SECONDS,
	MODE_WEEKDAY,
	MODE_DATE,
	MODE_MOONPHASE,
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
	MODE_TEMP,
#endif
#ifdef SENSOR_DHT22
	MODE_HUMIDITY,
#endif
	MODE_EXT_TEMP,
	MODE_EXT_HUMIDITY,
	MODE_EXT_CONDITION,
#ifdef BUZZER
	MODE_TIMER,
#endif
	EXT_MODE_TEST,
	EXT_MODE_RED,
	EXT_MODE_GREEN,
	EXT_MODE_BLUE,
	EXT_MODE_WHITE,
	MODE_COUNT,
	MODE_BLANK,
	MODE_FEED
} Mode;

// Overload the ControlType++ operator.
inline Mode& operator++(Mode& eDOW, int)
{
	const uint8_t i = static_cast<uint8_t>(eDOW) + 1;
	eDOW = static_cast<Mode>((i) % MODE_COUNT);
	return eDOW;
}

enum eTransition : uint8_t
{
	TRANSITION_NORMAL,
	TRANSITION_FADE
};

#endif
