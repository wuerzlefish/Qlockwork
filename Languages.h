/******************************************************************************
   Languages.h
******************************************************************************/

#ifndef LANGUAGES_H
#define LANGUAGES_H

const String sLanguage[] = {
  "DE",   // 00
  "DESW", // 01
  "DEBA", // 02
  "DESA", // 03
  "MKDE", // 04
  "MKSW", // 05
  "MKBA", // 06
  "MKSA", // 07
  "D3",   // 08
  "CH",   // 09
  "CHGS", // 10
  "EN",   // 11
  "ES",   // 12
  "FR",   // 13
  "IT",   // 14
  "NL"    // 15
};

const String sWeekday[][8] = {
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // DE
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // DESW
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // DEBA
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // DESA
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // MKDE
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // MKSW
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // MKBA
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // MKSA
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // D3
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // CH
{ "", "SO", "MO", "DI", "MI", "DO", "FR", "SA" }, // CHGS
{ "", "SU", "MO", "TU", "WE", "TH", "FR", "SA" }, // EN
{ "", "DO", "LU", "MA", "MI", "JU", "VI", "SA" }, // ES
{ "", "DI", "LU", "MA", "ME", "JE", "VE", "SA" }, // FR
{ "", "DO", "LU", "MA", "ME", "GI", "VE", "SA" }, // IT
{ "", "ZO", "MA", "DI", "WO", "DO", "VR", "ZA" }  // NL
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
