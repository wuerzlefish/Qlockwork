/******************************************************************************
Languages.h
******************************************************************************/

#ifndef LANGUAGES_H
#define LANGUAGES_H

//enum eLanguage : uint8_t
//{
//	LANGUAGE_DE_DE,
//	LANGUAGE_DE_SW,
//	LANGUAGE_DE_BA,
//	LANGUAGE_DE_SA,
//	LANGUAGE_DE_MKF_DE,
//	LANGUAGE_DE_MKF_SW,
//	LANGUAGE_DE_MKF_BA,
//	LANGUAGE_DE_MKF_SA,
//	LANGUAGE_D3,
//	LANGUAGE_CH,
//	LANGUAGE_CH_GS,
//	LANGUAGE_EN,
//	LANGUAGE_ES,
//	LANGUAGE_FR,
//	LANGUAGE_IT,
//	LANGUAGE_NL,
//	LANGUAGE_COUNT = LANGUAGE_NL
//};

//const char sLanguage[][4] =
//{
//	{ 'D', 'E', ' ', ' ' }, // 00
//	{ 'D', 'E', 'S', 'W' }, // 01
//	{ 'D', 'E', 'B', 'A' }, // 02
//	{ 'D', 'E', 'S', 'A' }, // 03
//	{ 'M', 'K', 'D', 'E' }, // 04
//	{ 'M', 'K', 'S', 'W' }, // 05
//	{ 'M', 'K', 'B', 'A' }, // 06
//	{ 'M', 'K', 'S', 'A' }, // 07
//	{ 'D', '3', ' ', ' ' }, // 08
//	{ 'C', 'H', ' ', ' ' }, // 09
//	{ 'C', 'H', 'G', 'S' }, // 10
//	{ 'E', 'N', ' ', ' ' }, // 11
//	{ 'E', 'S', ' ', ' ' }, // 12
//	{ 'F', 'R', ' ', ' ' }, // 13
//	{ 'I', 'T', ' ', ' ' }, // 14
//	{ 'N', 'L', ' ', ' ' }  // 15
//};

#if defined(FRONTCOVER_EN) || defined(FRONTCOVER_BINARY)
const char sWeekday[][2] =
{
	{ ' ', ' ' }, // 00
	{ 'S', 'U' }, // 01
	{ 'M', 'O' }, // 02
	{ 'T', 'U' }, // 03
	{ 'W', 'E' }, // 04
	{ 'T', 'H' }, // 05
	{ 'F', 'R' }, // 06
	{ 'S', 'A' }  // 07
};

const String sWeatherCondition[] =
{
	{ "tornado" }, // 00
	{ "tropical storm" }, // 01
	{ "hurricane" }, // 02
	{ "severe thunderstorms" }, // 03
	{ "thunderstorms" }, // 04
	{ "mixed rain and snow" }, // 05
	{ "mixed rain and sleet" }, // 06
	{ "mixed snow and sleet" }, // 07
	{ "freezing drizzle" }, // 08
	{ "drizzle" }, // 09
	{ "freezing rain" }, // 10
	{ "showers" }, // 11
	{ "showers" }, // 12
	{ "snow flurries" }, // 13
	{ "light snow showers" }, // 14
	{ "blowing snow" }, // 15
	{ "snow" }, // 16
	{ "hail" }, // 17
	{ "sleet" }, // 18
	{ "dust" }, // 19
	{ "foggy" }, // 20
	{ "haze" }, // 21
	{ "smoky" }, // 22
	{ "blustery" }, // 23
	{ "windy" }, // 24
	{ "cold" }, // 25
	{ "cloudy" }, // 26
	{ "mostly cloudy" }, // 27
	{ "mostly cloudy" }, // 28
	{ "partly cloudy" }, // 29
	{ "partly cloudy" }, // 30
	{ "clear" }, // 31
	{ "sunny" }, // 32
	{ "fair" }, // 33
	{ "fair" }, // 34
	{ "mixed rain and hail" }, // 35
	{ "hot" }, // 36
	{ "isolated thunderstorms" }, // 37
	{ "scattered thunderstorms" }, // 38
	{ "scattered thunderstorms" }, // 39
	{ "scattered showers" }, // 40
	{ "heavy snow" }, // 41
	{ "scattered snow showers" }, // 42
	{ "heavy snow" }, // 43
	{ "partly cloudy" }, // 44
	{ "thundershowers" }, // 45
	{ "snow showers" }, // 46
	{ "isolated thundershowers" }  // 47
};
#endif

