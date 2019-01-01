/******************************************************************************
QLOCKWORK
An advanced firmware for a DIY "word-clock".

@mc ESP8266
@created 12.03.2017

This source file is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This source file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#define FIRMWARE_VERSION 20181213

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <ArduinoOTA.h>
#include <DHT.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <Ticker.h>
#include <TimeLib.h>

#include "ArduinoJson.h"
#include "DS3232RTC.h"
#include "Colors.h"
#include "Configuration.h"
#include "Events.h"
#include "Languages.h"
#include "LedDriver_NeoPixel.h"
#include "Modes.h"
#include "Renderer.h"
#include "Settings.h"
#include "Syslog.h"
#include "Timezone.h"
#include "Timezones.h"
#include "WiFiManager.h"

/******************************************************************************
  Init
******************************************************************************/

ESP8266WebServer esp8266WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;
Renderer renderer;
Settings settings;
LedDriver_NeoPixel ledDriver;
#ifdef SENSOR_DHT22
DHT dht(PIN_DHT22, DHT22);
#endif
#ifdef IR_RECEIVER
IRrecv irrecv(PIN_IR_RECEIVER);
decode_results irDecodeResult;
#endif
#ifdef SYSLOGSERVER
WiFiUDP wifiUdp;
Syslog syslog(wifiUdp, SYSLOGSERVER_SERVER, SYSLOGSERVER_PORT, HOSTNAME, "QLOCKWORK", LOG_INFO);
#endif

// Screenbuffer
uint16_t matrix[10] = {};
uint16_t matrixOld[10] = {};
boolean screenBufferNeedsUpdate = true;

// Mode
Mode mode = MODE_TIME;
Mode lastMode = mode;
uint32_t modeTimeout = 0;
uint32_t autoModeChangeTimer = AUTO_MODECHANGE_TIME;
boolean runTransitionOnce = false;
uint8_t autoMode = 0;

//Time
uint8_t lastDay = 0;
uint8_t lastMinute = 0;
uint8_t lastHour = 0;
uint8_t lastSecond = 0;
uint32_t last500Millis = 0;
uint32_t last50Millis = 0;
time_t upTime = 0;
uint8_t randomHour = 0;
uint8_t randomMinute = 0;
uint8_t randomSecond = 0;
uint8_t moonphase = 0;
uint8_t errorCounterNtp = 0;

// Feed
String feedText = "";
uint8_t feedColor = WHITE;
uint8_t feedPosition = 0;

// Yahoo
String outdoorTitle = "No Wifi.";
int8_t outdoorTemperature = 0;
uint8_t outdoorHumidity = 0;
uint8_t outdoorCode = 0;
uint8_t errorCounterYahoo = 0;

// DHT22
float roomTemperature = 0;
float roomHumidity = 0;
uint8_t errorCounterDht = 0;

// LDR
uint8_t abcBrightness = map(settings.getBrightness(), 10, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
uint8_t ratedBrightness = abcBrightness;
uint8_t brightness = abcBrightness;
uint16_t ldrValue = 0;
uint16_t lastLdrValue = 0;
uint16_t minLdrValue = 512;
uint16_t maxLdrValue = 512;

#ifdef BUZZER
// Alarm
boolean alarmTimerSet = false;
uint8_t alarmTimer = 0;
uint8_t alarmTimerSecond = 0;
uint8_t alarmOn = false;
#endif

#ifdef EVENT_TIME
// Events
uint32_t showEventTimer = EVENT_TIME;
#endif

// Misc
uint8_t testColumn = 0;
String updateInfo = "";
IPAddress myIP = { 0,0,0,0 };
uint32_t lastButtonPress = 0;
Ticker tickerEveryMinute;
Ticker tickerEvery500ms;
Ticker tickerEvery250ms;
Ticker tickerEvery50ms;

/******************************************************************************
  Setup()
******************************************************************************/

void setup()
{
	// Init serial port
	Serial.begin(SERIAL_SPEED);
	while (!Serial);
	delay(1000);

	// And the monkey flips the switch. (Akiva Goldsman)
	Serial.println();
	Serial.println("*** QLOCKWORK ***");
	Serial.println("Firmware: " + String(FIRMWARE_VERSION));
	Serial.print("LED-Layout: ");
#ifdef LED_LAYOUT_HORIZONTAL
	Serial.println("horizontal");
#endif
#ifdef LED_LAYOUT_VERTICAL
	Serial.println("vertical");
#endif

#ifdef SELFTEST
	renderer.setAllScreenBuffer(matrix);
	Serial.println("Set all LEDs to red.");
	writeScreenBuffer(matrix, RED, BRIGHTNESS_SELFTEST);
	delay(2500);
	Serial.println("Set all LEDs to green.");
	writeScreenBuffer(matrix, GREEN, BRIGHTNESS_SELFTEST);
	delay(2500);
	Serial.println("Set all LEDs to blue.");
	writeScreenBuffer(matrix, BLUE, BRIGHTNESS_SELFTEST);
	delay(2500);
	Serial.println("Set all LEDs to white.");
	writeScreenBuffer(matrix, WHITE, BRIGHTNESS_SELFTEST);
	delay(2500);
#endif

#ifdef ESP_LED
	Serial.println("Setting up ESP-LED.");
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, HIGH);
#endif

#ifdef MODE_BUTTON
	Serial.println("Setting up Mode-Button.");
	pinMode(PIN_MODE_BUTTON, INPUT_PULLUP);
	attachInterrupt(PIN_MODE_BUTTON, buttonModeInterrupt, FALLING);
#endif

#ifdef ONOFF_BUTTON
	Serial.println("Setting up Back-Button.");
	pinMode(PIN_ONOFF_BUTTON, INPUT_PULLUP);
	attachInterrupt(PIN_ONOFF_BUTTON, buttonOnOffInterrupt, FALLING);
#endif

#ifdef TIME_BUTTON
	Serial.println("Setting up Time-Button.");
	pinMode(PIN_TIME_BUTTON, INPUT_PULLUP);
	attachInterrupt(PIN_TIME_BUTTON, buttonTimeInterrupt, FALLING);
#endif

#ifdef BUZZER
	Serial.println("Setting up Buzzer.");
	pinMode(PIN_BUZZER, OUTPUT);
#endif

#ifdef SENSOR_DHT22
	Serial.println("Setting up DHT22.");
	dht.begin();
#endif

#ifdef LDR
	Serial.print("Setting up LDR. ABC: ");
	settings.getUseAbc() ? Serial.println("enabled") : Serial.println("disabled");
	pinMode(PIN_LDR, INPUT);
#endif

#ifdef IR_RECEIVER
	Serial.println("Setting up IR-Receiver.");
	irrecv.enableIRIn();
#endif

	Serial.println("Setting up Tickers.");
	tickerEveryMinute.attach(60, callbackEveryMinute);
	tickerEvery500ms.attach_ms(500, callbackEvery500ms);
	tickerEvery250ms.attach_ms(250, callbackEvery250ms);
	tickerEvery50ms.attach_ms(50, callbackEvery50ms);

	// Start WiFi and services
	renderer.clearScreenBuffer(matrix);
	renderer.setSmallText("WI", TEXT_POS_TOP, matrix);
	renderer.setSmallText("FI", TEXT_POS_BOTTOM, matrix);
	writeScreenBuffer(matrix, WHITE, brightness);
	WiFiManager wifiManager;
	//wifiManager.resetSettings();
	wifiManager.setTimeout(WIFI_SETUP_TIMEOUT);
	wifiManager.autoConnect(HOSTNAME, WIFI_AP_PASS);
	WiFi.setAutoReconnect(true);
	if (!WiFi.isConnected())
	{
		WiFi.mode(WIFI_AP);
		Serial.println("No WLAN connected. Staying in AP mode.");
		writeScreenBuffer(matrix, RED, brightness);
#ifdef BUZZER
		digitalWrite(PIN_BUZZER, HIGH);
		delay(1500);
		digitalWrite(PIN_BUZZER, LOW);
#endif
		delay(1000);
		myIP = WiFi.softAPIP();
	}
	else
	{
		WiFi.mode(WIFI_STA);
		Serial.println("WLAN connected. Switching to STA mode.");
		writeScreenBuffer(matrix, GREEN, brightness);
#ifdef BUZZER
		for (uint8_t i = 0; i <= 2; i++)
		{
#ifdef DEBUG
			Serial.println("Beep!");
#endif
			digitalWrite(PIN_BUZZER, HIGH);
			delay(100);
			digitalWrite(PIN_BUZZER, LOW);
			delay(100);
		}
#endif
		delay(1000);
		myIP = WiFi.localIP();

		// mDNS is needed to see HOSTNAME in Arduino IDE
		Serial.println("Starting mDNS responder.");
		MDNS.begin(HOSTNAME);
		//MDNS.addService("http", "tcp", 80);

		Serial.println("Starting OTA service.");
#ifdef DEBUG
		ArduinoOTA.onStart([]()
		{
			Serial.println("Start OTA update.");
		});
		ArduinoOTA.onError([](ota_error_t error)
		{
			Serial.println("OTA Error: " + String(error));
			if (error == OTA_AUTH_ERROR) Serial.println("Auth failed.");
			else if (error == OTA_BEGIN_ERROR) Serial.println("Begin failed.");
			else if (error == OTA_CONNECT_ERROR) Serial.println("Connect failed.");
			else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive failed.");
			else if (error == OTA_END_ERROR) Serial.println("End failed.");
		});
		ArduinoOTA.onEnd([]()
		{
			Serial.println("End OTA update.");
		});
#endif
		ArduinoOTA.setPassword(OTA_PASS);
		ArduinoOTA.begin();

#ifdef SYSLOGSERVER
		Serial.println("Starting syslog.");
		syslog.log(LOG_INFO, ";#;dateTime;roomTemperature;roomHumidity;outdoorTemperature;outdoorHumidity;outdoorCode;ldrValue;errorCounterNtp;errorCounterDht;errorCounterYahoo;freeHeapSize;upTime");
#ifdef DEBUG
		Serial.println("Header written to syslog.");
#endif
#endif

		getOutdoorConditions(LOCATION);
	}

#ifdef SHOW_IP
	WiFi.isConnected() ? feedText = "  IP: " : feedText = "  AP-IP: ";
	feedText += String(myIP[0]) + '.' + String(myIP[1]) + '.' + String(myIP[2]) + '.' + String(myIP[3]) + "   ";
	feedPosition = 0;
	feedColor = WHITE;
	mode = MODE_FEED;
#endif

	Serial.println("Starting webserver.");
	setupWebServer();

	Serial.println("Starting updateserver.");
	httpUpdater.setup(&esp8266WebServer);

	renderer.clearScreenBuffer(matrix);

	// Get the time!
#ifdef RTC_BACKUP
	time_t tempRtcTime = RTC.get();
	setTime(timeZone.toLocal(tempRtcTime));
#ifdef DEBUG
	Serial.printf("Time (RTC): %02u:%02u:%02u %02u.%02u.%04u (UTC)\r\n", hour(tempRtcTime), minute(tempRtcTime), second(tempRtcTime), day(tempRtcTime), month(tempRtcTime), year(tempRtcTime));
#endif
#endif
	if (WiFi.isConnected())
	{
		time_t tempNtpTime = getNtpTime(NTP_SERVER);
		if (tempNtpTime)
		{
			setTime(timeZone.toLocal(tempNtpTime));
#ifdef RTC_BACKUP
#ifdef DEBUG
			Serial.printf("Drift (RTC): %d sec.\r\n", tempNtpTime - RTC.get());
#endif
			RTC.set(tempNtpTime);
#endif
		}
	}

	// Get a random time
	randomSeed(analogRead(A0));
	randomHour = random(0, 24);
	randomMinute = random(5, 56);
	randomSecond = random(5, 56);

#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
	// Update room conditions
	getRoomConditions();
#endif

#ifdef DEBUG
	// Print some infos
	Serial.printf("Defined events: %u\r\n", sizeof(events) / sizeof(event_t));
	Serial.printf("Night off: %02u:%02u:00\r\n", hour(settings.getNightOffTime()), minute(settings.getNightOffTime()));
	Serial.printf("Day on: %02u:%02u:00\r\n", hour(settings.getDayOnTime()), minute(settings.getDayOnTime()));
	Serial.printf("Alarm1: %02u:%02u:00 ", hour(settings.getAlarm1Time()), minute(settings.getAlarm1Time()));
	settings.getAlarm1() ? Serial.print("on ") : Serial.print("off ");
	Serial.println(settings.getAlarm1Weekdays(), BIN);
	Serial.printf("Alarm2: %02u:%02u:00 ", hour(settings.getAlarm2Time()), minute(settings.getAlarm2Time()));
	settings.getAlarm2() ? Serial.print("on ") : Serial.print("off ");
	Serial.println(settings.getAlarm2Weekdays(), BIN);
	Serial.printf("Random time: %02u:%02u:%02u\r\n", randomHour, randomMinute, randomSecond);
	Serial.printf("Free RAM: %u bytes\r\n", system_get_free_heap_size());
#else
	Serial.println("Debug is off.");
#endif

	lastDay = day();
	lastHour = hour();
	lastMinute = minute();
	lastSecond = second();

#ifdef FRONTCOVER_BINARY
	settings.setTransition(TRANSITION_NORMAL);
#endif
} // setup()

