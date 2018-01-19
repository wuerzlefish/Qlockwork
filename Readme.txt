******************************************************************************
QLOCKWORK2
An advanced firmware for a DIY "word-clock".
******************************************************************************

Qlockwork2 is an ESP8266 (NodeMCU or WeMos D1 mini) firmware (under GPL license) for a so called "word-clock".

The clock adjusts the time and date once per hour via NTP with a time server on the Internet.
If an RTC is installed, the time of the ESP is also set from the RTC every minute.

At first startup and if there is a new version of the settings the EEPROM of the clock is written with default values.
From there on the settings are read from EEPROM at startup.
At startup the clock performs a short self test.
The sequence of the colors should be: red, green, blue and white.
The clock also shows the local IP address it received via DHCP.
Use this address in a browser to access the clocks web site to control it.

WiFi manager: If the clock can not connect to any WLAN at startup, it turns on an access point.
Connect a mobile phone or tablet to the AP and enter the WLAN credentials. A white "WiFi" is shown on the clock.
On success there are three short beeps and "WiFi" will be green.
If no WLAN is connected or the timeout has expired, there is a long beep and "WiFi" turns red.
After the WLAN timeout the clock works without NTP but you can still control it via its AP.
Without WLAN the clock uses the optional RTC to set the time or if no RTC is present has to be set manually.
In either ways the clocks IP is shown as a textfeed.

Events can be shown every five minutes on a particular day of the year as a textfeed.
You can set them in "Events.h". Expand the array with events as shown in the default values.
Set variables for recurrent texts to save RAM. You can set a color for every event.
There is no comma behind the last entry.

Updates of the firmware could be uploaded via USB, OTA or the clocks webserver.
You will find more help and information on how to configure and compile the firmware in "Readme.md" in the zip-archive.
You will also find a circuit diagram, a partslist and some pictures in the "/misc" directory.
All sensors, the RTC and the buzzer are optional.
The clock will run with the ESP8266 module only. No PCB needed.

Warning: Do not power up the clock from USB only.
         This would blow up the ESP or even the USB port because of high power demand of the LED stripe.
         Always use an external 5V powersupply with at least 4A.

Disclaimer: Qlockwork2 uses lots of third party libraries.
            I can not guarantee the integrity of these libraries.
			You use Qlockwork2 at your own risk.

You can download the latest version of the firmware here:
http://tmw-it.ch/qlockwork/

Visit this forum for comments, suggestions and bug reports:
http://diskussion.christians-bastel-laden.de/viewtopic.php?f=23&t=2843

******************************************************************************
Top features.
******************************************************************************

Almost no electronics needed.
Only ESP8266 (NodeMCU or WeMos D1 mini) and an LED-stripe.
Optional support for WiFi, RTC, LDR, Buzzer, temperature and humidity sensor and IR-remote.
Supports more than 30 types of LED stripes.
FastLED (RGB), LPD8806 (RGBW), NeoPixel (RGB and RGBW).
Horizontal and vertical LED layout.
Webpage to control the clock.
Adaptive brightness control.
2 Transitions for timechange.
Room and outdoor temperature.
Room and outdoor humidity.
Weatherconditions.
Moonphase.
Textfeed for events and infos.
Supports 16 frontcovers and 6 languages.
25 Colors.
Timer.
2 Alarmtimes.
NTP timesync.
RTC timesync as backup.
Timezones.
Automatic adjustment of daylight saving time.
USB, Over-the-air or webserver firmware updates.
WiFi manager for initial setup via accesspoint.

******************************************************************************
Modes.
******************************************************************************

Time.
AM/PM.
Seconds.
Weekday.
Date.
Moonphase.
Room temperature (RTC or DHT22).
Room humidity (DHT22).
Outdoor temperature (Yahoo).
Outdoor humidity (Yahoo).
Weathercondition (Yahoo).
Timer.
LED-test.
Red.
Green.
Blue.
White.

******************************************************************************
Needed libraries.
******************************************************************************

via Arduino:
ArduinoOTA by Ivan Grokhotkov
ArduinoHttpClient by Arduino
ESP8266mDNS by Tony DiCola and Ivan Grokhotkov
ESP8266WebServer by Ivan Grokhotkov
ESP8266WiFi by Ivan Grokhotkov

