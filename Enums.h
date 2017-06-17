/******************************************************************************
   Enums.h
******************************************************************************/

#ifndef ENUMS_H
#define ENUMS_H

typedef enum eMode : uint8_t {
	STD_MODE_NORMAL,
	STD_MODE_AMPM,
	STD_MODE_SECONDS,
	STD_MODE_WEEKDAY,
	STD_MODE_DATE,
	STD_MODE_TITLE_TEMP,
#ifdef RTC_BACKUP
	STD_MODE_TEMP,
#endif
	STD_MODE_EXT_TEMP,
#ifdef BUZZER
	STD_MODE_TITLE_ALARM,
	STD_MODE_SET_TIMER,
	STD_MODE_TIMER,
	STD_MODE_ALARM_1,
	STD_MODE_SET_ALARM_1,
	STD_MODE_ALARM_2,
	STD_MODE_SET_ALARM_2,
#endif
	STD_MODE_COUNT,
	STD_MODE_BLANK,
	EXT_MODE_START,
	EXT_MODE_TITLE_MAIN = EXT_MODE_START,
#ifdef LDR
	EXT_MODE_LDR,
#endif
	EXT_MODE_BRIGHTNESS,
	EXT_MODE_COLORCHANGE,
	EXT_MODE_COLOR,
	EXT_MODE_TRANSITION,
	EXT_MODE_TIMEOUT,
	EXT_MODE_LANGUAGE,
	EXT_MODE_TITLE_TIME,
	EXT_MODE_TIMESET,
	EXT_MODE_IT_IS,
	EXT_MODE_DAYSET,
	EXT_MODE_MONTHSET,
	EXT_MODE_YEARSET,
	EXT_MODE_TEXT_NIGHTOFF,
	EXT_MODE_NIGHTOFF,
	EXT_MODE_TEXT_DAYON,
	EXT_MODE_DAYON,
	EXT_MODE_TITLE_IP,
	EXT_MODE_IP_0,
	EXT_MODE_IP_1,
	EXT_MODE_IP_2,
	EXT_MODE_IP_3,
	EXT_MODE_TITLE_TEST,
	EXT_MODE_TEST,
	EXT_MODE_COUNT
} Mode;

// Overload the ControlType++ operator
inline Mode& operator++(Mode& eDOW, int) {
	const uint8_t i = static_cast<uint8_t>(eDOW) + 1;
	eDOW = static_cast<Mode>((i) % EXT_MODE_COUNT);
	return eDOW;
}

enum eColor : uint8_t {
	WHITE,
	RED,
	GREEN,
	BLUE,
	CYAN,
	MAGENTA,
	YELLOW,
	AMETHYST,
	AQUA,
	BLUEVIOLET,
	BROWN,
	BURLYWOOD,
	CADETBLUE,
	CORAL,
	CRIMSON,
	FORESTGREEN,
	GOLD,
	GOLDENROD,
	GREEENYELLOW,
	HOTPINK,
	INDIANRED,
	INDIGO,
	LAWNGREEN,
	MAROON,
	MIDNIGHTBLUE,
	ORANGE,
	ORANGERED,
	ORCHID,
	PALEVIOLETRED,
	PURPLE,
	ROYALBLUE,
	SEAGREEN,
	SPRINGGREEN,
	STEELBLUE,
	TEAL,
	TURQUOISE,
	VIOLET,
	COLOR_COUNT = VIOLET
};

enum eColorChange : uint8_t {
	COLORCHANGE_NO,
	COLORCHANGE_FIVE,
	COLORCHANGE_HOUR,
	COLORCHANGE_DAY,
	COLORCHANGE_COUNT = COLORCHANGE_DAY
};

enum eTransition : uint8_t {
	TRANSITION_NORMAL,
	TRANSITION_FADE,
	TRANSITION_COUNT = TRANSITION_FADE
};

enum eTextPos : uint8_t {
	TEXT_POS_TOP,
	TEXT_POS_MIDDLE = 2,
	TEXT_POS_BOTTOM = 5
};

enum eLanguage : uint8_t {
	LANGUAGE_DE_DE,
	LANGUAGE_DE_SW,
	LANGUAGE_DE_BA,
	LANGUAGE_DE_SA,
	LANGUAGE_DE_MKF_DE,
	LANGUAGE_DE_MKF_SW,
	LANGUAGE_DE_MKF_BA,
	LANGUAGE_DE_MKF_SA,
	LANGUAGE_D3,
	LANGUAGE_CH,
	LANGUAGE_CH_GS,
	LANGUAGE_EN,
	LANGUAGE_ES,
	LANGUAGE_FR,
	LANGUAGE_IT,
	LANGUAGE_NL,
	LANGUAGE_COUNT = LANGUAGE_NL
};

#endif