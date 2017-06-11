/******************************************************************************
   Languages.h
******************************************************************************/

#ifndef LANGUAGES_H
#define LANGUAGES_H

const char sWeekday[][2] = {
	{ ' ', ' ' }, // 00
	{ 'S', 'U' }, // 01
	{ 'M', 'O' }, // 02
	{ 'T', 'U' }, // 03
	{ 'W', 'E' }, // 04
	{ 'T', 'H' }, // 05
	{ 'F', 'R' }, // 06
	{ 'S', 'A' }  // 07
};

const char sLanguage[][4] = {
	{ 'D', 'E', ' ', ' ' }, // 00
	{ 'D', 'E', 'S', 'W' }, // 01
	{ 'D', 'E', 'B', 'A' }, // 02
	{ 'D', 'E', 'S', 'A' }, // 03
	{ 'M', 'K', 'D', 'E' }, // 04
	{ 'M', 'K', 'S', 'W' }, // 05
	{ 'M', 'K', 'B', 'A' }, // 06
	{ 'M', 'K', 'S', 'A' }, // 07
	{ 'D', '3', ' ', ' ' }, // 08
	{ 'C', 'H', ' ', ' ' }, // 09
	{ 'C', 'H', 'G', 'S' }, // 10
	{ 'E', 'N', ' ', ' ' }, // 11
	{ 'E', 'S', ' ', ' ' }, // 12
	{ 'F', 'R', ' ', ' ' }, // 13
	{ 'I', 'T', ' ', ' ' }, // 14
	{ 'N', 'L', ' ', ' ' }  // 15
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
	LANGUAGE_COUNT
};

#endif