/******************************************************************************
Run every Minute
******************************************************************************/

void callbackEveryMinute()
{
#ifdef RTC_BACKUP
	// Set ESP time from RTC
#ifdef DEBUG
	Serial.println("ESP set from RTC.");
#endif
	setTime(timeZone.toLocal(RTC.get()));
#endif
}

/******************************************************************************
Run every 500 milliseconds (2 Hz)
******************************************************************************/

void callbackEvery500ms()
{
#ifdef LDR
	// Get rated brightness from LDR
	if (settings.getUseAbc()) getBrightnessFromLdr();
#endif
}

/******************************************************************************
Run every 250 milliseconds (4 Hz)
******************************************************************************/

void callbackEvery250ms()
{
#ifdef BUZZER
	// Make some noise.
	if (alarmOn)
	{
		alarmOn--;
		digitalRead(PIN_BUZZER) == LOW ? digitalWrite(PIN_BUZZER, HIGH) : digitalWrite(PIN_BUZZER, LOW);
		if (!alarmOn)
		{
#ifdef DEBUG
			Serial.println("Alarm: off");
#endif
			digitalWrite(PIN_BUZZER, LOW);
			screenBufferNeedsUpdate = true;
		}
	}
#endif
}

/******************************************************************************
Run every 50 milliseconds (20 Hz)
******************************************************************************/

void callbackEvery50ms()
{
	// Fade brightness to rated brightness
	if (brightness < ratedBrightness)
	{
		brightness++;
		writeScreenBuffer(matrix, settings.getColor(), brightness);
	}
	if (brightness > ratedBrightness)
	{
		brightness--;
		writeScreenBuffer(matrix, settings.getColor(), brightness);
	}
}

/******************************************************************************
  Loop()
******************************************************************************/