#if defined(FRONTCOVER_DE_DE) || defined(FRONTCOVER_DE_SW) || defined(FRONTCOVER_DE_BA) || defined(FRONTCOVER_DE_SA) || defined(FRONTCOVER_D3) || defined(FRONTCOVER_DE_MKF_DE) || defined(FRONTCOVER_DE_MKF_SW) || defined(FRONTCOVER_DE_MKF_BA) || defined(FRONTCOVER_DE_MKF_SA) || defined(FRONTCOVER_CH) || defined(FRONTCOVER_CH_GS)
const char sWeekday[][2] =
{
	{ ' ', ' ' }, // 00
	{ 'S', 'O' }, // 01
	{ 'M', 'O' }, // 02
	{ 'D', 'I' }, // 03
	{ 'M', 'I' }, // 04
	{ 'D', 'O' }, // 05
	{ 'F', 'R' }, // 06
	{ 'S', 'A' }  // 07
};

const String sWeatherCondition[] =
{
	{ "Tornado" }, // 00
	{ "Tropischer Sturm" }, // 01
	{ "Hurrikan" }, // 02
	{ "schwere Gewitter" }, // 03
	{ "Gewitter" }, // 04
	{ "gemischter Regen und Schnee" }, // 05
	{ "gemischter Regen und Graupel" }, // 06
	{ "gemischter Schnee und Graupel" }, // 07
	{ "gefrierender Nieselregen" }, // 08
	{ "Nieselregen" }, // 09
	{ "gefrierender Regen" }, // 10
	{ "Schauer" }, // 11
	{ "Schauer" }, // 12
	//{ "Schnee schw&auml;rmen" }, // 13
	{ "Schnee schwaermen" }, // 13
	{ "leichte Schneeschauer" }, // 14
	{ "Schnee schneiden" }, // 15
	{ "Schnee" }, // 16
	{ "Hagel" }, // 17
	{ "Schneeregen" }, // 18
	{ "Nebel" }, // 19
	{ "nebelig" }, // 20
	{ "Dunst" }, // 21
	{ "dunstig" }, // 22
	//{ "st&uuml;rmisch" }, // 23
	{ "stuermisch" }, // 23
	{ "windig" }, // 24
	{ "kalt" }, // 25
	{ "wolkig" }, // 26
	//{ "meist bew&ouml;lkt" }, // 27
	//{ "meist bew&ouml;lkt" }, // 28
	{ "meist bewoelkt" }, // 27
	{ "meist bewoelkt" }, // 28
	{ "teilweise wolkig" }, // 29
	{ "teilweise wolkig" }, // 30
	{ "klar" }, // 31
	{ "sonnig" }, // 32
	//{ "sch&ouml;n" }, // 33
	//{ "sch&ouml;n" }, // 34
	{ "schoen" }, // 33
	{ "schoen" }, // 34
	{ "Regen und Hagel" }, // 35
	{ "heiss" }, // 36
	{ "vereinzelte Gewitter" }, // 37
	{ "vereinzelte Gewitter" }, // 38
	{ "vereinzelte Gewitter" }, // 39
	{ "vereinzelte Regenschauer" }, // 40
	{ "starker Schneefall" }, // 41
	{ "vereinzelte Schneeschauer" }, // 42
	{ "starker Schneefall" }, // 43
	{ "teilweise wolkig" }, // 44
	{ "Gewitterschauer" }, // 45
	{ "Schneeschauer" }, // 46
	{ "vereinzelte Gewitterschauer" }  // 47
};
#endif

#if defined(FRONTCOVER_ES)
const char sWeekday[][2] =
{
	{ ' ', ' ' }, // 00
	{ 'D', 'O' }, // 01
	{ 'L', 'U' }, // 02
	{ 'M', 'A' }, // 03
	{ 'M', 'I' }, // 04
	{ 'J', 'U' }, // 05
	{ 'V', 'I' }, // 06
	{ 'S', 'A' }  // 07
};