via Web:
https://github.com/adafruit/Adafruit_NeoPixel
https://github.com/adafruit/Adafruit_Sensor
https://github.com/bblanchon/ArduinoJson
https://github.com/adafruit/DHT-sensor-library
https://github.com/JChristensen/DS3232RTC
https://github.com/FastLED/FastLED
https://github.com/markszabo/IRremoteESP8266
https://github.com/arcao/Syslog
https://github.com/PaulStoffregen/Time
https://github.com/JChristensen/Timezone
https://github.com/tzapu/WiFiManager

There is a warning from FastLED about SPI when compiling. Just ignore it.

For OTA and web-server updates check out:
http://esp8266.github.io/Arduino/versions/2.3.0/doc/ota_updates/readme.html
Don't forget to install Python 2.7 and to select "Add python.exe to path".

Call "http://your_clocks_ip/update" to upload a new firmware via webbrowser.
Call "http://your_clocks_ip/reset" to restart the ESP.

******************************************************************************
Operation manual.
******************************************************************************

Press "on/off" to switch the LEDs on and off.
Press "Settings" to configure the clock via web-site.
Press "Mode" to jump to the next mode.
Press "Time" to always jump back to the time.

*** Modes: *******************************************************************

Time:                              The default mode of the clock. Shows the actual time. :)
Display AM/PM:                     Indicates whether it is AM or PM.
Seconds:                           Shows the seconds.
Weekday:                           Shows the weekday in local language.
Date:                              Shows day and month.
Moonphase:                         Shows the moonphase.
Room temperature:                  Display of the measured temperature in the room (only with RTC or DHT22).
Room humidity:                     Display of the measured humidity in the room (only with DHT22).
Outdoor temperature:               Displays the temperature for a location from Yahoo Weather.
Outdoor humidity:                  Humidity display for the selected location from Yahoo Weather.
Outdoor condition:                 The weathercondition in a feed in local language from Yahoo Weather.
Timer:                             Display of the remaining time if a timer is set.
LED-Test:                          Moves a horizontal bar across the display.
Red:                               Set all LEDs to red.
Green:                             Set all LEDs to green.
Blue:                              Set all LEDs to blue.
White:                             Set all LEDs to white.

*** Settings: ****************************************************************

Alarm 1:                           Enable (on) or disable (off) alarm 1.
                                   Time for alarm 1.
                                   Weekdays on which alarm 1 is active.
Alarm 2:                           Enable (on) or disable (off) alarm 2.
                                   Time for alarm 2.
                                   Weekdays on which alarm 2 is active.
Timer:                             Sets the minute timer. (0: disabled)
Madechange:                        Enable (on) or disable (off) automatic modechange in time view.
ABC:                               Enable (on) or disable (off) adaptive brightness control.
                                   Brightness will adjust itself in the range of MIN_BRIGHTNESS and brightness.
Brightness:                        Brightness of the LEDs in percent. The range is MIN_BRIGHTNESS to MAX_BRIGHTNESS.
                                   If ABC is enabled this is the maximum achievable brightness.
Color:                             Choose one of 25 colors for the LEDs.
Colorchange:                       Change the color in intervals.
                                   Do not change (off), every 5 minutes (five), every hour (hour), every day (day).
Transition:                        Choose between normal or faded LED timechanges.
Timeout:                           Time in seconds to change mode back to time. (0: disabled)
Night off:                         Set the time the clocks turns itself off at night.
Day on:                            Set the time the clocks turns itself on at day.
Show "It is":                      Enable (on) or disable (off) "It is". It will be shown every half and full hour anyway.
Set date/time:                     Date and time of the clock. The seconds are set to zero if you press save.

******************************************************************************
Configuration.h - Software settings.
******************************************************************************

#define HOSTNAME                   The name of the clock.
#define WIFI_AP_TIMEOUT            Time in seconds for the WiFiManager to setup/search for a WLAN.
                                   If no WLAN is connected the clock enters AP mode.
                                   You can control the clock if you connect your phone or tablet to this accesspoint.
								   On Android you have to tell the phone that it's ok to have no internet.