void loop()
{
	// ************************************************************************
	// Run once a day
	// ************************************************************************

	if (day() != lastDay)
	{
		lastDay = day();
		screenBufferNeedsUpdate = true;
		time_t tempEspTime = now();

#ifdef SHOW_MODE_MOONPHASE
		moonphase = getMoonphase(year(), month(), day());
#endif

		// Reset URL event 0
		events[0].day = 0;
		events[0].month = 0;

#ifdef DEBUG
		Serial.printf("Uptime: %u days, %02u:%02u\r\n", int(upTime / 86400), hour(upTime), minute(upTime));
		Serial.printf("Free RAM: %u bytes\r\n", system_get_free_heap_size());
		Serial.printf("Moonphase: %u\r\n", moonphase);
#endif

		// Change color
		if (settings.getColorChange() == COLORCHANGE_DAY)
		{
			settings.setColor(random(0, COLORCHANGE_COUNT + 1));
#ifdef DEBUG
			Serial.printf("Color changed to: %u\r\n", settings.getColor());
#endif
		}
	}

	// ************************************************************************
	// Run once every hour
	// ************************************************************************

	if (hour() != lastHour)
	{
		lastHour = hour();
		screenBufferNeedsUpdate = true;

		// Change color
		if (settings.getColorChange() == COLORCHANGE_HOUR)
		{
			settings.setColor(random(0, COLOR_COUNT + 1));
#ifdef DEBUG
			Serial.printf("Color changed to: %u\r\n", settings.getColor());
#endif
		}

		// Hourly beep
#ifdef BUZZER
		if ((settings.getHourBeep() == true) && (mode == MODE_TIME))
		{
			digitalWrite(PIN_BUZZER, HIGH);
			delay(25);
			digitalWrite(PIN_BUZZER, LOW);
#ifdef DEBUG
			Serial.println("Beep!");
#endif
		}
#endif

		if (hour() == randomHour)
		{
#if defined(UPDATE_INFO_STABLE) || defined(UPDATE_INFO_UNSTABLE)
			// Get updateinfo once a day at the random hour
			if (WiFi.isConnected()) getUpdateInfo();
#endif
		}

	}

	// ************************************************************************
	// Run once every minute
	// ************************************************************************

	if (minute() != lastMinute)
	{
		lastMinute = minute();
		screenBufferNeedsUpdate = true;

#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
		// Update room conditions
		getRoomConditions();
#endif

#ifdef BUZZER
		// Switch on buzzer for alarm 1
		if (settings.getAlarm1() && (hour() == hour(settings.getAlarm1Time())) && (minute() == minute(settings.getAlarm1Time())) && bitRead(settings.getAlarm1Weekdays(), weekday()))
		{
			alarmOn = BUZZTIME_ALARM_1 * 4;
#ifdef DEBUG
			Serial.println("Alarm1 on.");
#endif
		}

		// Switch on buzzer for alarm 2
		if (settings.getAlarm2() && (hour() == hour(settings.getAlarm2Time())) && (minute() == minute(settings.getAlarm2Time())) && bitRead(settings.getAlarm2Weekdays(), weekday()))
		{
			alarmOn = BUZZTIME_ALARM_2 * 4;
#ifdef DEBUG
			Serial.println("Alarm2 on.");
#endif
		}
#endif

#ifdef DEBUG
		time_t tempEspTime = now();
		Serial.printf("Time (ESP): %02u:%02u:%02u %02u.%02u.%04u\r\n", hour(tempEspTime), minute(tempEspTime), second(tempEspTime), day(tempEspTime), month(tempEspTime), year(tempEspTime));
#endif
		// Set night- and daymode
		if ((hour() == hour(settings.getNightOffTime())) && (minute() == minute(settings.getNightOffTime())))
		{
#ifdef DEBUG
			Serial.println("Night off.");
#endif
			setMode(MODE_BLANK);
		}
		if ((hour() == hour(settings.getDayOnTime())) && (minute() == minute(settings.getDayOnTime())))
		{
#ifdef DEBUG
			Serial.println("Day on.");
#endif
			setMode(lastMode);
		}

		if ((minute() == randomMinute) || (errorCounterNtp > 0))
		{
			if (WiFi.isConnected())
			{
				// Set ESP (and RTC) time from NTP
				time_t tempNtpTime = getNtpTime(NTP_SERVER);
				if (tempNtpTime)
				{
#ifdef DEBUG
					Serial.printf("Drift (ESP): %d sec.\r\n", tempNtpTime - timeZone.toUTC(now()));
#endif
					setTime(timeZone.toLocal(tempNtpTime));
#ifdef RTC_BACKUP
#ifdef DEBUG
					Serial.printf("Drift (RTC): %d sec.\r\n", tempNtpTime - RTC.get());
#endif
					RTC.set(tempNtpTime);
#endif
				}
			}
		}

		if (minute() == randomMinute)
		{
			if (WiFi.isConnected())
			{
				// Get weather from Yahoo
				getOutdoorConditions(LOCATION);
			}
		}


		// ************************************************************************
		// Run once every 5 minutes
		// ************************************************************************

		if (minute() % 5 == 0)
		{
#ifdef SYSLOGSERVER
			// Write some data to syslog
			if (WiFi.isConnected())
			{
				time_t tempEspTime = now();
				syslog.log(LOG_INFO, ";D;" + String(tempEspTime) + ";" + String(roomTemperature) + ";" + String(roomHumidity) + ";" + String(outdoorTemperature) + ";" + String(outdoorHumidity) + ";" + String(outdoorCode) + ";" + String(ldrValue) + ";" + String(errorCounterNtp) + ";" + String(errorCounterDht) + ";" + String(errorCounterYahoo) + ";" + String(system_get_free_heap_size()) + ";" + String(upTime));
#ifdef DEBUG
				Serial.println("Data written to syslog.");
#endif
			}
#endif
			// Change color
			if (settings.getColorChange() == COLORCHANGE_FIVE)
			{
				settings.setColor(random(0, COLOR_COUNT + 1));
#ifdef DEBUG
				Serial.printf("Color changed to: %u\r\n", settings.getColor());
#endif
			}
		}
	}

	// ************************************************************************
	// Run once every second
	// ************************************************************************

	if (second() != lastSecond)
	{
		lastSecond = second();

		upTime++;

#ifdef FRONTCOVER_BINARY
		if (mode != MODE_BLANK) screenBufferNeedsUpdate = true;
#else
		// Running displayupdate in MODE_TIME or MODE_BLANK every second will lock the ESP due to TRANSITION_FADE
		if ((mode != MODE_TIME) && (mode != MODE_BLANK)) screenBufferNeedsUpdate = true;
#endif

#ifdef ESP_LED
		// Flash ESP LED
		digitalRead(PIN_LED) == LOW ? digitalWrite(PIN_LED, HIGH) : digitalWrite(PIN_LED, LOW);
#endif

#ifdef BUZZER
		// Countdown timeralarm by one minute in the second it was activated
		if (alarmTimer && alarmTimerSet && (alarmTimerSecond == second()))
		{
			alarmTimer--;
#ifdef DEBUG
			if (alarmTimer) Serial.printf("Timeralarm in %u min.\r\n", alarmTimer);
#endif
		}

		// Switch on buzzer for timer
		if (!alarmTimer && alarmTimerSet)
		{
			alarmTimerSet = false;
			alarmOn = BUZZTIME_TIMER * 4;
#ifdef DEBUG
			Serial.println("Timeralarm: on");
#endif
		}
#endif

		//		// Auto switch modes
		//		if (settings.getModeChange() && (mode == MODE_TIME))
		//		{
		//			autoModeChangeTimer--;
		//			if (!autoModeChangeTimer)
		//			{
		//#ifdef DEBUG
		//				Serial.println("Auto modechange. (" + String(autoMode) + ")");
		//#endif
		//				autoModeChangeTimer = AUTO_MODECHANGE_TIME;
		//				switch (autoMode)
		//				{
		//				case 0:
		//					if (WiFi.isConnected()) setMode(MODE_EXT_TEMP);
		//					autoMode = 1;
		//					break;
		//				case 1:
		//#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
		//					setMode(MODE_TEMP);
		//#else
		//					if (WiFi.isConnected()) setMode(MODE_EXT_TEMP);
		//#endif
		//					autoMode = 2;
		//					break;
		//				case 2:
		//#ifdef SENSOR_DHT22
		//					setMode(MODE_HUMIDITY);
		//#else
		//					if (WiFi.isConnected()) setMode(MODE_EXT_TEMP);
		//#endif
		//					autoMode = 0;
		//					break;
		//				}
		//			}
		//		}

				// Auto switch modes
		if (settings.getModeChange() && (mode == MODE_TIME))
		{
			autoModeChangeTimer--;
			if (!autoModeChangeTimer)
			{
#ifdef DEBUG
				Serial.println("Auto modechange. (" + String(autoMode) + ")");
#endif
				autoModeChangeTimer = AUTO_MODECHANGE_TIME;
				switch (autoMode)
				{
				case 0:
					if (WiFi.isConnected()) setMode(MODE_EXT_TEMP);
					autoMode = 1;
					break;
				case 1:
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
					setMode(MODE_TEMP);
#else
					if (WiFi.isConnected()) setMode(MODE_EXT_TEMP);
#endif
					autoMode = 0;
					break;
				}
			}
		}

#ifdef EVENT_TIME
		// Show event in feed
		if (mode == MODE_TIME)
		{
			showEventTimer--;
			if (!showEventTimer)
			{
				showEventTimer = EVENT_TIME;
				for (uint8_t i = 0; i < (sizeof(events) / sizeof(event_t)); i++)
				{
					if ((day() == events[i].day) && (month() == events[i].month))
					{
						if (events[i].year) feedText = "  " + events[i].text + " (" + String(year() - events[i].year) + ")   ";
						else feedText = "  " + events[i].text + "   ";
						feedPosition = 0;
						feedColor = events[i].color;
#ifdef DEBUG
						Serial.println("Event: \"" + feedText + "\"");
#endif
						setMode(MODE_FEED);
					}
				}
			}
		}
#endif
	}

	// ************************************************************************
	// Run always
	// ************************************************************************

	if (mode == MODE_FEED) screenBufferNeedsUpdate = true;

	// Call HTTP- and OTA-handle
	esp8266WebServer.handleClient();
	ArduinoOTA.handle();

#ifdef IR_RECEIVER
	// Look for IR commands
	if (irrecv.decode(&irDecodeResult))
	{
#ifdef DEBUG_IR
		Serial.print("IR signal: 0x");
		serialPrintUint64(irDecodeResult.value, HEX);
		Serial.println();
#endif
		switch (irDecodeResult.value)
		{
		case IR_CODE_ONOFF:
			buttonOnOffPressed();
			break;
		case IR_CODE_TIME:
			buttonTimePressed();
			break;
		case IR_CODE_MODE:
			buttonModePressed();
			break;
		}
		irrecv.resume();
	}
#endif

	// Render a new screenbuffer if needed
	if (screenBufferNeedsUpdate)
	{
		screenBufferNeedsUpdate = false;

		// Save old screenbuffer
		for (uint8_t i = 0; i <= 9; i++) matrixOld[i] = matrix[i];

		switch (mode)
		{
		case MODE_TIME:
			renderer.clearScreenBuffer(matrix);

#ifdef FRONTCOVER_BINARY
			matrix[0] = 0b1111000000000000;
			matrix[1] = hour() << 5;
			matrix[2] = minute() << 5;
			matrix[3] = second() << 5;
			matrix[5] = 0b1111000000000000;
			matrix[6] = day() << 5;
			matrix[7] = month() << 5;
			matrix[8] = year() - 2000 << 5;
#else
			renderer.setTime(hour(), minute(), matrix);
			renderer.setCorners(minute(), matrix);
			if (!settings.getItIs() && ((minute() / 5) % 6)) renderer.clearEntryWords(matrix);
#endif
#ifdef BUZZER
			if (settings.getAlarm1() || settings.getAlarm2() || alarmTimerSet) renderer.setAlarmLed(matrix);
#endif
			break;
#ifdef SHOW_MODE_AMPM
		case MODE_AMPM:
			renderer.clearScreenBuffer(matrix);
			isAM() ? renderer.setSmallText("AM", TEXT_POS_MIDDLE, matrix) : renderer.setSmallText("PM", TEXT_POS_MIDDLE, matrix);
			break;
#endif
#ifdef SHOW_MODE_SECONDS
		case MODE_SECONDS:
			renderer.clearScreenBuffer(matrix);
			renderer.setCorners(minute(), matrix);
			for (uint8_t i = 0; i <= 6; i++)
			{
				matrix[1 + i] |= numbersBig[second() / 10][i] << 11;
				matrix[1 + i] |= numbersBig[second() % 10][i] << 5;
			}
			break;
#endif
#ifdef SHOW_MODE_WEEKDAY
		case MODE_WEEKDAY:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText(String(sWeekday[weekday()][0]) + String(sWeekday[weekday()][1]), TEXT_POS_MIDDLE, matrix);
			break;
#endif
#ifdef SHOW_MODE_DATE
		case MODE_DATE:
			renderer.clearScreenBuffer(matrix);
			if (day() < 10) renderer.setSmallText(("0" + String(day())), TEXT_POS_TOP, matrix);
			else renderer.setSmallText(String(day()), TEXT_POS_TOP, matrix);
			if (month() < 10) renderer.setSmallText(("0" + String(month())), TEXT_POS_BOTTOM, matrix);
			else renderer.setSmallText(String(month()), TEXT_POS_BOTTOM, matrix);
			renderer.setPixelInScreenBuffer(5, 4, matrix);
			renderer.setPixelInScreenBuffer(5, 9, matrix);
			break;
#endif
#ifdef SHOW_MODE_MOONPHASE
		case MODE_MOONPHASE:
			renderer.clearScreenBuffer(matrix);
			switch (moonphase)
			{
			case 0:
				matrix[0] = 0b0000111000000000;
				matrix[1] = 0b0011000110000000;
				matrix[2] = 0b0100000001000000;
				matrix[3] = 0b0100000001000000;
				matrix[4] = 0b1000000000100000;
				matrix[5] = 0b1000000000100000;
				matrix[6] = 0b0100000001000000;
				matrix[7] = 0b0100000001000000;
				matrix[8] = 0b0011000110000000;
				matrix[9] = 0b0000111000000000;
				break;
			case 1:
				matrix[0] = 0b0000111000000000;
				matrix[1] = 0b0000001110000000;
				matrix[2] = 0b0000000111000000;
				matrix[3] = 0b0000000111000000;
				matrix[4] = 0b0000000111100000;
				matrix[5] = 0b0000000111100000;
				matrix[6] = 0b0000000111000000;
				matrix[7] = 0b0000000111000000;
				matrix[8] = 0b0000001110000000;
				matrix[9] = 0b0000111000000000;
				break;
			case 2:
				matrix[0] = 0b0000011000000000;
				matrix[1] = 0b0000011110000000;
				matrix[2] = 0b0000011111000000;
				matrix[3] = 0b0000011111000000;
				matrix[4] = 0b0000011111100000;
				matrix[5] = 0b0000011111100000;
				matrix[6] = 0b0000011111000000;
				matrix[7] = 0b0000011111000000;
				matrix[8] = 0b0000011110000000;
				matrix[9] = 0b0000011000000000;
				break;
			case 3:
				matrix[0] = 0b0000111000000000;
				matrix[1] = 0b0001111110000000;
				matrix[2] = 0b0001111111000000;
				matrix[3] = 0b0001111111000000;
				matrix[4] = 0b0001111111100000;
				matrix[5] = 0b0001111111100000;
				matrix[6] = 0b0001111111000000;
				matrix[7] = 0b0001111111000000;
				matrix[8] = 0b0001111110000000;
				matrix[9] = 0b0000111000000000;
				break;
			case 4:
				matrix[0] = 0b0000111000000000;
				matrix[1] = 0b0011111110000000;
				matrix[2] = 0b0111111111000000;
				matrix[3] = 0b0111111111000000;
				matrix[4] = 0b1111111111100000;
				matrix[5] = 0b1111111111100000;
				matrix[6] = 0b0111111111000000;
				matrix[7] = 0b0111111111000000;
				matrix[8] = 0b0011111110000000;
				matrix[9] = 0b0000111000000000;
				break;
			case 5:
				matrix[0] = 0b0000111000000000;
				matrix[1] = 0b0011111100000000;
				matrix[2] = 0b0111111100000000;
				matrix[3] = 0b0111111100000000;
				matrix[4] = 0b1111111100000000;
				matrix[5] = 0b1111111100000000;
				matrix[6] = 0b0111111100000000;
				matrix[7] = 0b0111111100000000;
				matrix[8] = 0b0011111100000000;
				matrix[9] = 0b0000111000000000;
				break;
			case 6:
				matrix[0] = 0b0000110000000000;
				matrix[1] = 0b0011110000000000;
				matrix[2] = 0b0111110000000000;
				matrix[3] = 0b0111110000000000;
				matrix[4] = 0b1111110000000000;
				matrix[5] = 0b1111110000000000;
				matrix[6] = 0b0111110000000000;
				matrix[7] = 0b0111110000000000;
				matrix[8] = 0b0011110000000000;
				matrix[9] = 0b0000110000000000;
				break;
			case 7:
				matrix[0] = 0b0000111000000000;
				matrix[1] = 0b0011100000000000;
				matrix[2] = 0b0111000000000000;
				matrix[3] = 0b0111000000000000;
				matrix[4] = 0b1111000000000000;
				matrix[5] = 0b1111000000000000;
				matrix[6] = 0b0111000000000000;
				matrix[7] = 0b0111000000000000;
				matrix[8] = 0b0011100000000000;
				matrix[9] = 0b0000111000000000;
				break;
			}
			break;
#endif
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
		case MODE_TEMP:
			renderer.clearScreenBuffer(matrix);
			if (roomTemperature == 0)
			{
				matrix[0] = 0b0000000001000000;
				matrix[1] = 0b0000000010100000;
				matrix[2] = 0b0000000010100000;
				matrix[3] = 0b0000000011100000;
			}
			if (roomTemperature > 0)
			{
				matrix[0] = 0b0000000001000000;
				matrix[1] = 0b0100000010100000;
				matrix[2] = 0b1110000010100000;
				matrix[3] = 0b0100000011100000;
			}
			if (roomTemperature < 0)
			{
				matrix[0] = 0b0000000001000000;
				matrix[1] = 0b0000000010100000;
				matrix[2] = 0b1110000010100000;
				matrix[3] = 0b0000000011100000;
			}
			renderer.setSmallText(String(int(roomTemperature + 0.5)), TEXT_POS_BOTTOM, matrix);
			break;
#endif
#ifdef SENSOR_DHT22
		case MODE_HUMIDITY:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText(String(int(roomHumidity + 0.5)), TEXT_POS_TOP, matrix);
			matrix[6] = 0b0100100001000000;
			matrix[7] = 0b0001000010100000;
			matrix[8] = 0b0010000010100000;
			matrix[9] = 0b0100100011100000;
			break;
#endif
		case MODE_EXT_TEMP:
			renderer.clearScreenBuffer(matrix);
			if (outdoorTemperature > 0)
			{
				matrix[1] = 0b0100000000000000;
				matrix[2] = 0b1110000000000000;
				matrix[3] = 0b0100000000000000;
			}
			if (outdoorTemperature < 0)
			{
				matrix[2] = 0b1110000000000000;
			}
			renderer.setSmallText(String(outdoorTemperature), TEXT_POS_BOTTOM, matrix);
			break;
		case MODE_EXT_HUMIDITY:
			renderer.clearScreenBuffer(matrix);
			if (outdoorHumidity != 100) renderer.setSmallText(String(outdoorHumidity), TEXT_POS_TOP, matrix);
			else
			{
				matrix[0] = 0b0010111011100000;
				matrix[1] = 0b0110101010100000;
				matrix[2] = 0b0010101010100000;
				matrix[3] = 0b0010101010100000;
				matrix[4] = 0b0010111011100000;
			}
			matrix[6] = 0b0100100000000000;
			matrix[7] = 0b0001000000000000;
			matrix[8] = 0b0010000000000000;
			matrix[9] = 0b0100100000000000;
			break;
#ifdef BUZZER
		case MODE_TIMER:
			renderer.clearScreenBuffer(matrix);
			renderer.setSmallText("TI", TEXT_POS_TOP, matrix);
			renderer.setSmallText(String(alarmTimer), TEXT_POS_BOTTOM, matrix);
			break;
#endif
#ifdef SHOW_MODE_TEST
		case MODE_TEST:
			renderer.clearScreenBuffer(matrix);
			if (testColumn == 10) testColumn = 0;
			matrix[testColumn] = 0b1111111111110000;
			testColumn++;
			break;
		case MODE_RED:
		case MODE_GREEN:
		case MODE_BLUE:
		case MODE_WHITE:
			renderer.setAllScreenBuffer(matrix);
			break;
#endif
		case MODE_BLANK:
			renderer.clearScreenBuffer(matrix);
			break;
		case MODE_FEED:
			for (uint8_t y = 0; y <= 5; y++)
			{
				renderer.clearScreenBuffer(matrix);
				for (uint8_t z = 0; z <= 6; z++)
				{
					matrix[2 + z] |= (lettersBig[feedText[feedPosition] - 32][z] << 11 + y) & 0b1111111111100000;
					matrix[2 + z] |= (lettersBig[feedText[feedPosition + 1] - 32][z] << 5 + y) & 0b1111111111100000;
					matrix[2 + z] |= (lettersBig[feedText[feedPosition + 2] - 32][z] << y - 1) & 0b1111111111100000;
				}
				writeScreenBuffer(matrix, feedColor, brightness);
				delay(FEED_SPEED);
			}
			feedPosition++;
			if (feedPosition == feedText.length() - 2)
			{
				feedPosition = 0;
				setMode(MODE_TIME);
			}
			break;
		}

#if defined(IR_LETTER_OFF)
		// Turn off LED behind IR-sensor
		renderer.unsetPixelInScreenBuffer(8, 9, matrix);
#endif

#ifdef DEBUG_MATRIX
		//debugScreenBuffer(matrixOld);
		debugScreenBuffer(matrix);
#endif

		// Write screenbuffer to display
		switch (mode)
		{
		case MODE_TIME:
		case MODE_BLANK:
			if (settings.getTransition() == TRANSITION_NORMAL) writeScreenBuffer(matrix, settings.getColor(), brightness);
			if (settings.getTransition() == TRANSITION_FADE) writeScreenBufferFade(matrixOld, matrix, settings.getColor(), brightness);
			if (settings.getTransition() == TRANSITION_MOVEUP)
			{
				if (minute() % 5 == 0) moveScreenBufferUp(matrixOld, matrix, settings.getColor(), brightness);
				else writeScreenBuffer(matrix, settings.getColor(), brightness);
			}
			break;
#ifdef SHOW_MODE_TEST
		case MODE_RED:
			writeScreenBuffer(matrix, RED, BRIGHTNESS_SELFTEST);
			break;
		case MODE_GREEN:
			writeScreenBuffer(matrix, GREEN, BRIGHTNESS_SELFTEST);
			break;
		case MODE_BLUE:
			writeScreenBuffer(matrix, BLUE, BRIGHTNESS_SELFTEST);
			break;
		case MODE_WHITE:
			writeScreenBuffer(matrix, WHITE, BRIGHTNESS_SELFTEST);
			break;
#endif
		case MODE_FEED:
			writeScreenBuffer(matrix, feedColor, brightness);
			break;
		default:
			if (runTransitionOnce)
			{
				//moveScreenBufferUp(matrixOld, matrix, settings.getColor(), brightness);
				if (settings.getTransition() == TRANSITION_NORMAL) writeScreenBuffer(matrix, settings.getColor(), brightness);
				if (settings.getTransition() == TRANSITION_FADE) writeScreenBufferFade(matrixOld, matrix, settings.getColor(), brightness);
				if (settings.getTransition() == TRANSITION_MOVEUP) moveScreenBufferUp(matrixOld, matrix, settings.getColor(), brightness);
				runTransitionOnce = false;
				testColumn = 0;
			}
			else writeScreenBuffer(matrix, settings.getColor(), brightness);
			break;
		}
	}

	// Wait for mode timeout then switch back to time
	if ((millis() > (modeTimeout + settings.getTimeout() * 1000)) && modeTimeout) setMode(MODE_TIME);

#ifdef DEBUG_FPS
	debugFps();
#endif
} // loop()

