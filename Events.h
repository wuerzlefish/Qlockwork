/******************************************************************************
Events.h
******************************************************************************/

#ifndef EVENTS_H
#define EVENTS_H

struct event_t
{
	uint8_t month;
	uint8_t day;
	String text;
	uint16_t year;
	eColor color;
};

String textBirthday = "s birthday!";

const event_t event[] =
{
	{ 1,  1, "Happy New Year!", 0, YELLOW_25 },
	{ 3, 14, "Albert Einstein" + textBirthday, 1879, MAGENTA },
	{ 12, 24, "It's Christmas!", 0, RED },
	{ 2, 1, "Qlockwork" + textBirthday, 2017, MAGENTA }
};

#endif