#define WIFI_AP_PASS               The password for the AP. At least 8 characters. Default is "12345678".
#define OTA_PASS                   Password for "Over the Air" updates. Default is "1234".
#define NTP_SERVER                 NTP server to be queried.
#define NTP_TIMEOUT                Milliseconds to wait for NTP server to answer.
#define SHOW_IP                    Show local IP at startup. Use this in the browser to access the clocks menue.
#define NONE_TECHNICAL_ZERO        Displays the zero without the diagonal line.
#define AUTO_MODECHANGE_TIME       Time in seconds to wait between switching from time to temperature.
#define FEED_SPEED                 Feed delay in milliseconds. 120 is a good start.
#define EVENT_TIME                 Time in seconds to wait between showing events. Comment to turn off events.
#define ALARM_LED_COLOR            Color of the alarm LED. If not defined the display color will be used.
                                   The possible colors are:
                                   WHITE, RED, RED_25, RED_50, ORANGE, YELLOW, YELLOW_25, YELLOW_50, GREENYELLOW,
                                   GREEN, GREEN_25, GREEN_50, MINTGREEN, CYAN, CYAN_25, CYAN_50, LIGHTBLUE, BLUE,
                                   BLUE_25, BLUE_50, VIOLET, MAGENTA, MAGENTA_25, MAGENTA_50, PINK.
#define ABUSE_CORNER_LED_FOR_ALARM Use the upper right minute LED as alarm LED. Only works if ALARM_LED_COLOR is defined.
                                   If no alarm or timer is set the LED is used as expected.
#define DEDICATION                 Show a text on the clocks webpage.
#define SELFTEST                   Test LEDs at startup. Colors are: white, red, green, blue. In this order.
#define BRIGHTNESS_SELFTEST        Brightness of the LEDs while in testmode to not overload the powersupply.
#define LOCATION                   Location for the weather conditions as shown on the Yahoo site if you enter your city.
                                   "https://www.yahoo.com/news/weather" Click on "Change location". (Not Search.)
                                   (Only letters, ' ', and ',' are allowed).
#define FRONTCOVER_*               Frontcover of the clock. Also sets the language of the menu and the web-site.
#define SHOW_MODE_AMPM
#define SHOW_MODE_SECONDS
#define SHOW_MODE_WEEKDAY
#define SHOW_MODE_DATE
#define SHOW_MODE_MOONPHASE
#define SHOW_MODE_TEST
#define TIMEZONE_*                 The time zone in which the clock is located. Important for the UTC offset and the
                                   summer/winter time change.

******************************************************************************
Configuration.h - Hardware settings.
******************************************************************************

#define ESP_LED                    Displays the function using the LED on the ESP. It flashes once a second.

#define SENSOR_DHT22               Use a DHT22 sensor module (not the plain sensor) for room temperature and humidity.
#define DHT_TEMPERATURE_OFFSET     Sets how many degrees the measured room temperature (+ or -) should be corrected.
#define DHT_HUMIDITY_OFFSET        Sets how many degrees the measured room humidity (+ or -) should be corrected.

#define RTC_BACKUP                 Use an RTC as backup and room temperature.
#define RTC_TEMPERATURE_OFFSET     Sets how many degrees the measured room temperature (+ or -) should be corrected.

#define LDR                        Use an LDR for adaptive brightness control (ABC).
#define LDR_IS_INVERSE             Combined with #define LDR inverses the value of the LDR.
#define LDR_HYSTERESIS             Brightness control from a deviation in the range from 0 to 1023. Default: 40.
                                   If your display is flickering increase this value.
#define MIN_BRIGHTNESS             Minimum brightness of LEDs ranging from 0 to 255. Default: 20.
#define MAX_BRIGHTNESS             Maximum brightness of LEDs ranging from 0 to 255. Default 255.
                                   Your powersupply has to support this brightness.

#define BUZZER                     Use a buzzer to make noise for alarmtime and timer.
                                   If not defined all alarmfunctions are disabled.
#define BUZZTIME_ALARM_1           Maximum time in seconds for alarm 1 to be active when not turned off manually.
#define BUZZTIME_ALARM_2           Maximum time in seconds for alarm 2 to be active when not turned off manually.
#define BUZZTIME_TIMER             Maximum time in seconds for the timer alarm to be active when not turned off manually.