/******************************************************************************
  Transitions
******************************************************************************/

void moveScreenBufferUp(uint16_t screenBufferOld[], uint16_t screenBufferNew[], uint8_t color, uint8_t brightness)
{
	for (uint8_t z = 0; z <= 9; z++)
	{
		for (uint8_t i = 0; i <= 8; i++) screenBufferOld[i] = screenBufferOld[i + 1];
		screenBufferOld[9] = screenBufferNew[z];
		writeScreenBuffer(screenBufferOld, color, brightness);
		esp8266WebServer.handleClient();
		delay(50);
	}
}

void writeScreenBuffer(uint16_t screenBuffer[], uint8_t color, uint8_t brightness)
{
	ledDriver.clear();
	for (uint8_t y = 0; y <= 9; y++)
	{
		for (uint8_t x = 0; x <= 10; x++)
		{
			if (bitRead(screenBuffer[y], 15 - x)) ledDriver.setPixel(x, y, color, brightness);
		}
	}

	// Corner LEDs
	for (uint8_t y = 0; y <= 3; y++)
	{
		if (bitRead(screenBuffer[y], 4)) ledDriver.setPixel(110 + y, color, brightness);
	}

#ifdef BUZZER
	// Alarm LED
	if (bitRead(screenBuffer[4], 4))
	{
#ifdef ALARM_LED_COLOR
#ifdef ABUSE_CORNER_LED_FOR_ALARM
		if (settings.getAlarm1() || settings.getAlarm2() || alarmTimerSet) ledDriver.setPixel(111, ALARM_LED_COLOR, brightness);
		else if (bitRead(screenBuffer[1], 4)) ledDriver.setPixel(111, color, brightness);
#else
		ledDriver.setPixel(114, ALARM_LED_COLOR, brightness);
#endif
#else
		ledDriver.setPixel(114, color, brightness);
#endif
	}
#endif

	ledDriver.show();
}

void writeScreenBufferFade(uint16_t screenBufferOld[], uint16_t screenBufferNew[], uint8_t color, uint8_t brightness)
{
	ledDriver.clear();
	uint8_t brightnessBuffer[10][12] = {};

	for (uint8_t y = 0; y <= 9; y++)
	{
		for (uint8_t x = 0; x <= 11; x++)
		{
			if (bitRead(screenBufferOld[y], 15 - x)) brightnessBuffer[y][x] = brightness;
		}
	}
	for (uint8_t i = 0; i < brightness; i++)
	{
		for (uint8_t y = 0; y <= 9; y++)
		{
			for (uint8_t x = 0; x <= 11; x++)
			{
				if (!(bitRead(screenBufferOld[y], 15 - x)) && (bitRead(screenBufferNew[y], 15 - x))) brightnessBuffer[y][x]++;
				if ((bitRead(screenBufferOld[y], 15 - x)) && !(bitRead(screenBufferNew[y], 15 - x))) brightnessBuffer[y][x]--;
				ledDriver.setPixel(x, y, color, brightnessBuffer[y][x]);
			}
		}

		// Corner LEDs
		for (uint8_t y = 0; y <= 3; y++) ledDriver.setPixel(110 + y, color, brightnessBuffer[y][11]);

#ifdef BUZZER
		// Alarm LED
#ifdef ALARM_LED_COLOR
#ifdef ABUSE_CORNER_LED_FOR_ALARM
		if (settings.getAlarm1() || settings.getAlarm2() || alarmTimerSet) ledDriver.setPixel(111, ALARM_LED_COLOR, brightnessBuffer[4][11]);
		else ledDriver.setPixel(111, color, brightnessBuffer[1][11]);
#else
		ledDriver.setPixel(114, ALARM_LED_COLOR, brightnessBuffer[4][11]);
#endif
#else
		ledDriver.setPixel(114, color, brightnessBuffer[4][11]);
#endif
#endif
		esp8266WebServer.handleClient();
		delay(1500 / brightness);
		ledDriver.show();
	}
}

/******************************************************************************
  "On/off" pressed
******************************************************************************/

void buttonOnOffPressed()
{
#ifdef DEBUG
	Serial.println("On/off pressed.");
#endif

	mode == MODE_BLANK ? setLedsOn() : setLedsOff();
}

/******************************************************************************
  "Time" pressed
******************************************************************************/

void buttonTimePressed()
{
#ifdef DEBUG
	Serial.println("Time pressed.");
#endif

#ifdef BUZZER
	// Switch off alarm
	if (alarmOn)
	{
#ifdef DEBUG
		Serial.println("Alarm: off");
#endif
		digitalWrite(PIN_BUZZER, LOW);
		alarmOn = false;
	}
#endif

	modeTimeout = 0;
	//renderer.clearScreenBuffer(matrix);
	setMode(MODE_TIME);
}

/******************************************************************************
  "Mode" pressed
******************************************************************************/

void buttonModePressed()
{
#ifdef DEBUG
	Serial.println("Mode pressed.");
#endif

#ifdef BUZZER
	// Switch off alarm
	if (alarmOn)
	{
#ifdef DEBUG
		Serial.println("Alarm: off");
#endif
		digitalWrite(PIN_BUZZER, LOW);
		alarmOn = false;
		setMode(MODE_TIME);
		return;
	}
#endif

	setMode(mode++);
}

/******************************************************************************
  Set mode
******************************************************************************/