const String sWeatherCondition[] =
{
	{ "tornado" }, // 00
	{ "tropical storm" }, // 01
	{ "hurricane" }, // 02
	{ "severe thunderstorms" }, // 03
	{ "thunderstorms" }, // 04
	{ "mixed rain and snow" }, // 05
	{ "mixed rain and sleet" }, // 06
	{ "mixed snow and sleet" }, // 07
	{ "freezing drizzle" }, // 08
	{ "drizzle" }, // 09
	{ "freezing rain" }, // 10
	{ "showers" }, // 11
	{ "showers" }, // 12
	{ "snow flurries" }, // 13
	{ "light snow showers" }, // 14
	{ "blowing snow" }, // 15
	{ "snow" }, // 16
	{ "hail" }, // 17
	{ "sleet" }, // 18
	{ "dust" }, // 19
	{ "foggy" }, // 20
	{ "haze" }, // 21
	{ "smoky" }, // 22
	{ "blustery" }, // 23
	{ "windy" }, // 24
	{ "cold" }, // 25
	{ "cloudy" }, // 26
	{ "mostly cloudy (night)" }, // 27
	{ "mostly cloudy (day)" }, // 28
	{ "partly cloudy (night)" }, // 29
	{ "partly cloudy (day)" }, // 30
	{ "clear (night)" }, // 31
	{ "sunny" }, // 32
	{ "fair (night)" }, // 33
	{ "fair (day)" }, // 34
	{ "mixed rain and hail" }, // 35
	{ "hot" }, // 36
	{ "isolated thunderstorms" }, // 37
	{ "scattered thunderstorms" }, // 38
	{ "scattered thunderstorms" }, // 39
	{ "scattered showers" }, // 40
	{ "heavy snow" }, // 41
	{ "scattered snow showers" }, // 42
	{ "heavy snow" }, // 43
	{ "partly cloudy" }, // 44
	{ "thundershowers" }, // 45
	{ "snow showers" }, // 46
	{ "isolated thundershowers" }  // 47
};
#endif

#if defined(FRONTCOVER_FR)
const char sWeekday[][2] =
{
	{ ' ', ' ' }, // 00
	{ 'D', 'I' }, // 01
	{ 'L', 'U' }, // 02
	{ 'M', 'A' }, // 03
	{ 'M', 'E' }, // 04
	{ 'J', 'E' }, // 05
	{ 'V', 'E' }, // 06
	{ 'S', 'A' }  // 07
};

const String sWeatherCondition[] =
{
	{ "tornado" }, // 00
	{ "tropical storm" }, // 01
	{ "hurricane" }, // 02
	{ "severe thunderstorms" }, // 03
	{ "thunderstorms" }, // 04
	{ "mixed rain and snow" }, // 05
	{ "mixed rain and sleet" }, // 06
	{ "mixed snow and sleet" }, // 07
	{ "freezing drizzle" }, // 08
	{ "drizzle" }, // 09
	{ "freezing rain" }, // 10
	{ "showers" }, // 11
	{ "showers" }, // 12
	{ "snow flurries" }, // 13
	{ "light snow showers" }, // 14
	{ "blowing snow" }, // 15
	{ "snow" }, // 16
	{ "hail" }, // 17
	{ "sleet" }, // 18
	{ "dust" }, // 19
	{ "foggy" }, // 20
	{ "haze" }, // 21
	{ "smoky" }, // 22
	{ "blustery" }, // 23
	{ "windy" }, // 24
	{ "cold" }, // 25
	{ "cloudy" }, // 26
	{ "mostly cloudy (night)" }, // 27
	{ "mostly cloudy (day)" }, // 28
	{ "partly cloudy (night)" }, // 29
	{ "partly cloudy (day)" }, // 30
	{ "clear (night)" }, // 31
	{ "sunny" }, // 32
	{ "fair (night)" }, // 33
	{ "fair (day)" }, // 34
	{ "mixed rain and hail" }, // 35
	{ "hot" }, // 36
	{ "isolated thunderstorms" }, // 37
	{ "scattered thunderstorms" }, // 38
	{ "scattered thunderstorms" }, // 39
	{ "scattered showers" }, // 40
	{ "heavy snow" }, // 41
	{ "scattered snow showers" }, // 42
	{ "heavy snow" }, // 43
	{ "partly cloudy" }, // 44
	{ "thundershowers" }, // 45
	{ "snow showers" }, // 46
	{ "isolated thundershowers" }  // 47
};
#endif

#if defined(FRONTCOVER_IT)
const char sWeekday[][2] =
{
	{ ' ', ' ' }, // 00
	{ 'D', 'O' }, // 01
	{ 'L', 'U' }, // 02
	{ 'M', 'A' }, // 03
	{ 'M', 'E' }, // 04
	{ 'G', 'I' }, // 05
	{ 'V', 'E' }, // 06
	{ 'S', 'A' }  // 07
};

