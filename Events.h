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
	eColor color;
};

String textBirthday = "s birthday!";

const event_t event[] =
{
	{ 1,  1, "Happy New Year!", YELLOW_25 },
	{ 3, 14, "Albert Einstein" + textBirthday, MAGENTA },
	{ 12, 24, "It's Christmas!", RED },
	{ 2, 1, "Qlockwork" + textBirthday, MAGENTA }
};

#endif