void setMode(Mode newMode)
{
	screenBufferNeedsUpdate = true;
	runTransitionOnce = true;
	lastMode = mode;
	mode = newMode;

	// Set timeout
	switch (mode)
	{
#ifdef SHOW_MODE_AMPM
	case MODE_AMPM:
#endif
#ifdef SHOW_MODE_SECONDS
	case MODE_SECONDS:
#endif
#ifdef SHOW_MODE_WEEKDAY
	case MODE_WEEKDAY:
#endif
#ifdef SHOW_MODE_DATE
	case MODE_DATE:
#endif
#ifdef SHOW_MODE_MOONPHASE
	case MODE_MOONPHASE:
#endif
#if defined(RTC_BACKUP) && !defined(SENSOR_DHT22)
	case MODE_TEMP:
#endif
#ifdef SENSOR_DHT22
	case MODE_TEMP:
	case MODE_HUMIDITY:
#endif
	case MODE_EXT_TEMP:
	case MODE_EXT_HUMIDITY:
		modeTimeout = millis();
		break;
	default:
		modeTimeout = 0;
		break;
	}
}

#ifdef LDR
/******************************************************************************
  Get rated brightness from LDR
******************************************************************************/

void getBrightnessFromLdr()
{
#ifdef LDR_IS_INVERSE
	ldrValue = 1023 - analogRead(PIN_LDR);
#else
	ldrValue = analogRead(PIN_LDR);
#endif
	//Serial.println(ldrValue);
	if (ldrValue < minLdrValue) minLdrValue = ldrValue;
	if (ldrValue > maxLdrValue) maxLdrValue = ldrValue;
	if (settings.getUseAbc() && ((ldrValue >= (lastLdrValue + LDR_HYSTERESIS)) || (ldrValue <= (lastLdrValue - LDR_HYSTERESIS))))
	{
		lastLdrValue = ldrValue;
		if (minLdrValue != maxLdrValue) // The ESP will crash if minLdrValue and maxLdrValue are equal due to an error in map().
		{
			ratedBrightness = map(ldrValue, minLdrValue, maxLdrValue, MIN_BRIGHTNESS, abcBrightness);
		}
#ifdef DEBUG
		Serial.printf("Brightness: %u (min: %u, max: %u)\r\n", ratedBrightness, MIN_BRIGHTNESS, abcBrightness);
		Serial.printf("LDR: %u (min: %u, max: %u)\r\n", ldrValue, minLdrValue, maxLdrValue);
#endif
	}
}
#endif

#if defined(UPDATE_INFO_STABLE) || defined(UPDATE_INFO_UNSTABLE)
/******************************************************************************
  Get update info
******************************************************************************/

void getUpdateInfo()
{
#ifdef DEBUG
	Serial.println("Sending HTTP-request for update info.");
#endif
	char server[] = UPDATE_INFOSERVER;
	WiFiClient wifiClient;
	HttpClient client = HttpClient(wifiClient, server, 80);
	client.get(UPDATE_INFOFILE);
	uint16_t statusCode = client.responseStatusCode();
	if (statusCode == 200)
	{
		String response = client.responseBody();
		response = response.substring(response.indexOf('{'), response.lastIndexOf('}') + 1);
#ifdef DEBUG
		Serial.printf("Status: %u\r\n", statusCode);
		Serial.printf("Response is %u bytes.\r\n", response.length());
		Serial.println(response);
		Serial.println("Parsing JSON.");
#endif
		StaticJsonBuffer<256> jsonBuffer;
		JsonObject &responseJson = jsonBuffer.parseObject(response);
		if (responseJson.success())
		{
#ifdef UPDATE_INFO_STABLE
			updateInfo = responseJson["channel"]["stable"]["version"].as<String>();
#endif
#ifdef UPDATE_INFO_UNSTABLE
			updateInfo = responseJson["channel"]["unstable"]["version"].as<String>();
#endif
#ifdef DEBUG
			updateInfo > String(FIRMWARE_VERSION) ? Serial.println("Firmwareupdate available! (" + updateInfo + ")") : Serial.println("Firmware is uptodate.");
#endif
			return;
		}
	}
#ifdef DEBUG
	else Serial.printf("Status: %u\r\n", statusCode);
	Serial.println("Error (" + String(UPDATE_INFOSERVER) + ")");
#endif
}
#endif

/******************************************************************************
  Get outdoor conditions from Yahoo
******************************************************************************/

void getOutdoorConditions(String location)
{
#ifdef DEBUG
	Serial.println("Sending HTTP-request for weather.");
#endif
	location.replace(" ", "%20");
	location.replace(",", "%2C");
	char server[] = "query.yahooapis.com";
	WiFiClient wifiClient;
	HttpClient client = HttpClient(wifiClient, server, 80);
	String sqlQuery = "select%20atmosphere.humidity%2C%20item.title%2C%20item.condition.temp%2C%20item.condition.code%20";
	sqlQuery += "from%20weather.forecast%20where%20woeid%20in%20";
	sqlQuery += "(select%20woeid%20from%20geo.places(1)%20where%20text=%22" + location + "%22)%20";
	sqlQuery += "and%20u=%27c%27";
	client.get("query.yahooapis.com/v1/public/yql?q=" + sqlQuery + "&format=json");
	uint16_t statusCode = client.responseStatusCode();
	if (statusCode == 200)
	{
		String response = client.responseBody();
		response = response.substring(response.indexOf('{'), response.lastIndexOf('}') + 1);
#ifdef DEBUG
		Serial.printf("Status: %u\r\n", statusCode);
		Serial.printf("Response is %u bytes.\r\n", response.length());
		Serial.println(response);
		Serial.println("Parsing JSON.");
#endif
		StaticJsonBuffer<512> jsonBuffer;
		JsonObject &responseJson = jsonBuffer.parseObject(response);
		if (responseJson.success() && responseJson["query"]["count"].as<int8_t>())
		{
			outdoorTitle = responseJson["query"]["results"]["channel"]["item"]["title"].as<String>();
			outdoorTitle = outdoorTitle.substring(0, outdoorTitle.indexOf(" at "));
			outdoorTemperature = responseJson["query"]["results"]["channel"]["item"]["condition"]["temp"].as<int8_t>();
			outdoorHumidity = responseJson["query"]["results"]["channel"]["atmosphere"]["humidity"].as<uint8_t>();
			outdoorCode = responseJson["query"]["results"]["channel"]["item"]["condition"]["code"].as<uint8_t>();
#ifdef DEBUG
			Serial.println(outdoorTitle);
			Serial.printf("Temperature (Yahoo): %dC\r\n", outdoorTemperature);
			Serial.printf("Humidity (Yahoo): %u%%\r\n", outdoorHumidity);
			Serial.println("Condition (Yahoo): " + sWeatherCondition[outdoorCode] + " (" + String(outdoorCode) + ")");
#endif
			errorCounterYahoo = 0;
			return;
		}
	}
#ifdef DEBUG
	else Serial.printf("Status: %u\r\n", statusCode);
	outdoorTitle = "Request failed.";
#endif
	if (errorCounterYahoo < 255) errorCounterYahoo++;
#ifdef DEBUG
	Serial.printf("Error (Yahoo): %u\r\n", errorCounterYahoo);
#endif
}

#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
/******************************************************************************
  Get room conditions
******************************************************************************/

void getRoomConditions()
{
#if defined(RTC_BACKUP) && !defined(SENSOR_DHT22)
	roomTemperature = RTC.temperature() / 4.0 + RTC_TEMPERATURE_OFFSET;
#ifdef DEBUG
	Serial.println("Temperature (RTC): " + String(roomTemperature) + "C");
#endif
#endif
#ifdef SENSOR_DHT22
	float dhtTemperature = dht.readTemperature();
	float dhtHumidity = dht.readHumidity();
	if (!isnan(dhtTemperature) && !isnan(dhtHumidity))
	{
		errorCounterDht = 0;
		roomTemperature = dhtTemperature + DHT_TEMPERATURE_OFFSET;
		roomHumidity = dhtHumidity + DHT_HUMIDITY_OFFSET;
#ifdef DEBUG
		Serial.println("Temperature (DHT): " + String(roomTemperature) + "C");
		Serial.println("Humidity (DHT): " + String(roomHumidity) + "%");
#endif
	}
	else
	{
		if (errorCounterDht < 255) errorCounterDht++;
#ifdef DEBUG
		Serial.printf("Error (DHT): %u\r\n", errorCounterDht);
#endif
	}
#endif
}
#endif

/******************************************************************************
  Get UTC time from NTP
******************************************************************************/

time_t getNtpTime(const char server[])
{
#ifdef DEBUG
	Serial.println("Sending NTP request to \"" + String(server) + "\". ");
#endif
	uint8_t packetBuffer[49] = {};
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
	IPAddress timeServerIP;
	WiFi.hostByName(server, timeServerIP);
	wifiUdp.beginPacket(timeServerIP, 123);
	wifiUdp.write(packetBuffer, 48);
	wifiUdp.endPacket();
	uint32_t beginWait = millis();
	while ((millis() - beginWait) < NTP_TIMEOUT)
	{
		yield();
		if (wifiUdp.parsePacket() >= 48)
		{
			wifiUdp.read(packetBuffer, 48);
			uint32_t ntpTime = (packetBuffer[40] << 24) + (packetBuffer[41] << 16) + (packetBuffer[42] << 8) + packetBuffer[43];
			// NTP time is seconds from 1900, TimeLib.h needs seconds from 1970
			ntpTime -= 2208988800;
			errorCounterNtp = 0;
#ifdef DEBUG
			Serial.printf("Time (NTP): %02u:%02u:%02u %02u.%02u.%04u (UTC)\r\n", hour(ntpTime), minute(ntpTime), second(ntpTime), day(ntpTime), month(ntpTime), year(ntpTime));
#endif
			return ntpTime;
		}
	}
	if (errorCounterNtp < 255) errorCounterNtp++;
#ifdef DEBUG
	Serial.printf("Error (NTP): %u\r\n", errorCounterNtp);
#endif
	return 0;
}

/******************************************************************************
  Misc
******************************************************************************/

#ifdef MODE_BUTTON
void buttonModeInterrupt()
{
	if (millis() > lastButtonPress + 250)
	{
		lastButtonPress = millis();
		buttonModePressed();
	}
}
#endif

#ifdef ONOFF_BUTTON
void buttonOnOffInterrupt()
{
	if (millis() > lastButtonPress + 250)
	{
		lastButtonPress = millis();
		buttonOnOffPressed();
	}
}
#endif

#ifdef TIME_BUTTON
void buttonTimeInterrupt()
{
	if (millis() > lastButtonPress + 250)
	{
		lastButtonPress = millis();
		buttonTimePressed();
	}
}
#endif

// Switch off LEDs
void setLedsOff()
{
#ifdef DEBUG
	Serial.println("LEDs: off");
#endif
	setMode(MODE_BLANK);
}

// Switch on LEDs
void setLedsOn()
{
#ifdef DEBUG
	Serial.println("LEDs: on");
#endif
	setMode(lastMode);
}

#ifdef SHOW_MODE_MOONPHASE
// Calculate moonphase
int getMoonphase(int y, int m, int d)
{
	int b;
	int c;
	int e;
	double jd;
	if (m < 3)
	{
		y--;
		m += 12;
	}
	++m;
	c = 365.25 * y;
	e = 30.6 * m;
	jd = c + e + d - 694039.09; // jd is total days elapsed
	jd /= 29.53;                // divide by the moon cycle (29.53 days)
	b = jd;		                // int(jd) -> b, take integer part of jd
	jd -= b;		            // subtract integer part to leave fractional part of original jd
	b = jd * 8 + 0.5;	        // scale fraction from 0-8 and round by adding 0.5
	b = b & 7;		            // 0 and 8 are the same so turn 8 into 0
	return b;
}
#endif

