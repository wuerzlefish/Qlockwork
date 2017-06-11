/******************************************************************************
QLOCKWORK
Eine Firmware der Selbstbau-QLOCKTWO.

@mc      ESP8266
@created 01.02.2017
******************************************************************************/

#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <DS3232RTC.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <RestClient.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <WiFiManager.h>
#include "Configuration.h"
#include "Colors.h"
#include "Debug.h"
#include "Languages.h"
#include "LedDriver_FastLED.h"
#include "LedDriver_NeoPixel.h"
#include "LedDriver_LPD8806RGBW.h"
#include "Modes.h"
#include "Renderer.h"
#include "Settings.h"
#include "Timezones.h"

/******************************************************************************
init
******************************************************************************/

#ifdef LED_LIBRARY_FASTLED
LedDriver_FastLED ledDriver;
#endif
#ifdef LED_LIBRARY_LPD8806RGBW
LedDriver_LPD8806RGBW ledDriver;
#endif
#ifdef LED_LIBRARY_NEOPIXEL
LedDriver_NeoPixel ledDriver;
#endif

ESP8266WebServer esp8266WebServer(80);
Renderer renderer;
Settings settings;
Debug debug;

uint16_t matrix[10] = {};
uint16_t matrixOld[10] = {};
boolean screenBufferNeedsUpdate = true;
Mode mode = STD_MODE_NORMAL;
Mode lastMode = mode;
uint8_t fallBackCounter = 0;
uint8_t lastDay = 0;
uint8_t lastHour = 0;
uint8_t lastMinute = 0;
uint8_t lastFiveMinute = 0;
time_t lastTime = 0;
uint8_t testColumn = 0;
uint8_t brightness = settings.getBrightness();
uint8_t randomMinute;
int8_t yahooTemp;
uint8_t yahooCode;
#ifdef BUZZER
boolean timerSet = false;
time_t timer = 0;
uint8_t timerMinutes = 0;
uint8_t alarmOn = 0;
#endif
#ifdef LDR
uint8_t ratedBrightness = settings.getBrightness();
uint16_t ldrValue = 0;
uint16_t lastLdrValue = 0;
uint16_t minLdrValue = 1023;
uint16_t maxLdrValue = 0;
uint32_t lastLdrCheck = 0;
uint32_t lastBrightnessCheck = 0;
#endif
#ifdef IR_REMOTE
IRrecv irrecv(PIN_IR_RECEIVER);
decode_results irDecodeResult;
irparams_t irDecodeSave;
#endif

/******************************************************************************
setup()
******************************************************************************/

void setup() {
	// init serial port
	Serial.begin(SERIAL_SPEED);
	while (!Serial);
	DEBUG_PRINTLN();
	DEBUG_PRINTLN("QLOCKWORK");
	DEBUG_PRINTLN(String("Firmware: " + String(FIRMWARE_VERSION)));
	DEBUG_PRINTLN(String("LED-Driver: " + ledDriver.getSignature()));
	// init LDR, Buzzer and LED
#ifdef BUZZER
	DEBUG_PRINTLN("Setting up Buzzer.");
	pinMode(PIN_BUZZER, OUTPUT);
#endif
#ifdef LDR
	DEBUG_PRINTLN("Setting up LDR.");
	pinMode(PIN_LDR, INPUT);
#endif
#ifdef ESP_LED
	DEBUG_PRINTLN("Setting up ESP LED.");
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, HIGH);
#endif
	// init WiFi and services
	matrix[0] = 0b0000000000010000;
	matrix[1] = 0b0000111000010000;
	matrix[2] = 0b0011111110010000;
	matrix[3] = 0b1111111111110000;
	matrix[4] = 0b0111111111000000;
	matrix[5] = 0b0011111110000000;
	matrix[6] = 0b0001111100000000;
	matrix[7] = 0b0000111000000000;
	matrix[8] = 0b0000010000000000;
	matrix[9] = 0b0000000000000000;
	writeScreenBuffer(matrix, WHITE, brightness);
	WiFiManager wifiManager;
	//wifiManager.resetSettings();
	wifiManager.setTimeout(WIFI_AP_TIMEOUT);
	wifiManager.autoConnect(HOSTNAME);
	if (WiFi.status() != WL_CONNECTED) {
		DEBUG_PRINTLN("Error connecting to WiFi. Shutting down WiFi.");
		renderer.clearScreenBuffer(matrix);
		renderer.setSmallText("ER", Renderer::TEXT_POS_TOP, matrix);
		renderer.setSmallText("OR", Renderer::TEXT_POS_BOTTOM, matrix);
		writeScreenBuffer(matrix, RED, brightness);
		WiFi.mode(WIFI_OFF);
		digitalWrite(PIN_BUZZER, HIGH);
		delay(1500);
		digitalWrite(PIN_BUZZER, LOW);
	}
	else {
		renderer.clearScreenBuffer(matrix);
		renderer.setSmallText("OK", Renderer::TEXT_POS_MIDDLE, matrix);
		writeScreenBuffer(matrix, GREEN, brightness);
		for (uint8_t i = 0; i <= 2; i++) {
			digitalWrite(PIN_BUZZER, HIGH);
			delay(100);
			digitalWrite(PIN_BUZZER, LOW);
			delay(100);
		}
	}
	renderer.clearScreenBuffer(matrix);
	DEBUG_PRINTLN("Starting mDNS responder.");
	MDNS.begin(HOSTNAME);
	DEBUG_PRINTLN("Starting webserver on port 80.");
	setupWebServer();
	DEBUG_PRINTLN("Starting Arduino-OTA service.");
	char otaPassword[] = OTA_PASS;
	ArduinoOTA.setPassword(otaPassword);
	ArduinoOTA.begin();
#ifdef IR_REMOTE
	DEBUG_PRINTLN("Starting IR-receiver.");
	irrecv.enableIRIn();
#endif
	DEBUG_PRINT("Use LDR: ");
	if (settings.getUseLdr()) DEBUG_PRINTLN("On"); else DEBUG_PRINTLN("Off");
	randomSeed(analogRead(PIN_LDR));
	randomMinute = random(1, 60);
	DEBUG_PRINTLN("Random minute is: " + String(randomMinute));
	// set timeprovider
