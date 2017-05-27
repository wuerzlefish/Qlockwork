/******************************************************************************
   Renderer.h
******************************************************************************/

#ifndef RENDERER_H
#define RENDERER_H

#include <Arduino.h>
#include "Words_DE.h"
#include "Words_DE_MKF.h"
#include "Words_D3.h"
#include "Words_CH.h"
#include "Words_EN.h"
#include "Words_ES.h"
#include "Words_FR.h"
#include "Words_IT.h"
#include "Words_NL.h"
#include "Languages.h"
#include "Letters.h"
#include "Numbers.h"

class Renderer {

public:
	enum eTextPos : uint8_t {
		TEXT_POS_TOP,
		TEXT_POS_MIDDLE = 2,
		TEXT_POS_BOTTOM = 5
	};
	Renderer();
	void setHours(uint8_t hours, boolean glatt, uint8_t language, word matrix[16]);
	void setTime(uint8_t hours, uint8_t minutes, uint8_t language, word matrix[16]);
	void setCorners(uint8_t minutes, word matrix[16]);
	void activateAlarmLed(word matrix[16]);
	void deactivateAlarmLed(word matrix[16]);
	void clearEntryWords(uint8_t language, word matrix[16]);
	void setSmallText(String menuText, eTextPos textPos, word matrix[16]);
	void setAMPM(uint8_t hours, uint8_t language, word matrix[16]);
	void setPixelInScreenBuffer(uint8_t x, uint8_t y, word matrix[16]);
	void unsetPixelInScreenBuffer(uint8_t x, uint8_t y, word matrix[16]);
	void clearScreenBuffer(word matrix[16]);
	void setAllScreenBuffer(word matrix[16]);

private:
	boolean isNumber(char symbol);
	void FR_hours(uint8_t hours, word matrix[16]);
	void IT_hours(uint8_t hours, word matrix[16]);
	void ES_hours(uint8_t hours, word matrix[16]);
};

#endif