#ifdef DEBUG_MATRIX
// Write screenbuffer to console
void debugScreenBuffer(uint16_t screenBuffer[])
{
	const char buchstabensalat[][12] =
	{
	  { 'E', 'S', 'K', 'I', 'S', 'T', 'A', 'F', 'U', 'N', 'F', '1' },
	  { 'Z', 'E', 'H', 'N', 'Z', 'W', 'A', 'N', 'Z', 'I', 'G', '2' },
	  { 'D', 'R', 'E', 'I', 'V', 'I', 'E', 'R', 'T', 'E', 'L', '3' },
	  { 'V', 'O', 'R', 'F', 'U', 'N', 'K', 'N', 'A', 'C', 'H', '4' },
	  { 'H', 'A', 'L', 'B', 'A', 'E', 'L', 'F', 'U', 'N', 'F', 'A' },
	  { 'E', 'I', 'N', 'S', 'X', 'A', 'M', 'Z', 'W', 'E', 'I', ' ' },
	  { 'D', 'R', 'E', 'I', 'P', 'M', 'J', 'V', 'I', 'E', 'R', ' ' },
	  { 'S', 'E', 'C', 'H', 'S', 'N', 'L', 'A', 'C', 'H', 'T', ' ' },
	  { 'S', 'I', 'E', 'B', 'E', 'N', 'Z', 'W', 'O', 'L', 'F', ' ' },
	  { 'Z', 'E', 'H', 'N', 'E', 'U', 'N', 'K', 'U', 'H', 'R', ' ' }
	};
	//Serial.println("\033[0;0H"); // set cursor to 0, 0 position
	Serial.println(" -----------");
	for (uint8_t y = 0; y <= 9; y++)
	{
		Serial.print('|');
		for (uint8_t x = 0; x <= 10; x++)
		{
			Serial.print((bitRead(screenBuffer[y], 15 - x) ? buchstabensalat[y][x] : ' '));
		}
		Serial.print('|');
		Serial.println((bitRead(screenBuffer[y], 4) ? buchstabensalat[y][11] : ' '));
	}
	Serial.println(" -----------");
}
#endif

#ifdef DEBUG_FPS
// Write FPS to console
void debugFps()
{
	static uint16_t frames;
	static uint32_t lastFpsCheck;
	frames++;
	if ((millis() % 1000 == 0) && (millis() != lastFpsCheck))
	{
		lastFpsCheck = millis();
		Serial.printf("FPS: %u\r\n", frames);
		frames = 0;
	}
}
#endif

/******************************************************************************
  Webserver
******************************************************************************/

void setupWebServer()
{
	esp8266WebServer.onNotFound(handleNotFound);
	esp8266WebServer.on("/", handleRoot);
	esp8266WebServer.on("/handleButtonOnOff", []() { buttonOnOffPressed(); callRoot(); });
	esp8266WebServer.on("/handleButtonSettings", handleButtonSettings);
	esp8266WebServer.on("/handleButtonMode", []() { buttonModePressed(); callRoot(); });
	esp8266WebServer.on("/handleButtonTime", []() {	buttonTimePressed(); callRoot(); });
	esp8266WebServer.on("/commitSettings", handleCommitSettings);
	esp8266WebServer.on("/reset", handleReset);
	esp8266WebServer.on("/setEvent", handleSetEvent);
	esp8266WebServer.on("/showText", handleShowText);
	esp8266WebServer.begin();
}

void callRoot()
{
	esp8266WebServer.send(200, "text/html", "<!doctype html><html><head><script>window.onload=function(){window.location.replace('/');}</script></head></html>");
}

// Page 404
void handleNotFound()
{
	esp8266WebServer.send(404, "text/plain", "404 - File Not Found.");
}

// Page /
void handleRoot()
{
	String message = "<!doctype html>"
		"<html>"
		"<head>"
		"<title>" + String(HOSTNAME) + "</title>"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		"<meta http-equiv=\"refresh\" content=\"60\" charset=\"UTF-8\">"
		"<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">"
		"<style>"
		"body{background-color:#FFFFFF;text-align:center;color:#333333;font-family:Sans-serif;font-size:16px;}"
		"button{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:10px;border:5px solid #FFFFFF;font-size:24px;border-radius:10px;}"
		"</style>"
		"</head>"
		"<body>"
		"<h1>" + String(HOSTNAME) + "</h1>";
#ifdef DEDICATION
	message += DEDICATION;
	message += "<br><br>";
#endif
	if (mode == MODE_BLANK) message += "<button title=\"Switch LEDs on\" onclick=\"window.location.href='/handleButtonOnOff'\"><i class=\"fa fa-toggle-off\"></i></button>";
	else message += "<button title=\"Switch LEDs off\" onclick=\"window.location.href='/handleButtonOnOff'\"><i class=\"fa fa-toggle-on\"></i></button>";
	message += "<button title=\"Settings\" onclick=\"window.location.href='/handleButtonSettings'\"><i class=\"fa fa-gear\"></i></button>"
		"<br><br>"
		"<button title=\"Switch modes\" onclick=\"window.location.href='/handleButtonMode'\"><i class=\"fa fa-bars\"></i></button>"
		"<button title=\"Return to time\" onclick=\"window.location.href='/handleButtonTime'\"><i class=\"fa fa-clock-o\"></i></button>";
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
	message += "<br><br><i class = \"fa fa-home\" style=\"font-size:20px;\"></i>"
		"<br><i class=\"fa fa-thermometer\" style=\"font-size:20px;\"></i> " + String(roomTemperature) + "&deg;C / " + String(roomTemperature * 9.0 / 5.0 + 32.0) + "&deg;F";
#endif
#ifdef SENSOR_DHT22
	message += "<br><i class=\"fa fa-tint\" style=\"font-size:20px;\"></i> " + String(roomHumidity) + "%RH"
		"<br><span style=\"font-size:20px;\">";
	if (roomHumidity < 30) message += "<i style=\"color:Red;\" class=\"fa fa-square\"\"></i>";
	else message += "<i style=\"color:Red;\" class=\"fa fa-square-o\"></i>";
	if ((roomHumidity >= 30) && (roomHumidity < 40)) message += "&nbsp;<i style=\"color:Orange;\" class=\"fa fa-square\"></i>";
	else message += "&nbsp;<i style=\"color:Orange;\" class=\"fa fa-square-o\"></i>";
	if ((roomHumidity >= 40) && (roomHumidity <= 50)) message += "&nbsp;<i style=\"color:MediumSeaGreen;\" class=\"fa fa-square\"></i>";
	else message += "&nbsp;<i style=\"color:MediumSeaGreen;\" class=\"fa fa-square-o\"></i>";
	if ((roomHumidity > 50) && (roomHumidity < 60)) message += "&nbsp;<i style=\"color:Lightblue;\" class=\"fa fa-square\"></i>";
	else message += "&nbsp;<i style=\"color:Lightblue;\" class=\"fa fa-square-o\"></i>";
	if (roomHumidity >= 60) message += "&nbsp;<i style=\"color:Blue;\" class=\"fa fa-square\"></i>";
	else message += "&nbsp;<i style=\"color:Blue;\" class=\"fa fa-square-o\"></i>";
	message += "</span>";
#endif
	if (WiFi.isConnected())
	{
		message += "<br><br><i class = \"fa fa-tree\" style=\"font-size:20px;\"></i>"
			"<br><i class = \"fa fa-thermometer\" style=\"font-size:20px;\"></i> " + String(outdoorTemperature) + "&deg;C / " + String(outdoorTemperature * 9.0 / 5.0 + 32.0) + "&deg;F"
			"<br><i class = \"fa fa-tint\" style=\"font-size:20px;\"></i> " + String(outdoorHumidity) + "%RH"
			"<br><span class = \"";
		switch (outdoorCode)
		{
		case 0:  // tornado
		case 1:  // tropical storm
		case 2:  // hurricane
			message += "fa fa-frown-o";
			break;
		case 3:  // severe thunderstorms
		case 4:  // thunderstorms
		case 37: // isolated thunderstorms
		case 38: // scattered thunderstorms
		case 39: // scattered thunderstorms
		case 45: // thundershowers
		case 47: // isolated thundershowers
			message += "fa fa-flash";
			break;
		case 5:  // mixed rain and snow
		case 6:  // mixed rain and sleet
		case 7:  // mixed snow and sleet
		case 8:  // freezing drizzle
		case 9:  // drizzle
		case 10: // freezing rain
		case 11: // showers
		case 12: // showers
			message += "fa fa-umbrella";
			break;
		case 13: // snow flurries
		case 14: // light snow showers
		case 15: // blowing snow
		case 16: // snow
		case 17: // hail
		case 41: // heavy snow
		case 42: // scattered snow showers
		case 43: // heavy snow
		case 46: // snow showers
			message += "fa fa-snowflake-o";
			break;
		case 23: // blustery
		case 24: // windy
			message += "fa fa-flag";
			break;
		case 31: // clear (night)
		case 33: // fair (night)
			message += "fa fa-moon-o";
			break;
		case 32: // sunny
		case 34: // fair (day)
		case 36: // hot
			message += "fa fa-sun-o";
			break;
		default:
			message += "fa fa-cloud";
			break;
		}
		message += "\" style=\"font-size:20px;\"></span> " + sWeatherCondition[outdoorCode];
	}
	message += "<span style=\"font-size:12px;\">"
		"<br><br><a href=\"http://tmw-it.ch/qlockwork/\">Qlockwork</a> was <i class=\"fa fa-code\"></i> with <i class=\"fa fa-heart\"></i> by tmw-it.ch"
		"<br>Firmware: " + String(FIRMWARE_VERSION);
#if defined(UPDATE_INFO_STABLE) || defined(UPDATE_INFO_UNSTABLE)
	if (updateInfo > String(FIRMWARE_VERSION)) message += "<br><span style=\"color:red;\">Firmwareupdate available! (" + updateInfo + ")</span>";
#endif
#ifdef DEBUG_WEB
	time_t tempEspTime = now();
	message += "<br><br>Time: " + String(hour(tempEspTime)) + ":";
	if (minute(tempEspTime) < 10) message += "0";
	message += String(minute(tempEspTime));
	if (timeZone.locIsDST(now())) message += " (DST)";
	message += " up " + String(int(upTime / 86400)) + " days, " + String(hour(upTime)) + ":";
	if (minute(upTime) < 10) message += "0";
	message += String(minute(upTime));
	message += "<br>" + String(dayStr(weekday(tempEspTime))) + ", " + String(monthStr(month(tempEspTime))) + " " + String(day(tempEspTime)) + ". " + String(year(tempEspTime));
	message += "<br>Moonphase: " + String(moonphase);
	message += "<br>Free RAM: " + String(system_get_free_heap_size()) + " bytes";
#ifdef LDR
	message += "<br>Brightness: " + String(ratedBrightness) + " (ABC: ";
	settings.getUseAbc() ? message += "enabled" : message += "disabled";
	message += ", min: " + String(MIN_BRIGHTNESS) + ", max : " + String(abcBrightness) + ")";
	message += "<br>LDR: " + String(ldrValue) + " (min: " + String(minLdrValue) + ", max: " + String(maxLdrValue) + ")";
#endif
	message += "<br>Error (NTP): " + String(errorCounterNtp);
#ifdef SENSOR_DHT22
	message += "<br>Error (DHT): " + String(errorCounterDht);
#endif
	message += "<br>Error (Yahoo): " + String(errorCounterYahoo) +
		"<br>Weather: " + String(outdoorTitle) +
		"<br>Reset reason: " + ESP.getResetReason() +
		"<br>Flags: ";
#ifdef RTC_BACKUP
	message += "RTC ";
#else
	message += "<s>RTC</s> ";
#endif
#ifdef SENSOR_DHT22
	message += "DHT22 ";
#else
	message += "<s>DHT22</s> ";
#endif
#ifdef LDR
	message += "LDR ";
#else
	message += "<s>LDR</s> ";
#endif
#ifdef BUZZER
	message += "BUZZER ";
#else
	message += "<s>BUZZER</s> ";
#endif
#ifdef IR_RECEIVER
	message += "IR_RECEIVER ";
#else
	message += "<s>IR_RECEIVER</s> ";
#endif
#ifdef ESP_LED
	message += "ESP_LED ";
#else
	message += "<s>ESP_LED</s> ";
#endif
#endif
	message += "</span></body></html>";
	esp8266WebServer.send(200, "text/html", message);
}