#ifdef RTC_BACKUP
	if (WiFi.status() == WL_CONNECTED) {
		DEBUG_PRINTLN("Setting ESP from NTP with RTC backup.");
		setSyncProvider(getNtpTime);
		setSyncInterval(3600);
	}
	else {
		DEBUG_PRINTLN("Setting ESP from RTC.");
		setSyncProvider(getRtcTime);
		setSyncInterval(3600);
	}
#else
	if (WiFi.status() == WL_CONNECTED) {
		DEBUG_PRINTLN("Setting ESP from NTP.");
		setSyncProvider(getNtpTime);
		setSyncInterval(3600);
	}
	else DEBUG_PRINTLN("No provider for setting the time found.");
#endif
	lastDay = day();
	lastHour = hour();
	lastFiveMinute = minute() / 5;
	lastMinute = minute();
	lastTime = now();
	getYahooWeather(YAHOO_LOCATION);
	DEBUG_PRINT("Free RAM: ");
	DEBUG_PRINTLN(system_get_free_heap_size());
}

/******************************************************************************
loop()
******************************************************************************/

void loop() {

#ifdef DEBUG_FPS
	debug.debugFps();
#endif

	// execute every day

	if (day() != lastDay) {
		lastDay = day();
		DEBUG_PRINTLN("Reached a new day.");
		//if (settings.getColor() < maxColor) settings.setColor(settings.getColor() + 1);
		//else settings.setColor(0);
	}

	// execute every hour

	if (hour() != lastHour) {
		lastHour = hour();
		DEBUG_PRINTLN("Reached a new hour.");
		DEBUG_PRINT("Free RAM: ");
		DEBUG_PRINTLN(system_get_free_heap_size());
	}

	// execute every five minutes

	if ((minute() / 5) != lastFiveMinute) {
		lastFiveMinute = minute() / 5;
		DEBUG_PRINTLN("Reached new five minutes.");
	}

	// execute every minute

	if (minute() != lastMinute) {
		lastMinute = minute();
#ifdef DEBUG
		debug.debugTime("Time:", now());
#endif
		if ((minute() / float(randomMinute)) == 1.0) getYahooWeather(YAHOO_LOCATION);
		// display needs update every minute
		switch (mode) {
		case STD_MODE_NORMAL:
		case STD_MODE_EXT_TEMP:
			screenBufferNeedsUpdate = true;
			break;
		default:
			break;
		}
	}

	// execute every second

	if (now() != lastTime) {
		lastTime = now();
		// countdown fallback
		if (fallBackCounter > 1) fallBackCounter--;
		else if (fallBackCounter == 1) {
			fallBackCounter = 0;
			setMode(STD_MODE_NORMAL);
		}
		// display needs update every second
		switch (mode) {
		case STD_MODE_SECONDS:
#ifdef RTC_BACKUP
		case STD_MODE_TEMP:
#endif
#ifdef BUZZER
		case STD_MODE_SET_TIMER:
		case STD_MODE_TIMER:
		case STD_MODE_ALARM_1:
		case STD_MODE_SET_ALARM_1:
		case STD_MODE_ALARM_2:
		case STD_MODE_SET_ALARM_2:
#endif
#ifdef LDR
		case EXT_MODE_LDR:
#endif
		case EXT_MODE_COLOR:
		case EXT_MODE_TRANSITION:
		case EXT_MODE_TIMEOUT:
		case EXT_MODE_LANGUAGE:
		case EXT_MODE_TIMESET:
		case EXT_MODE_IT_IS:
		case EXT_MODE_DAYSET:
		case EXT_MODE_MONTHSET:
		case EXT_MODE_YEARSET:
		case EXT_MODE_NIGHTOFF:
		case EXT_MODE_DAYON:
		case EXT_MODE_TEST:
			screenBufferNeedsUpdate = true;
			break;
		default:
			break;
		}
#ifdef ESP_LED
		if (digitalRead(PIN_LED) == LOW) digitalWrite(PIN_LED, HIGH);
		else digitalWrite(PIN_LED, LOW);
#endif
#ifdef BUZZER
		// alarm
		if (settings.getAlarm1() && (hour() == hour(settings.getAlarmTime1())) && (minute() == minute(settings.getAlarmTime1())) && (second() == 0)) alarmOn = BUZZTIME_ALARM_1;
		if (settings.getAlarm2() && (hour() == hour(settings.getAlarmTime2())) && (minute() == minute(settings.getAlarmTime2())) && (second() == 0)) alarmOn = BUZZTIME_ALARM_2;

		// timer
		if (timerSet && (now() == timer)) {
			setMode(STD_MODE_SET_TIMER);
			timerMinutes = 0;
			timerSet = false;
			alarmOn = BUZZTIME_TIMER;
		}
		// make some noise
		if (alarmOn) {
			if (second() % 2 == 0) digitalWrite(PIN_BUZZER, HIGH);
			else digitalWrite(PIN_BUZZER, LOW);
			alarmOn--;
		}
		else digitalWrite(PIN_BUZZER, LOW);
#endif
		// set nightmode/daymode
		if ((hour() == hour(settings.getNightOffTime())) && (minute() == minute(settings.getNightOffTime())) && (second() == 0)) setMode(STD_MODE_BLANK);
		if ((hour() == hour(settings.getDayOnTime())) && (minute() == minute(settings.getDayOnTime())) && (second() == 0)) setMode(lastMode);
	}

	// always execute

	// call HTTP- and OTA-handler
	esp8266WebServer.handleClient();
	ArduinoOTA.handle();
#ifdef LDR
	// get rated brightness from LDR
	if (millis() > (lastLdrCheck + 250)) {
		lastLdrCheck = millis();
		ldrValue = 1023 - analogRead(PIN_LDR);
		if (ldrValue < minLdrValue) minLdrValue = ldrValue;
		if (ldrValue > maxLdrValue) maxLdrValue = ldrValue;
		if (settings.getUseLdr() && ((ldrValue >= (lastLdrValue + LDR_HYSTERESIS)) || (ldrValue <= (lastLdrValue - LDR_HYSTERESIS)))) {
			lastLdrValue = ldrValue;
			ratedBrightness = map(ldrValue, minLdrValue, maxLdrValue + 1, 0, 255); // ESP will crash if min and max are equal
			ratedBrightness = constrain(ratedBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
			DEBUG_PRINTLN("Brightness: " + String(ratedBrightness) + " (LDR: " + String(ldrValue) + ", min: " + String(minLdrValue) + ", max: " + String(maxLdrValue) + ")");
		}
	}
	// set brightness to rated brightness
	if (settings.getUseLdr() && (millis() > (lastBrightnessCheck + 50))) {
		lastBrightnessCheck = millis();
		if (brightness < ratedBrightness) brightness++;
		if (brightness > ratedBrightness) brightness--;
		if (brightness != ratedBrightness) {
			writeScreenBuffer(matrix, settings.getColor(), brightness);
			//DEBUG_PRINTLN("Brightness: " + String(brightness) + ", rated brightness: " + String(ratedBrightness));
		}
	}
#endif
#ifdef IR_REMOTE
	if (irrecv.decode(&irDecodeResult, &irDecodeSave)) {
		DEBUG_PRINTLN("IR signal: " + String(uint32_t(irDecodeResult.value)));
		remoteAction(irDecodeResult);
		irrecv.resume();
	}
#endif
	// render new screenbuffer
	if (screenBufferNeedsUpdate) {
		screenBufferNeedsUpdate = false;
		for (uint8_t i = 0; i <= 9; i++) matrixOld[i] = matrix[i];
		switch (mode) {
		case STD_MODE_NORMAL:
			renderer.clearScreenBuffer(matrix);
			renderer.setTime(hour(), minute(), settings.getLanguage(), matrix);
			renderer.setCorners(minute(), matrix);
			if (settings.getAlarm1() || settings.getAlarm2()) renderer.setAlarmLed(matrix);
			if (!settings.getItIs() && ((minute() / 5) % 6)) renderer.clearEntryWords(settings.getLanguage(), matrix);
			break;
		case STD_MODE_AMPM:
			renderer.clearScreenBuffer(matrix);
			if (isAM()) renderer.setSmallText("AM", Renderer::TEXT_POS_MIDDLE, matrix);
			else renderer.setSmallText("PM", Renderer::TEXT_POS_MIDDLE, matrix);
			break;
		case STD_MODE_SECONDS:
			renderer.clearScreenBuffer(matrix);
			renderer.setCorners(minute(), matrix);
			for (uint8_t i = 0; i <= 6; i++) {
				matrix[1 + i] |= (zahlenGross[second() / 10][i]) << 11;
				matrix[1 + i] |= (zahlenGross[second() % 10][i]) << 5;
			}
			break;
		case STD_MODE_WEEKDAY:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText(String(sWeekday[weekday()][0]) + String(sWeekday[weekday()][1]), Renderer::TEXT_POS_MIDDLE, matrix);
			break;
		case STD_MODE_DATE:
			renderer.clearScreenBuffer(matrix);
			if (day() < 10) renderer.setSmallText(("0" + String(day())), Renderer::TEXT_POS_TOP, matrix);
			else renderer.setSmallText(String(day()), Renderer::TEXT_POS_TOP, matrix);
			if (month() < 10) renderer.setSmallText(("0" + String(month())), Renderer::TEXT_POS_BOTTOM, matrix);
			else renderer.setSmallText(String(month()), Renderer::TEXT_POS_BOTTOM, matrix);
			renderer.setPixelInScreenBuffer(5, 4, matrix);
			renderer.setPixelInScreenBuffer(5, 9, matrix);
			break;
		case STD_MODE_TITLE_TEMP:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TE", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("MP", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
#ifdef RTC_BACKUP
		case STD_MODE_TEMP:
			renderer.clearScreenBuffer(matrix);
			if ((RTC.temperature() / 4 + int(RTC_TEMP_OFFSET)) == 0) {
				matrix[0] = 0b0000000001000000;
				matrix[1] = 0b0000000010100000;
				matrix[2] = 0b0000000010100000;
				matrix[3] = 0b0000000011100000;
			}
			if ((RTC.temperature() / 4 + int(RTC_TEMP_OFFSET)) > 0) {
				matrix[0] = 0b0000000001000000;
				matrix[1] = 0b0100000010100000;
				matrix[2] = 0b1110000010100000;
				matrix[3] = 0b0100000011100000;
			}
			if ((RTC.temperature() / 4 + int(RTC_TEMP_OFFSET)) < 0) {
				matrix[0] = 0b0000000001000000;
				matrix[1] = 0b0000000010100000;
				matrix[2] = 0b1110000010100000;
				matrix[3] = 0b0000000011100000;
			}
			renderer.setSmallText(String(RTC.temperature() / 4 + int(RTC_TEMP_OFFSET)), Renderer::TEXT_POS_BOTTOM, matrix);
			DEBUG_PRINTLN(String(RTC.temperature() / 4.0 + RTC_TEMP_OFFSET)); // .0 to get float values for temp
			break;
#endif
		case STD_MODE_EXT_TEMP:
			renderer.clearScreenBuffer(matrix);
			if (yahooTemp > 0) {
				matrix[1] = 0b0100000000000000;
				matrix[2] = 0b1110000000000000;
				matrix[3] = 0b0100000000000000;
			}
			if (yahooTemp < 0) {
				matrix[2] = 0b1110000000000000;
			}
			renderer.setSmallText(String(yahooTemp), Renderer::TEXT_POS_BOTTOM, matrix);
			DEBUG_PRINTLN(String(yahooTemp));
			break;
#ifdef BUZZER
		case STD_MODE_TITLE_ALARM:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("AL", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("RM", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case STD_MODE_SET_TIMER:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TI", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else renderer.setSmallText(String(timerMinutes), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case STD_MODE_TIMER:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TI", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText(String((timer - now() + 60) / 60), Renderer::TEXT_POS_BOTTOM, matrix);
			DEBUG_PRINTLN(String(timer - now()));
			break;
		case STD_MODE_ALARM_1:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("A1", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else {
				if (settings.getAlarm1()) {
					renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
					renderer.setAlarmLed(matrix);
				}
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case STD_MODE_SET_ALARM_1:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getAlarmTime1()), minute(settings.getAlarmTime1()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getAlarmTime1()), settings.getLanguage(), matrix);
				renderer.setAlarmLed(matrix);
			}
			break;
		case STD_MODE_ALARM_2:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("A2", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else {
				if (settings.getAlarm2()) {
					renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
					renderer.setAlarmLed(matrix);
				}
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case STD_MODE_SET_ALARM_2:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getAlarmTime2()), minute(settings.getAlarmTime2()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getAlarmTime2()), settings.getLanguage(), matrix);
				renderer.setAlarmLed(matrix);
			}
			break;
#endif
		case EXT_MODE_TITLE_MAIN:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("MA", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("IN", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
#ifdef LDR
		case EXT_MODE_LDR:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("LD", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else {
				if (settings.getUseLdr()) renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
#endif
		case EXT_MODE_BRIGHTNESS:
			renderer.clearScreenBuffer(matrix);
			for (uint8_t x = 0; x < map(settings.getBrightness(), 0, 255, 1, 10); x++) {
				for (uint8_t y = 0; y <= x; y++) matrix[9 - y] |= 1 << (14 - x);
			}
			break;
		case EXT_MODE_COLOR:
			renderer.clearScreenBuffer(matrix);
			matrix[0] = 0b0000000000010000;
			matrix[1] = 0b0000000000010000;
			matrix[2] = 0b0000000000010000;
			matrix[3] = 0b0000000000010000;
			matrix[4] = 0b0000000000010000;
			renderer.setSmallText("CO", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else renderer.setSmallText(String(settings.getColor()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TRANSITION:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TR", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else {
				if (settings.getTransition() == Settings::TRANSITION_NORMAL) renderer.setSmallText("NO", Renderer::TEXT_POS_BOTTOM, matrix);
				if (settings.getTransition() == Settings::TRANSITION_FADE) renderer.setSmallText("FD", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_TIMEOUT:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("FB", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else renderer.setSmallText(String(settings.getTimeout()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_LANGUAGE:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				if (sLanguage[settings.getLanguage()][3] == ' ') renderer.setSmallText(String(sLanguage[settings.getLanguage()][0]) + String(sLanguage[settings.getLanguage()][1]), Renderer::TEXT_POS_MIDDLE, matrix);
				else {
					renderer.setSmallText(String(sLanguage[settings.getLanguage()][0]) + String(sLanguage[settings.getLanguage()][1]), Renderer::TEXT_POS_TOP, matrix);
					renderer.setSmallText(String(sLanguage[settings.getLanguage()][2]) + String(sLanguage[settings.getLanguage()][3]), Renderer::TEXT_POS_BOTTOM, matrix);
				}
			}
			break;
		case EXT_MODE_TITLE_TIME:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TI", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("ME", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TIMESET:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(), minute(), settings.getLanguage(), matrix);
				renderer.setCorners(minute(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(), settings.getLanguage(), matrix);
			}
			break;
		case EXT_MODE_IT_IS:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("IT", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else {
				if (settings.getItIs()) renderer.setSmallText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
				else renderer.setSmallText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_DAYSET:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("DD", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else renderer.setSmallText(String(day()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_MONTHSET:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("MM", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else renderer.setSmallText(String(month()), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_YEARSET:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("YY", Renderer::TEXT_POS_TOP, matrix);
			if (second() % 2 == 0) for (uint8_t i = 5; i <= 9; i++) matrix[i] = 0;
			else renderer.setSmallText(String(year() % 100), Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TEXT_NIGHTOFF:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("NT", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("OF", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_NIGHTOFF:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getNightOffTime()), minute(settings.getNightOffTime()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getNightOffTime()), settings.getLanguage(), matrix);
			}
			break;
		case EXT_MODE_TEXT_DAYON:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("DY", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("ON", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_DAYON:
			renderer.clearScreenBuffer(matrix);
			if (second() % 2 == 0) {
				renderer.setTime(hour(settings.getDayOnTime()), minute(settings.getDayOnTime()), settings.getLanguage(), matrix);
				renderer.clearEntryWords(settings.getLanguage(), matrix);
				renderer.setAMPM(hour(settings.getDayOnTime()), settings.getLanguage(), matrix);
			}
			break;
		case EXT_MODE_TITLE_IP:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("IP", Renderer::TEXT_POS_MIDDLE, matrix);
			break;
		case EXT_MODE_IP_0:
			renderer.clearScreenBuffer(matrix);
			if (WiFi.localIP()[0] / 10 == 0) renderer.setSmallText(String(WiFi.localIP()[0] % 10), Renderer::TEXT_POS_MIDDLE, matrix);
			else {
				renderer.setSmallText(String(WiFi.localIP()[0] / 10), Renderer::TEXT_POS_TOP, matrix);
				renderer.setSmallText(String(WiFi.localIP()[0] % 10), Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_IP_1:
			renderer.clearScreenBuffer(matrix);
			if (WiFi.localIP()[1] / 10 == 0) renderer.setSmallText(String(WiFi.localIP()[1] % 10), Renderer::TEXT_POS_MIDDLE, matrix);
			else {
				renderer.setSmallText(String(WiFi.localIP()[1] / 10), Renderer::TEXT_POS_TOP, matrix);
				renderer.setSmallText(String(WiFi.localIP()[1] % 10), Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_IP_2:
			renderer.clearScreenBuffer(matrix);
			if (WiFi.localIP()[2] / 10 == 0) renderer.setSmallText(String(WiFi.localIP()[2] % 10), Renderer::TEXT_POS_MIDDLE, matrix);
			else {
				renderer.setSmallText(String(WiFi.localIP()[2] / 10), Renderer::TEXT_POS_TOP, matrix);
				renderer.setSmallText(String(WiFi.localIP()[2] % 10), Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_IP_3:
			renderer.clearScreenBuffer(matrix);
			if (WiFi.localIP()[3] / 10 == 0) renderer.setSmallText(String(WiFi.localIP()[3] % 10), Renderer::TEXT_POS_MIDDLE, matrix);
			else {
				renderer.setSmallText(String(WiFi.localIP()[3] / 10), Renderer::TEXT_POS_TOP, matrix);
				renderer.setSmallText(String(WiFi.localIP()[3] % 10), Renderer::TEXT_POS_BOTTOM, matrix);
			}
			break;
		case EXT_MODE_TITLE_TEST:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TE", Renderer::TEXT_POS_TOP, matrix);
			renderer.setSmallText("ST", Renderer::TEXT_POS_BOTTOM, matrix);
			break;
		case EXT_MODE_TEST:
			renderer.clearScreenBuffer(matrix);
			if (testColumn == 10) testColumn = 0;
			matrix[testColumn] = 0b1111111111110000;
			testColumn++;
			break;
		case STD_MODE_BLANK:
			renderer.clearScreenBuffer(matrix);
			break;
		default:
			break;
		}
#if defined(IR_LETTER_OFF)
		// turn off LED behind IR-sensor
		renderer.unsetPixelInScreenBuffer(8, 9, matrix);
#endif
#ifdef DEBUG_MATRIX
		// write screenbuffer to console
		debug.debugScreenBuffer(matrix);
#endif
		switch (mode) {
		case STD_MODE_NORMAL:
		case STD_MODE_BLANK:
			if (settings.getTransition() == Settings::TRANSITION_NORMAL) writeScreenBuffer(matrix, settings.getColor(), brightness);
			if (settings.getTransition() == Settings::TRANSITION_FADE) writeScreenBufferFade(matrixOld, matrix, settings.getColor(), brightness);
			break;
		default:
			writeScreenBuffer(matrix, settings.getColor(), brightness);
			break;
		}
	}
}

/******************************************************************************
"mode" pressed
******************************************************************************/

void buttonModePressed() {
#ifdef BUZZER
	// turn off alarm
	if (alarmOn) {
		alarmOn = false;
		digitalWrite(PIN_BUZZER, LOW);
		return;
	}
#endif
	// turn off nightmode
	if (mode == STD_MODE_BLANK) {
		setMode(STD_MODE_NORMAL);
		return;
	}
	// set mode and fallback
	setMode(mode++);
	switch (mode) {
	case STD_MODE_COUNT:
	case EXT_MODE_COUNT:
		setMode(STD_MODE_NORMAL);
		break;
#ifdef BUZZER
	case STD_MODE_SET_TIMER:
		if (timerSet) setMode(mode++);
		fallBackCounter = 0;
		break;
	case STD_MODE_TIMER:
		if (!timerSet) setMode(mode++);
		fallBackCounter = 0;
		break;
	case STD_MODE_SET_ALARM_1:
		if (!settings.getAlarm1()) setMode(STD_MODE_ALARM_2);
		break;
	case STD_MODE_SET_ALARM_2:
		if (!settings.getAlarm2()) setMode(STD_MODE_NORMAL);
		break;
#endif
#ifdef LDR
	case EXT_MODE_BRIGHTNESS:
		if (settings.getUseLdr()) setMode(mode++);
		break;
#endif
	case EXT_MODE_TEST:
		testColumn = 0;
		return;
	case STD_MODE_AMPM:
	case STD_MODE_SECONDS:
	case STD_MODE_WEEKDAY:
	case STD_MODE_DATE:
#ifdef RTC_BACKUP
	case STD_MODE_TEMP:
#endif
	case STD_MODE_EXT_TEMP:
		fallBackCounter = settings.getTimeout();
		break;
	default:
		fallBackCounter = 0;
		break;
	}
#ifdef RTC_BACKUP
	RTC.set(now());
#endif
	settings.saveToEEPROM();
}

/******************************************************************************
"settings" pressed
******************************************************************************/

void buttonSettingsPressed() {
	DEBUG_PRINTLN("Settings pressed.");
	if (mode < EXT_MODE_START) setMode(EXT_MODE_START);
	else buttonModePressed();
}

/******************************************************************************
"+" pressed
******************************************************************************/

void buttonPlusPressed() {
	DEBUG_PRINTLN("+ pressed.");
	screenBufferNeedsUpdate = true;
	switch (mode) {
	case STD_MODE_NORMAL:
		setMode(STD_MODE_TITLE_TEMP);
		break;
#ifndef BUZZER
	case STD_MODE_TITLE_TEMP:
		setMode(STD_MODE_NORMAL);
		break;
#endif
#ifdef BUZZER
	case STD_MODE_TITLE_TEMP:
		setMode(STD_MODE_TITLE_ALARM);
		break;
	case STD_MODE_TITLE_ALARM:
		setMode(STD_MODE_NORMAL);
		break;
	case STD_MODE_SET_TIMER:
		if (timerMinutes < 100) timerMinutes++;
		timer = now() + timerMinutes * 60;
		timerSet = true;
		break;
	case STD_MODE_TIMER:
		timerSet = false;
		setMode(STD_MODE_SET_TIMER);
		break;
	case STD_MODE_ALARM_1:
		settings.toggleAlarm1();
		break;
	case STD_MODE_SET_ALARM_1:
		settings.setAlarmTime1(settings.getAlarmTime1() + 3600);
#ifdef DEBUG
		debug.debugTime("Alarm 1:", settings.getAlarmTime1());
#endif
		break;
	case STD_MODE_ALARM_2:
		settings.toggleAlarm2();
		break;
	case STD_MODE_SET_ALARM_2:
		settings.setAlarmTime2(settings.getAlarmTime2() + 3600);
#ifdef DEBUG
		debug.debugTime("Alarm 2:", settings.getAlarmTime2());
#endif
#endif
		break;
	case EXT_MODE_TITLE_MAIN:
		setMode(EXT_MODE_TITLE_TIME);
		break;
#ifdef LDR
	case EXT_MODE_LDR:
		settings.toggleUseLdr();
		lastLdrValue = 0;
		break;
#endif
	case EXT_MODE_BRIGHTNESS:
		settings.setBrightness(constrain(settings.getBrightness() + 10, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
		brightness = settings.getBrightness();
		writeScreenBuffer(matrix, settings.getColor(), brightness);
		DEBUG_PRINTLN(settings.getBrightness());
		break;
	case EXT_MODE_COLOR:
		if (settings.getColor() < COLOR_COUNT - 1) settings.setColor(settings.getColor() + 1);
		else settings.setColor(0);
		break;
	case EXT_MODE_TRANSITION:
		if (settings.getTransition() < Settings::TRANSITION_COUNT - 1) settings.setTransition(settings.getTransition() + 1);
		else settings.setTransition(0);
		break;
	case EXT_MODE_TIMEOUT:
		if (settings.getTimeout() < 99) settings.setTimeout(settings.getTimeout() + 1);
		break;
	case EXT_MODE_LANGUAGE:
		if (settings.getLanguage() < LANGUAGE_COUNT - 1) settings.setLanguage(settings.getLanguage() + 1);
		else settings.setLanguage(0);
		break;
	case EXT_MODE_TITLE_TIME:
		setMode(EXT_MODE_TITLE_IP);
		break;
	case EXT_MODE_TIMESET:
		setTime(hour() + 1, minute(), second(), day(), month(), year());
#ifdef DEBUG
		debug.debugTime("Time set:", now());
#endif
		break;
	case EXT_MODE_IT_IS:
		settings.toggleItIs();
		break;
	case EXT_MODE_DAYSET:
		setTime(hour(), minute(), second(), day() + 1, month(), year());
		break;
	case EXT_MODE_MONTHSET:
		setTime(hour(), minute(), second(), day(), month() + 1, year());
		break;
	case EXT_MODE_YEARSET:
		setTime(hour(), minute(), second(), day(), month(), year() + 1);
		break;
	case EXT_MODE_NIGHTOFF:
		settings.setNightOffTime(settings.getNightOffTime() + 3600);
#ifdef DEBUG
		debug.debugTime("Night off:", settings.getNightOffTime());
#endif
		break;
	case EXT_MODE_DAYON:
		settings.setDayOnTime(settings.getDayOnTime() + 3600);
#ifdef DEBUG
		debug.debugTime("Night on:", settings.getDayOnTime());
#endif
		break;
	case EXT_MODE_TITLE_IP:
		setMode(EXT_MODE_TITLE_TEST);
		break;
	case EXT_MODE_TITLE_TEST:
		setMode(EXT_MODE_TITLE_MAIN);
		break;
	default:
		break;
	}
}

/******************************************************************************
"-" pressed
******************************************************************************/

void buttonMinusPressed() {
	DEBUG_PRINTLN("- pressed.");
	screenBufferNeedsUpdate = true;
	switch (mode) {
	case STD_MODE_TITLE_TEMP:
		setMode(STD_MODE_NORMAL);
		break;
#ifndef BUZZER
	case STD_MODE_NORMAL:
		setMode(STD_MODE_TITLE_TEMP);
		break;
#endif
#ifdef BUZZER
	case STD_MODE_TITLE_ALARM:
		setMode(STD_MODE_TITLE_TEMP);
		break;
	case STD_MODE_NORMAL:
		setMode(STD_MODE_TITLE_ALARM);
		break;
	case STD_MODE_SET_TIMER:
		if (timerMinutes > 0) {
			timerMinutes--;
			if (timerMinutes == 0) timerSet = false;
			else {
				timer = now() + timerMinutes * 60;
				timerSet = true;
			}
		}
		break;
	case STD_MODE_TIMER:
		timerSet = false;
		setMode(STD_MODE_SET_TIMER);
		break;
	case STD_MODE_ALARM_1:
		settings.toggleAlarm1();
		break;
	case STD_MODE_SET_ALARM_1:
		settings.setAlarmTime1(settings.getAlarmTime1() + 300);
#ifdef DEBUG
		debug.debugTime("Alarm 1:", settings.getAlarmTime1());
#endif
		break;
	case STD_MODE_ALARM_2:
		settings.toggleAlarm2();
		break;
	case STD_MODE_SET_ALARM_2:
		settings.setAlarmTime2(settings.getAlarmTime2() + 300);
#ifdef DEBUG
		debug.debugTime("Alarm 2:", settings.getAlarmTime2());
#endif
#endif
		break;
	case EXT_MODE_TITLE_MAIN:
		setMode(EXT_MODE_TITLE_TEST);
		break;
#ifdef LDR
	case EXT_MODE_LDR:
		settings.toggleUseLdr();
		lastLdrValue = 0;
		break;
#endif
	case EXT_MODE_BRIGHTNESS:
		settings.setBrightness(constrain(settings.getBrightness() - 10, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
		brightness = settings.getBrightness();
		writeScreenBuffer(matrix, settings.getColor(), brightness);
		DEBUG_PRINTLN(settings.getBrightness());
		break;
	case EXT_MODE_COLOR:
		if (settings.getColor() > 0) settings.setColor(settings.getColor() - 1);
		else settings.setColor(COLOR_COUNT - 1);
		break;
	case EXT_MODE_TRANSITION:
		if (settings.getTransition() > 0) settings.setTransition(settings.getTransition() - 1);
		else settings.setTransition(Settings::TRANSITION_COUNT - 1);
		break;
	case EXT_MODE_TIMEOUT:
		if (settings.getTimeout() > 0) settings.setTimeout(settings.getTimeout() - 1);
		break;
	case EXT_MODE_LANGUAGE:
		if (settings.getLanguage() > 0) settings.setLanguage(settings.getLanguage() - 1);
		else settings.setLanguage(LANGUAGE_COUNT - 1);
		break;
	case EXT_MODE_TITLE_TIME:
		setMode(EXT_MODE_TITLE_MAIN);
		break;
	case EXT_MODE_TIMESET:
		setTime(hour(), minute() + 1, 0, day(), month(), year());
#ifdef DEBUG
		debug.debugTime("Time set:", now());
#endif
		break;
	case EXT_MODE_IT_IS:
		settings.toggleItIs();
		break;
	case EXT_MODE_DAYSET:
		setTime(hour(), minute(), second(), day() - 1, month(), year());
		break;
	case EXT_MODE_MONTHSET:
		setTime(hour(), minute(), second(), day(), month() - 1, year());
		break;
	case EXT_MODE_YEARSET:
		setTime(hour(), minute(), second(), day(), month(), year() - 1);
		break;
	case EXT_MODE_NIGHTOFF:
		settings.setNightOffTime(settings.getNightOffTime() + 300);
#ifdef DEBUG
		debug.debugTime("Night off:", settings.getNightOffTime());
#endif
		break;
	case EXT_MODE_DAYON:
		settings.setDayOnTime(settings.getDayOnTime() + 300);
#ifdef DEBUG
		debug.debugTime("Night on:", settings.getDayOnTime());
#endif
		break;
	case EXT_MODE_TITLE_IP:
		setMode(EXT_MODE_TITLE_TIME);
		break;
	case EXT_MODE_TITLE_TEST:
		setMode(EXT_MODE_TITLE_IP);
		break;
	default:
		break;
	}
}

/******************************************************************************
"time" pressed
******************************************************************************/

void buttonTimePressed() {
	DEBUG_PRINTLN("Time pressed.");
	screenBufferNeedsUpdate = true;
#ifdef BUZZER
	if (alarmOn) {
		alarmOn = false;
		digitalWrite(PIN_BUZZER, LOW);
	}
#endif
#ifdef RTC_BACKUP
	RTC.set(now());
#endif
	settings.saveToEEPROM();
	fallBackCounter = 0;
	setMode(STD_MODE_NORMAL);
}

/******************************************************************************
IR-signal received
******************************************************************************/

#ifdef IR_REMOTE
void remoteAction(decode_results irDecodeResult) {
	switch (irDecodeResult.value) {
	case IR_CODE_ONOFF:
		setDisplayToToggle();
		break;
	case IR_CODE_TIME:
		buttonTimePressed();
		break;
	case IR_CODE_MODE:
		buttonModePressed();
		break;
	case IR_CODE_EXTMODE:
		buttonSettingsPressed();
		break;
	case IR_CODE_PLUS:
		buttonPlusPressed();
		break;
	case IR_CODE_MINUS:
		buttonMinusPressed();
		break;
	default:
		break;
	}
}
#endif

/******************************************************************************
transitions
******************************************************************************/

void writeScreenBuffer(uint16_t screenBuffer[], uint8_t color, uint8_t brightness) {
	ledDriver.clear();
	for (uint8_t y = 0; y <= 9; y++) {
		for (uint8_t x = 0; x <= 10; x++) {
			if (bitRead(screenBuffer[y], 15 - x)) ledDriver.setPixel(x, y, color, brightness);
		}
	}
	for (uint8_t y = 0; y <= 4; y++) {
		if (bitRead(screenBuffer[y], 4)) ledDriver.setPixel(110 + y, color, brightness);
	}
	ledDriver.show();
}

void writeScreenBufferFade(uint16_t screenBufferOld[], uint16_t screenBufferNew[], uint8_t color, uint8_t brightness) {
	ledDriver.clear();
	uint8_t brightnessBuffer[10][12] = {};
	for (uint8_t y = 0; y <= 9; y++) {
		for (uint8_t x = 0; x <= 11; x++) {
			if (bitRead(screenBufferOld[y], 15 - x)) brightnessBuffer[y][x] = brightness;
		}
	}
	for (uint8_t i = 0; i < brightness; i++) {
		for (uint8_t y = 0; y <= 9; y++) {
			for (uint8_t x = 0; x <= 11; x++) {
				if (!(bitRead(screenBufferOld[y], 15 - x)) && (bitRead(screenBufferNew[y], 15 - x))) brightnessBuffer[y][x]++;
				if ((bitRead(screenBufferOld[y], 15 - x)) && !(bitRead(screenBufferNew[y], 15 - x))) brightnessBuffer[y][x]--;
				ledDriver.setPixel(x, y, color, brightnessBuffer[y][x]);
			}
		}
		for (uint8_t y = 0; y <= 4; y++) ledDriver.setPixel(110 + y, color, brightnessBuffer[y][11]);
		esp8266WebServer.handleClient();
		ledDriver.show();
		//delay(3);
	}
}

/******************************************************************************
misc
******************************************************************************/

// set mode
void setMode(Mode newMode) {
	DEBUG_PRINT("Mode: ");
	DEBUG_PRINTLN(newMode);
	screenBufferNeedsUpdate = true;
	lastMode = mode;
	mode = newMode;
}

// turn off LEDs
void setLedsOff() {
	DEBUG_PRINTLN("LEDs off.");
	setMode(STD_MODE_BLANK);
}

// turn on LEDs
void setLedsOn() {
	DEBUG_PRINTLN("LEDs on.");
	setMode(lastMode);
}

// toggle LEDs
void setDisplayToToggle() {
	if (mode != STD_MODE_BLANK) setLedsOff();
	else setLedsOn();
}

#ifdef RTC_BACKUP
// get time from RTC
time_t getRtcTime() {
	DEBUG_PRINTLN("*** ESP set from RTC. ***");
	return RTC.get();
}
#endif

/******************************************************************************
weather
******************************************************************************/

// complete result for Zuerich
// https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22zurich%2C%20ch%22)%20and%20u=%27c%27&format=json

void getYahooWeather(String yahooLocation) {
	if (WiFi.status() != WL_CONNECTED) {
		DEBUG_PRINTLN("WiFi not connected. :( Can not get weather.");
		return;
	}
	DEBUG_PRINTLN("Sending REST-request for weather.");
	WiFiClient	wifiClient;
	char server[] = "query.yahooapis.com";
	RestClient restClient = RestClient(wifiClient, server, 80);
	String sqlQuery = "select item.title, item.condition.temp, item.condition.code ";
	sqlQuery += "from weather.forecast where woeid in (select woeid from geo.places(1) where text=%22" + yahooLocation + "%22) and u=%27c%27";
	sqlQuery.replace(" ", "%20");
	sqlQuery.replace(",", "%2C");
	restClient.get("query.yahooapis.com/v1/public/yql?q=" + sqlQuery + "&format=json");
	String response = restClient.readResponse();
	response = response.substring(response.indexOf('{'), response.lastIndexOf('}' + 1));
	//DEBUG_PRINTLN("REST-response: " + response);
	DynamicJsonBuffer jsonBuffer;
	JsonObject &responseJson = jsonBuffer.parseObject(response);
	if (!responseJson.success()) {
		DEBUG_PRINTLN("Parsing JSON failed.");
		return;
	}
	else DEBUG_PRINTLN("Parsing JSON succeeded.");
	String yahooTitle = responseJson["query"]["results"]["channel"]["item"]["title"];
	DEBUG_PRINTLN(yahooTitle);
	yahooTemp = responseJson["query"]["results"]["channel"]["item"]["condition"]["temp"];
	DEBUG_PRINTLN("External temperature is: " + String(yahooTemp));
	yahooCode = responseJson["query"]["results"]["channel"]["item"]["condition"]["code"];
	DEBUG_PRINTLN("Condition code is: " + String(yahooCode));
}

/******************************************************************************
ntp
******************************************************************************/

time_t getNtpTime() {
	if (WiFi.status() != WL_CONNECTED) {
		DEBUG_PRINTLN("WiFi not connected. :( Can not get NTP time.");
#ifdef RTC_BACKUP
		return getRtcTime();
#else
		return now();
#endif
	}
	DEBUG_PRINTLN("Sending NTP-request for time.");
	uint8_t packetBuffer[49] = { };
	packetBuffer[0] = 0xE3;
	packetBuffer[1] = 0x00;
	packetBuffer[2] = 0x06;
	packetBuffer[3] = 0xEC;
	packetBuffer[12] = 0x31;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 0x31;
	packetBuffer[15] = 0x34;
	WiFiUDP wifiUdp;
	wifiUdp.begin(2390);
	char server[] = NTP_SERVER;
	IPAddress timeServerIP;
	WiFi.hostByName(server, timeServerIP);
	wifiUdp.beginPacket(timeServerIP, 123);
	wifiUdp.write(packetBuffer, 48);
	wifiUdp.endPacket();
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		if (wifiUdp.parsePacket() >= 48) {
			wifiUdp.read(packetBuffer, 48);
			uint32_t ntpTime = (packetBuffer[40] << 24) + (packetBuffer[41] << 16) + (packetBuffer[42] << 8) + packetBuffer[43];
			ntpTime -= 2208988800; // NTP time is seconds from 1900, we need from 1970
#ifdef DEBUG
			debug.debugTime("NTP-response: (GMT) ", ntpTime);
#endif
#ifdef RTC_BACKUP
			DEBUG_PRINTLN("*** RTC set from NTP. ***");
			RTC.set(timeZone.toLocal(ntpTime));
#endif
			DEBUG_PRINTLN("*** ESP set from NTP. ***");
			return (timeZone.toLocal(ntpTime));
		}
	}
	DEBUG_PRINTLN("No NTP response. :(");
#ifdef RTC_BACKUP
	return getRtcTime();
#else
	return now();
#endif
}

/******************************************************************************
webserver
******************************************************************************/

void setupWebServer() {
	esp8266WebServer.onNotFound(handleNotFound);
	esp8266WebServer.on("/", handleRoot);
	esp8266WebServer.on("/handle_TOGGLEBLANK", handle_TOGGLEBLANK);
	esp8266WebServer.on("/handle_BUTTON_TIME", handle_BUTTON_TIME);
	esp8266WebServer.on("/handle_BUTTON_MODE", handle_BUTTON_MODE);
	esp8266WebServer.on("/handle_BUTTON_SETTINGS", handle_BUTTON_SETTINGS);
	esp8266WebServer.on("/handle_BUTTON_PLUS", handle_BUTTON_PLUS);
	esp8266WebServer.on("/handle_BUTTON_MINUS", handle_BUTTON_MINUS);
	esp8266WebServer.begin();
}

// page 404
void handleNotFound() {
	esp8266WebServer.send(404, "text/plain", "404 - File Not Found.");
}

// page /
void handleRoot() {
	String message = "<!doctype html>";
	message += "<html>";
	message += "<head>";
	message += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
	message += "<title>";
	message += HOSTNAME;
	message += "</title>";
	message += "<style>";
	message += "body {background-color:#FFFFFF; text-align:center; font-family:verdana; color:#333333;}";
	message += "button {background-color:#1FA3EC; border: 5px solid #FFFFFF; color:#FFFFFF; width: 200px; padding:15px 32px; text-align:center; display:inline-block; font-size:16px;}";
	message += "</style>";
	message += "</head>";
	message += "<body>";
	message += "<h1>";
	message += HOSTNAME;
	message += "</h1>";
	if (mode == STD_MODE_BLANK)	message += "<button onclick=\"window.location.href='/handle_TOGGLEBLANK'\">" + String(LANG_ON) + "</button>";
	else message += "<button onclick=\"window.location.href='/handle_TOGGLEBLANK'\">" + String(LANG_OFF) + "</button>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_TIME'\">" + String(LANG_TIME) + "</button>";
	message += "<br><br>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_MODE'\">" + String(LANG_MODE) + "</button>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_SETTINGS'\">" + String(LANG_SETTINGS) + "</button>";
	message += "<br><br>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_PLUS'\">" + String(LANG_PLUS) + "</button>";
	message += "<button onclick=\"window.location.href='/handle_BUTTON_MINUS'\">" + String(LANG_MINUS) + "</button>";
	message += "<br><br>";
#ifdef RTC_BACKUP
	message += String(LANG_TEMPERATURE) + ": " + String(RTC.temperature() / 4.0 + RTC_TEMP_OFFSET) + "&#176;C / " + String((RTC.temperature() / 4.0 + RTC_TEMP_OFFSET) * 9.0 / 5.0 + 32.0) + "&#176;F";
	message += "<br>";
#endif
	message += String(LANG_EXT_TEMPERATURE) + ": " + String(yahooTemp) + "&#176;C / " + String(yahooTemp * 9 / 5 + 32) + "&#176;F";
	message += "<br>";
	message += "<font size=2>";
	message += "Firmware: " + String(FIRMWARE_VERSION);
#ifdef DEBUG_WEBSITE
	message += "<br>";
	message += "LED-Driver: " + ledDriver.getSignature();
	message += "<br>";
	message += "Free RAM: " + String(system_get_free_heap_size()) + " bytes";
#ifdef LDR
	message += "<br>";
	message += "Use LDR: ";
	if (settings.getUseLdr()) message += "On"; else message += "Off";
	message += "<br>";
	message += "Brightness: " + String(ratedBrightness) + " (LDR: " + String(ldrValue) + ", min: " + String(minLdrValue) + ", max: " + String(maxLdrValue) + ")";
#endif
#endif
	message += "</font>";
	message += "</body>";
	message += "</html>";
	esp8266WebServer.send(200, "text/html", message);
}

// site buttons

void handle_TOGGLEBLANK() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	esp8266WebServer.send(200, "text/html", message);
	setDisplayToToggle();
}

void handle_BUTTON_TIME() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	esp8266WebServer.send(200, "text/html", message);
	buttonTimePressed();
}

void handle_BUTTON_MODE() {
	String message = "<!doctype html><html><head><script>window.onload = function() {window.location.replace('/')};</script></head><body></body></html>";
	esp8266WebServer.send(200, "text/html", message);
	buttonModePressed();
}

void handle_BUTTON_SETTINGS() {
	String message = "<!doctype html><html><head><script>window.onload = function() {window.location.replace('/')};</script></head><body></body></html>";
	esp8266WebServer.send(200, "text/html", message);
	buttonSettingsPressed();
}

void handle_BUTTON_PLUS() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	esp8266WebServer.send(200, "text/html", message);
	buttonPlusPressed();
}

void handle_BUTTON_MINUS() {
	String message = "<!doctype html><html><head><script>window.onload  = function() {window.location.replace('/')};</script></head><body></body></html>";
	esp8266WebServer.send(200, "text/html", message);
	buttonMinusPressed();
}