#define IR_REMOTE                  Use an IR remote control.
#define IR_LETTER_OFF              Turns off the LED behind the IR sensor permanently. This improves IR reception.
#define IR_CODE_*                  Any remote control can be used. 6 keys are supported.
                                   Press a button on the remote control in front of the clock.
                                   Then write the code displayed in the serial console to the file "Configuration.h".
								   If you see more than one try the code which is changing from button to button.
								   DEBUG has to be defined to show you the code.

#define LED_LIBRARY_LPD8806RGBW    LED library for LPD8806 RGBW LEDs. There is no driver to define.

#define LED_LIBRARY_NEOPIXEL       LED library for NeoPixel LEDs.
#define LED_DRIVER_NEO_*           Specifies the NeoPixel driver in conjunction with LED_LIBRARAY_NEOPIXEL.
                                   400kHz, 800kHz, GRB, RGB, GRBW and RGBW.

#define LED_LIBRARY_FASTLED        FastLED library for LEDs.
#define LED_DRIVER_FAST_*          Specifies the driver in conjunction with LED_LIBRARAY_FASTLED.
                                   All LED drivers supported by FAST-LED (RGB only) can be used:
                                   APA102, APA104, APA106, DOTSTAR, DMXSIMPLE, GW6205, GW6205_400, LPD1886,
                                   LPD1886_8BIT, LPD8806, NEOPIXEL, P9813, PL9823, SK6812, SK6822, SK9822,
                                   SM16716, TM1803, TM1804, TM1809, TM1812, TM1829, UCS1903, UCS1903B,
                                   UCS1904, UCS2903, WS2801, WS2803, WS2811, WS2811_400, WS2812, WS2812B,
                                   WS2813, WS2852.

#define LED_LAYOUT_HORIZONTAL      Horizontal and corner LEDs at the end of the strip. (As seen from the front.)

111                    114                    112
   000 001 002 003 004 005 006 007 008 009 010
   021 020 019 018 017 016 015 014 013 012 011
   022 023 024 025 026 027 028 029 030 031 032
   043 042 041 040 039 038 037 036 035 034 033
   044 045 046 047 048 049 050 051 052 053 054
   065 064 063 062 061 060 059 058 057 056 055
   066 067 068 069 070 071 072 073 074 075 076
   087 086 085 084 083 082 081 080 079 078 077
   088 089 090 091 092 093 094 095 096 097 098
   109 108 107 106 105 104 103 102 101 100 099
110                                            113

#define LED_LAYOUT_VERTICAL        Vertical and corner LEDs within the strip. (As seen from the front.)

000                    114                    102
   001 021 022 041 042 061 062 081 082 101 103
   002 020 023 040 043 060 063 080 083 100 104
   003 019 024 039 044 059 064 079 084 099 105
   004 018 025 038 045 058 065 078 085 098 106
   005 017 026 037 046 057 066 077 086 097 107
   006 016 027 036 047 056 067 076 087 096 108
   007 015 028 035 048 055 068 075 088 095 109
   008 014 029 034 049 054 069 074 089 094 110
   009 013 030 033 050 053 070 073 090 093 111
   010 012 031 032 051 052 071 072 091 092 112
011                                           113

******************************************************************************
Configuration.h - Default values for the EEPROM.
Only used when a new version of the settings is released.
******************************************************************************

#define DEFAULT_BRIGHTNESS         0 to 100 in percent.
#define DEFAULT_COLOR              WHITE, RED, RED_25, RED_50, ORANGE, YELLOW, YELLOW_25, YELLOW_50, GREENYELLOW,
                                   GREEN, GREEN_25, GREEN_50, MINTGREEN, CYAN, CYAN_25, CYAN_50, LIGHTBLUE, BLUE,
                                   BLUE_25, BLUE_50, VIOLET, MAGENTA, MAGENTA_25, MAGENTA_50, PINK.