// Page settings
void handleButtonSettings()
{
#ifdef DEBUG
	Serial.println("Settings pressed.");
#endif
	String message = "<!doctype html>"
		"<html>"
		"<head>"
		"<title>" + String(HOSTNAME) + " Settings</title>"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		"<meta charset=\"UTF-8\">"
		"<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">"
		"<style>"
		"body{background-color:#FFFFFF;text-align:center;color:#333333;font-family:Sans-serif;font-size:16px;}"
		"input[type=submit]{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:12px;border:5px solid #FFFFFF;font-size:20px;border-radius:10px;}"
		"table{border-collapse:collapse;margin:0px auto;} td{padding:12px;border-bottom:1px solid #ddd;} tr:first-child{border-top:1px solid #ddd;} td:first-child{text-align:right;} td:last-child{text-align:left;}"
		"select{font-size:16px;}"
		"button{background-color:#1FA3EC;text-align:center;color:#FFFFFF;width:200px;padding:10px;border:5px solid #FFFFFF;font-size:24px;border-radius:10px;}"
		"</style>"
		"</head>"
		"<body>"
		"<h1>" + String(HOSTNAME) + " Settings</h1>"
		"<form action=\"/commitSettings\">"
		"<table>";
	// ------------------------------------------------------------------------
#ifdef BUZZER
	message += "<tr><td>"
		"Alarm 1:"
		"</td><td>"
		"<input type=\"radio\" name=\"a1\" value=\"1\"";
	if (settings.getAlarm1()) message += " checked";
	message += "> on "
		"<input type=\"radio\" name=\"a1\" value=\"0\"";
	if (!settings.getAlarm1()) message += " checked";
	message += "> off&nbsp;&nbsp;&nbsp;"
		"<input type=\"time\" name=\"a1t\" value=\"";
	if (hour(settings.getAlarm1Time()) < 10) message += "0";
	message += String(hour(settings.getAlarm1Time())) + ":";
	if (minute(settings.getAlarm1Time()) < 10) message += "0";
	message += String(minute(settings.getAlarm1Time())) + "\">"
		" h<br><br>"
		"<input type=\"checkbox\" name=\"a1w2\" value=\"4\"";
	if (bitRead(settings.getAlarm1Weekdays(), 2)) message += " checked";
	message += "> Mo. "
		"<input type=\"checkbox\" name=\"a1w3\" value=\"8\"";
	if (bitRead(settings.getAlarm1Weekdays(), 3)) message += " checked";
	message += "> Tu. "
		"<input type=\"checkbox\" name=\"a1w4\" value=\"16\"";
	if (bitRead(settings.getAlarm1Weekdays(), 4)) message += " checked";
	message += "> We. "
		"<input type=\"checkbox\" name=\"a1w5\" value=\"32\"";
	if (bitRead(settings.getAlarm1Weekdays(), 5)) message += " checked";
	message += "> Th. "
		"<input type=\"checkbox\" name=\"a1w6\" value=\"64\"";
	if (bitRead(settings.getAlarm1Weekdays(), 6)) message += " checked";
	message += "> Fr. "
		"<input type=\"checkbox\" name=\"a1w7\" value=\"128\"";
	if (bitRead(settings.getAlarm1Weekdays(), 7)) message += " checked";
	message += "> Sa. "
		"<input type=\"checkbox\" name=\"a1w1\" value=\"2\"";
	if (bitRead(settings.getAlarm1Weekdays(), 1)) message += " checked";
	message += "> Su. "
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Alarm 2:"
		"</td><td>"
		"<input type=\"radio\" name=\"a2\" value=\"1\"";
	if (settings.getAlarm2()) message += " checked";
	message += "> on "
		"<input type=\"radio\" name=\"a2\" value=\"0\"";
	if (!settings.getAlarm2()) message += " checked";
	message += "> off&nbsp;&nbsp;&nbsp;"
		"<input type=\"time\" name=\"a2t\" value=\"";
	if (hour(settings.getAlarm2Time()) < 10) message += "0";
	message += String(hour(settings.getAlarm2Time())) + ":";
	if (minute(settings.getAlarm2Time()) < 10) message += "0";
	message += String(minute(settings.getAlarm2Time())) + "\">"
		" h<br><br>"
		"<input type=\"checkbox\" name=\"a2w2\" value=\"4\"";
	if (bitRead(settings.getAlarm2Weekdays(), 2)) message += " checked";
	message += "> Mo. "
		"<input type=\"checkbox\" name=\"a2w3\" value=\"8\"";
	if (bitRead(settings.getAlarm2Weekdays(), 3)) message += " checked";
	message += "> Tu. "
		"<input type=\"checkbox\" name=\"a2w4\" value=\"16\"";
	if (bitRead(settings.getAlarm2Weekdays(), 4)) message += " checked";
	message += "> We. "
		"<input type=\"checkbox\" name=\"a2w5\" value=\"32\"";
	if (bitRead(settings.getAlarm2Weekdays(), 5)) message += " checked";
	message += "> Th. "
		"<input type=\"checkbox\" name=\"a2w6\" value=\"64\"";
	if (bitRead(settings.getAlarm2Weekdays(), 6)) message += " checked";
	message += "> Fr. "
		"<input type=\"checkbox\" name=\"a2w7\" value=\"128\"";
	if (bitRead(settings.getAlarm2Weekdays(), 7)) message += " checked";
	message += "> Sa. "
		"<input type=\"checkbox\" name=\"a2w1\" value=\"2\"";
	if (bitRead(settings.getAlarm2Weekdays(), 1)) message += " checked";
	message += "> Su. "
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Timer:"
		"</td><td>"
		"<select name=\"ti\">";
	for (int i = 0; i <= 10; i++)
	{
		message += "<option value=\"" + String(i) + "\">";
		if (i < 10) message += "0";
		message += String(i) + "</option>";
	}
	message += "<option value=\"15\">15</option>"
		"<option value=\"20\">20</option>"
		"<option value=\"25\">25</option>"
		"<option value=\"30\">30</option>"
		"<option value=\"45\">45</option>"
		"<option value=\"60\">60</option>"
		"</select> minutes"
		"</td></tr>";
#endif
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Hourly beep:"
		"</td><td>"
		"<input type=\"radio\" name=\"hb\" value=\"1\"";
	if (settings.getHourBeep()) message += " checked";
	message += "> on "
		"<input type=\"radio\" name=\"hb\" value=\"0\"";
	if (!settings.getHourBeep()) message += " checked";
	message += "> off"
		"</td></tr>";
	// ------------------------------------------------------------------------
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
	message += "<tr><td>"
		"Show temperature:"
		"</td><td>"
		"<input type=\"radio\" name=\"mc\" value=\"1\"";
	if (settings.getModeChange()) message += " checked";
	message += "> on "
		"<input type=\"radio\" name=\"mc\" value=\"0\"";
	if (!settings.getModeChange()) message += " checked";
	message += "> off"
		"</td></tr>";
#endif
	// ------------------------------------------------------------------------
#if defined(LDR)
	message += "<tr><td>"
		"ABC:"
		"</td><td>"
		"<input type=\"radio\" name=\"ab\" value=\"1\"";
	if (settings.getUseAbc()) message += " checked";
	message += "> on "
		"<input type=\"radio\" name=\"ab\" value=\"0\"";
	if (!settings.getUseAbc()) message += " checked";
	message += "> off"
		"</td></tr>";
#endif
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Brightness:"
		"</td><td>"
		"<select name=\"br\">";
	for (int i = 10; i <= 100; i += 10)
	{
		message += "<option value=\"" + String(i) + "\"";
		if (i == settings.getBrightness()) message += " selected";
		message += ">";
		message += String(i) + "</option>";
	}
	message += "</select> %"
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Color:"
		"</td><td>"
		"<select name=\"co\">"
		"<option value=\"0\"";
	if (settings.getColor() == 0) message += " selected";
	message += ">"
		"White</option>"
		"<option value=\"1\"";
	if (settings.getColor() == 1) message += " selected";
	message += ">"
		"Red</option>"
		"<option value=\"2\"";
	if (settings.getColor() == 2) message += " selected";
	message += ">"
		"Red 75%</option>"
		"<option value=\"3\"";
	if (settings.getColor() == 3) message += " selected";
	message += ">"
		"Red 50%</option>"
		"<option value=\"4\"";
	if (settings.getColor() == 4) message += " selected";
	message += ">"
		"Orange</option>"
		"<option value=\"5\"";
	if (settings.getColor() == 5) message += " selected";
	message += ">"
		"Yellow</option>"
		"<option value=\"6\"";
	if (settings.getColor() == 6) message += " selected";
	message += ">"
		"Yellow 75%</option>"
		"<option value=\"7\"";
	if (settings.getColor() == 7) message += " selected";
	message += ">"
		"Yellow 50%</option>"
		"<option value=\"8\"";
	if (settings.getColor() == 8) message += " selected";
	message += ">"
		"Green-Yellow</option>"
		"<option value=\"9\"";
	if (settings.getColor() == 9) message += " selected";
	message += ">"
		"Green</option>"
		"<option value=\"10\"";
	if (settings.getColor() == 10) message += " selected";
	message += ">"
		"Green 75%</option>"
		"<option value=\"11\"";
	if (settings.getColor() == 11) message += " selected";
	message += ">"
		"Green 50%</option>"
		"<option value=\"12\"";
	if (settings.getColor() == 12) message += " selected";
	message += ">"
		"Mintgreen</option>"
		"<option value=\"13\"";
	if (settings.getColor() == 13) message += " selected";
	message += ">"
		"Cyan</option>"
		"<option value=\"14\"";
	if (settings.getColor() == 14) message += " selected";
	message += ">"
		"Cyan 75%</option>"
		"<option value=\"15\"";
	if (settings.getColor() == 15) message += " selected";
	message += ">"
		"Cyan 50%</option>"
		"<option value=\"16\"";
	if (settings.getColor() == 16) message += " selected";
	message += ">"
		"Light Blue</option>"
		"<option value=\"17\"";
	if (settings.getColor() == 17) message += " selected";
	message += ">"
		"Blue</option>"
		"<option value=\"18\"";
	if (settings.getColor() == 18) message += " selected";
	message += ">"
		"Blue 75%</option>"
		"<option value=\"19\"";
	if (settings.getColor() == 19) message += " selected";
	message += ">"
		"Blue 50%</option>"
		"<option value=\"20\"";
	if (settings.getColor() == 20) message += " selected";
	message += ">"
		"Violet</option>"
		"<option value=\"21\"";
	if (settings.getColor() == 21) message += " selected";
	message += ">"
		"Magenta</option>"
		"<option value=\"22\"";
	if (settings.getColor() == 22) message += " selected";
	message += ">"
		"Magenta 75%</option>"
		"<option value=\"23\"";
	if (settings.getColor() == 23) message += " selected";
	message += ">"
		"Magenta 50%</option>"
		"<option value=\"24\"";
	if (settings.getColor() == 24) message += " selected";
	message += ">"
		"Pink</option>"
		"</select>"
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Colorchange:"
		"</td><td>"
		"<input type=\"radio\" name=\"cc\" value=\"3\"";
	if (settings.getColorChange() == 3) message += " checked";
	message += "> day "
		"<input type=\"radio\" name=\"cc\" value=\"2\"";
	if (settings.getColorChange() == 2) message += " checked";
	message += "> hour "
		"<input type=\"radio\" name=\"cc\" value=\"1\"";
	if (settings.getColorChange() == 1) message += " checked";
	message += "> five "
		"<input type=\"radio\" name=\"cc\" value=\"0\"";
	if (settings.getColorChange() == 0) message += " checked";
	message += "> off"
		"</td></tr>";
	// ------------------------------------------------------------------------
#ifndef FRONTCOVER_BINARY
	message += "<tr><td>"
		"Transition:"
		"</td><td>"
		"<input type=\"radio\" name=\"tr\" value=\"2\"";
	if (settings.getTransition() == 2) message += " checked";
	message += "> fade "
		"<input type=\"radio\" name=\"tr\" value=\"1\"";
	if (settings.getTransition() == 1) message += " checked";
	message += "> move "
		"<input type=\"radio\" name=\"tr\" value=\"0\"";
	if (settings.getTransition() == 0) message += " checked";
	message += "> none"
		"</td></tr>";
#endif
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Timeout:"
		"</td><td>"
		"<select name=\"to\">";
	for (int i = 0; i <= 60; i += 5)
	{
		message += "<option value=\"" + String(i) + "\"";
		if (i == settings.getTimeout()) message += " selected";
		message += ">";
		if (i < 10) message += "0";
		message += String(i) + "</option>";
	}
	message += "</select> sec."
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Night off:"
		"</td><td>"
		"<input type=\"time\" name=\"no\" value=\"";
	if (hour(settings.getNightOffTime()) < 10) message += "0";
	message += String(hour(settings.getNightOffTime())) + ":";
	if (minute(settings.getNightOffTime()) < 10) message += "0";
	message += String(minute(settings.getNightOffTime())) + "\">"
		" h"
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Day on:"
		"</td><td>"
		"<input type=\"time\" name=\"do\" value=\"";
	if (hour(settings.getDayOnTime()) < 10) message += "0";
	message += String(hour(settings.getDayOnTime())) + ":";
	if (minute(settings.getDayOnTime()) < 10) message += "0";
	message += String(minute(settings.getDayOnTime())) + "\">"
		" h"
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Show \"It is\":"
		"</td><td>"
		"<input type=\"radio\" name=\"ii\" value=\"1\"";
	if (settings.getItIs()) message += " checked";
	message += "> on "
	"<input type=\"radio\" name=\"ii\" value=\"0\"";
	if (!settings.getItIs()) message += " checked";
	message += "> off"
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "<tr><td>"
		"Set date/time:"
		"</td><td>"
		"<input type=\"datetime-local\" name=\"st\">"
		"</td></tr>";
	// ------------------------------------------------------------------------
	message += "</table>"
		"<br><button title=\"Save Settings.\"><i class=\"fa fa-check\"></i></button>"
		"</form></body></html>";
	esp8266WebServer.send(200, "text/html", message);
}