const String sWeatherCondition[] =
{
	{ "tornado" }, // 00
	{ "tropical storm" }, // 01
	{ "hurricane" }, // 02
	{ "severe thunderstorms" }, // 03
	{ "thunderstorms" }, // 04
	{ "mixed rain and snow" }, // 05
	{ "mixed rain and sleet" }, // 06
	{ "mixed snow and sleet" }, // 07
	{ "freezing drizzle" }, // 08
	{ "drizzle" }, // 09
	{ "freezing rain" }, // 10
	{ "showers" }, // 11
	{ "showers" }, // 12
	{ "snow flurries" }, // 13
	{ "light snow showers" }, // 14
	{ "blowing snow" }, // 15
	{ "snow" }, // 16
	{ "hail" }, // 17
	{ "sleet" }, // 18
	{ "dust" }, // 19
	{ "foggy" }, // 20
	{ "haze" }, // 21
	{ "smoky" }, // 22
	{ "blustery" }, // 23
	{ "windy" }, // 24
	{ "cold" }, // 25
	{ "cloudy" }, // 26
	{ "mostly cloudy (night)" }, // 27
	{ "mostly cloudy (day)" }, // 28
	{ "partly cloudy (night)" }, // 29
	{ "partly cloudy (day)" }, // 30
	{ "clear (night)" }, // 31
	{ "sunny" }, // 32
	{ "fair (night)" }, // 33
	{ "fair (day)" }, // 34
	{ "mixed rain and hail" }, // 35
	{ "hot" }, // 36
	{ "isolated thunderstorms" }, // 37
	{ "scattered thunderstorms" }, // 38
	{ "scattered thunderstorms" }, // 39
	{ "scattered showers" }, // 40
	{ "heavy snow" }, // 41
	{ "scattered snow showers" }, // 42
	{ "heavy snow" }, // 43
	{ "partly cloudy" }, // 44
	{ "thundershowers" }, // 45
	{ "snow showers" }, // 46
	{ "isolated thundershowers" }  // 47
};
#endif

#if defined(FRONTCOVER_NL)
const char sWeekday[][2] =
{
	{ ' ', ' ' }, // 00
	{ 'Z', 'O' }, // 01
	{ 'M', 'A' }, // 02
	{ 'D', 'I' }, // 03
	{ 'W', 'O' }, // 04
	{ 'D', 'O' }, // 05
	{ 'V', 'R' }, // 06
	{ 'Z', 'A' }  // 07
};

const String sWeatherCondition[] =
{
	{ "tornado" }, // 00
	{ "tropical storm" }, // 01
	{ "hurricane" }, // 02
	{ "severe thunderstorms" }, // 03
	{ "thunderstorms" }, // 04
	{ "mixed rain and snow" }, // 05
	{ "mixed rain and sleet" }, // 06
	{ "mixed snow and sleet" }, // 07
	{ "freezing drizzle" }, // 08
	{ "drizzle" }, // 09
	{ "freezing rain" }, // 10
	{ "showers" }, // 11
	{ "showers" }, // 12
	{ "snow flurries" }, // 13
	{ "light snow showers" }, // 14
	{ "blowing snow" }, // 15
	{ "snow" }, // 16
	{ "hail" }, // 17
	{ "sleet" }, // 18
	{ "dust" }, // 19
	{ "foggy" }, // 20
	{ "haze" }, // 21
	{ "smoky" }, // 22
	{ "blustery" }, // 23
	{ "windy" }, // 24
	{ "cold" }, // 25
	{ "cloudy" }, // 26
	{ "mostly cloudy (night)" }, // 27
	{ "mostly cloudy (day)" }, // 28
	{ "partly cloudy (night)" }, // 29
	{ "partly cloudy (day)" }, // 30
	{ "clear (night)" }, // 31
	{ "sunny" }, // 32
	{ "fair (night)" }, // 33
	{ "fair (day)" }, // 34
	{ "mixed rain and hail" }, // 35
	{ "hot" }, // 36
	{ "isolated thunderstorms" }, // 37
	{ "scattered thunderstorms" }, // 38
	{ "scattered thunderstorms" }, // 39
	{ "scattered showers" }, // 40
	{ "heavy snow" }, // 41
	{ "scattered snow showers" }, // 42
	{ "heavy snow" }, // 43
	{ "partly cloudy" }, // 44
	{ "thundershowers" }, // 45
	{ "snow showers" }, // 46
	{ "isolated thundershowers" }  // 47
};
#endif

#endif