#define DEFAULT_COLORCHANGE        COLORCHANGE_DAY, COLORCHANGE_HOUR, COLORCHANGE_FIVE, COLORCHANGE_NO.
#define DEFAULT_SHOWTEMP           true (on) or false (off).
#define DEFAULT_SHOWITIS           true (on) or false (off).
#define DEFAULT_TRANSITION         TRANSITION_FADE, TRANSITION_NORMAL.
#define DEFAULT_TIMEOUT            Seconds for fallback.
#define DEFAULT_USELDR             true (on) or false (off).
#define DEFAULT_ALARM1             true (on) or false (off).
#define DEFAULT_ALARMTIME1         Seconds since 0:00h (12:00h am). E.g. 54000 for 3:00h pm.
#define DEFAULT_ALARM1WEEKDAYS     0b11111110 // Sa. Fr. Th. We. Tu. Mo. Su. 0
#define DEFAULT_ALARM2             true (on) or false (off).
#define DEFAULT_ALARMTIME2         Seconds since 0:00h (12:00h am). E.g. 72000 for 8:00h pm.
#define DEFAULT_ALARM2WEEKDAYS     0b11111110 // Sa. Fr. Th. We. Tu. Mo. Su. 0
#define DEFAULT_NIGHTOFF           Seconds since 0:00h (12:00h am). E.g. 3600 for 1:00h am.
#define DEFAULT_DAYON              Seconds since 0:00h (12:00h am). E.g. 18000 for 5:00h am.

******************************************************************************
Configuration.h - Misc.
******************************************************************************

#define DEBUG                      Show debug infos in the serial console.
#define DEBUG_WEB                  Show debug infos on the web page.
#define DEBUG_MATRIX               Renders the output of the matrix for the German front in the serial console.
#define DEBUG_FPS                  Show number of loops per second in the serial console.

#define SYSLOGSERVER               Turn logging to a syslogserver on/off.
#define SYSLOGSERVER_SERVER        Address of the syslogserver.
#define SYSLOGSERVER_PORT          Port of the syslogserver.

#define UPDATE_INFO_*              The update info periodically anonymously checks if there is a firmwareupdate
                                   available. No user data is send to the host. Comment if you do not want this info.
#define UPDATE_INFOSERVER          Address of the updateinfo server.
#define UPDATE_INFOFILE            Path and name of the updateinfo file.

#define SERIAL_SPEED               Serial port speed for the console.

#define PIN_IR_RECEIVER D3         Pin for the IR receiver.
#define PIN_LED         D4         Pin for the LED (build into the ESP).
#define PIN_BUZZER      D5         Pin for the buzzer.
#define PIN_DHT22       D6         Pin for the DHT22 module.
#define PIN_LEDS_CLOCK  D7         Pin for the LED stripe "clock" if needed.
#define PIN_LEDS_DATA   D8         Pin for the LED stripe "data".
#define PIN_LDR         A0         Pin for the LDR.

### Events.h ###

const event_t events[]             Display a textfeed on a particular day of the year.
                                   The format of an entry in the array is:
                                   { month, day, "Text to display.", year, color },
                                   The last entry has no comma at the end.
                                   Year will be used to calculate an age. "present year" - year = age.
								   '0' will not show an age. Only one event a day.
                                   The possible colors are:
                                   WHITE, RED, RED_25, RED_50, ORANGE, YELLOW, YELLOW_25, YELLOW_50, GREENYELLOW,
                                   GREEN, GREEN_25, GREEN_50, MINTGREEN, CYAN, CYAN_25, CYAN_50, LIGHTBLUE, BLUE,
                                   BLUE_25, BLUE_50, VIOLET, MAGENTA, MAGENTA_25, MAGENTA_50, PINK.

******************************************************************************
Changelog:
******************************************************************************

20171127:
Bugfixes.

20171125:
Set weekdays for alarms on web-page.
Set alarm to "on" if alarmtime is changed.
Set time, timeout, "Night off", "Day on" on web-page.
Removed settings from clock-menu which are present on web-page.
AP mode if no WLAN is connected.
"#define DEBUG" and "#define DEBUG_WEB" are back.
Switched from RestClient.h to ArduinoHttpClient.h.
Moved setting of frontcover to configuration.h "#define FRONTCOVER_*".
Removed "#define LANGUAGE_*". Now set from "#define FRONTCOVER_*".
Removed experimantal DUAL-support.
Settings set to defaults.

20171111 (stable):
Cleanup and bugfixes.
New syslog format.

20171019:
Changing automode.
Bugfix for Firefox.

20171013:
Settings of alarms, colorchange, automode and show "It is" on Web-Page.
RTC is now on UTC.

20171006:
Moonphase.
Color humidity indicator.
Faster Web-GUI.
Better offline behavior.
Reset via URL.

20170929:
Adaptive Brightness Control (ABC).
New syslog logging.
Code cleanup and bugfixes.