void handleCommitSettings()
{
#ifdef DEBUG
	Serial.println("Commit settings pressed.");
#endif
	// ------------------------------------------------------------------------
#ifdef BUZZER
	time_t alarmTimeFromWeb = 0;
	esp8266WebServer.arg("a1") == "0" ? settings.setAlarm1(false) : settings.setAlarm1(true);
	alarmTimeFromWeb = esp8266WebServer.arg("a1t").substring(0, 2).toInt() * 3600 + esp8266WebServer.arg("a1t").substring(3, 5).toInt() * 60;
	if (settings.getAlarm1Time() != alarmTimeFromWeb) settings.setAlarm1(true);
	settings.setAlarm1Time(alarmTimeFromWeb);
	settings.setAlarm1Weekdays(
		esp8266WebServer.arg("a1w1").toInt() +
		esp8266WebServer.arg("a1w2").toInt() +
		esp8266WebServer.arg("a1w3").toInt() +
		esp8266WebServer.arg("a1w4").toInt() +
		esp8266WebServer.arg("a1w5").toInt() +
		esp8266WebServer.arg("a1w6").toInt() +
		esp8266WebServer.arg("a1w7").toInt());
	// ------------------------------------------------------------------------
	esp8266WebServer.arg("a2") == "0" ? settings.setAlarm2(false) : settings.setAlarm2(true);
	alarmTimeFromWeb = esp8266WebServer.arg("a2t").substring(0, 2).toInt() * 3600 + esp8266WebServer.arg("a2t").substring(3, 5).toInt() * 60;
	if (settings.getAlarm2Time() != alarmTimeFromWeb) settings.setAlarm2(true);
	settings.setAlarm2Time(alarmTimeFromWeb);
	settings.setAlarm2Weekdays(
		esp8266WebServer.arg("a2w1").toInt() +
		esp8266WebServer.arg("a2w2").toInt() +
		esp8266WebServer.arg("a2w3").toInt() +
		esp8266WebServer.arg("a2w4").toInt() +
		esp8266WebServer.arg("a2w5").toInt() +
		esp8266WebServer.arg("a2w6").toInt() +
		esp8266WebServer.arg("a2w7").toInt());
	// ------------------------------------------------------------------------
	esp8266WebServer.arg("hb") == "0" ? settings.setHourBeep(false) : settings.setHourBeep(true);
	// ------------------------------------------------------------------------
	if (esp8266WebServer.arg("ti").toInt())
	{
		alarmTimer = esp8266WebServer.arg("ti").toInt();
		alarmTimerSecond = second();
		alarmTimerSet = true;
		setMode(MODE_TIMER);
	}
#endif
	// ------------------------------------------------------------------------
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22)
	esp8266WebServer.arg("mc") == "0" ? settings.setModeChange(false) : settings.setModeChange(true);
#endif
	// ------------------------------------------------------------------------
#if defined(LDR)
	if (esp8266WebServer.arg("ab") == "0")
	{
		settings.setUseAbc(false);
		ratedBrightness = abcBrightness;
	}
	else settings.setUseAbc(true);
#endif
	// ------------------------------------------------------------------------
	settings.setBrightness(esp8266WebServer.arg("br").toInt());
	abcBrightness = map(settings.getBrightness(), 10, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
	ratedBrightness = abcBrightness;
	// ------------------------------------------------------------------------
	settings.setColor(esp8266WebServer.arg("co").toInt());
	// ------------------------------------------------------------------------
	switch (esp8266WebServer.arg("cc").toInt())
	{
	case 0: settings.setColorChange(COLORCHANGE_NO); break;
	case 1: settings.setColorChange(COLORCHANGE_FIVE); break;
	case 2: settings.setColorChange(COLORCHANGE_HOUR); break;
	case 3: settings.setColorChange(COLORCHANGE_DAY); break;
	}
	// ------------------------------------------------------------------------
	switch (esp8266WebServer.arg("tr").toInt())
	{
	case 0: settings.setTransition(TRANSITION_NORMAL); break;
	case 1: settings.setTransition(TRANSITION_MOVEUP); break;
	case 2: settings.setTransition(TRANSITION_FADE); break;
	}
	// ------------------------------------------------------------------------
	settings.setTimeout(esp8266WebServer.arg("to").toInt());
	// ------------------------------------------------------------------------
	settings.setNightOffTime(esp8266WebServer.arg("no").substring(0, 2).toInt() * 3600 + esp8266WebServer.arg("no").substring(3, 5).toInt() * 60);
	// ------------------------------------------------------------------------
	settings.setDayOnTime(esp8266WebServer.arg("do").substring(0, 2).toInt() * 3600 + esp8266WebServer.arg("do").substring(3, 5).toInt() * 60);
	// ------------------------------------------------------------------------
	esp8266WebServer.arg("ii") == "0" ? settings.setItIs(false) : settings.setItIs(true);
	// ------------------------------------------------------------------------
	if (esp8266WebServer.arg("st").length())
	{
		Serial.println(esp8266WebServer.arg("st"));
		setTime(esp8266WebServer.arg("st").substring(11, 13).toInt(), esp8266WebServer.arg("st").substring(14, 16).toInt(), 0, esp8266WebServer.arg("st").substring(8, 10).toInt(), esp8266WebServer.arg("st").substring(5, 7).toInt(), esp8266WebServer.arg("st").substring(0, 4).toInt());
#ifdef RTC_BACKUP
		RTC.set(timeZone.toUTC(now()));
#endif
	}
	// ------------------------------------------------------------------------
	settings.saveToEEPROM();
	callRoot();
	screenBufferNeedsUpdate = true;
}

// Page reset
void handleReset()
{
	esp8266WebServer.send(200, "text/plain", "OK. I'll be back!");
	ESP.restart();
}

// Page setEvent
void handleSetEvent()
{
	events[0].day = esp8266WebServer.arg("day").toInt();
	events[0].month = esp8266WebServer.arg("month").toInt();
	events[0].text = esp8266WebServer.arg("text").substring(0, 40);
	events[0].color = (eColor)esp8266WebServer.arg("color").toInt();;
	esp8266WebServer.send(200, "text/plain", "OK.");

#ifdef DEBUG
	Serial.println("Event set: " + String(events[0].day) + "." + String(events[0].month) + ". " + events[0].text);
#endif
}

// Page showText
void handleShowText()
{
	uint8_t feedBuzzer = esp8266WebServer.arg("buzzer").toInt();
	feedColor = esp8266WebServer.arg("color").toInt();
	feedText = "  " + esp8266WebServer.arg("text").substring(0, 80) + "   ";
	feedPosition = 0;
	esp8266WebServer.send(200, "text/plain", "OK.");

#ifdef DEBUG
	Serial.println("Show text: " + esp8266WebServer.arg("text").substring(0, 80));
#endif

#ifdef BUZZER
	for (uint8_t i = 0; i < feedBuzzer; i++)
	{
		digitalWrite(PIN_BUZZER, HIGH);
		delay(75);
		digitalWrite(PIN_BUZZER, LOW);
		delay(100);
	}
#endif

	setMode(MODE_FEED);
}
