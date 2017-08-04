# QLOCKWORK
### A firmware for the DIY-QLOCKTWO.

### Top Features:
```
Almost no electronics needed.
Only ESP8266 (NodeMCU or WeMos D1 mini) and an LED-stripe.
Optional support for WiFi, RTC, LDR, Buzzer, temperature and humidity sensor and IR-remote.
Supports more than 30 types of LED stripes.
FastLED (RGB), LPD8806 (RGBW), NeoPixel (RGB and RGBW).
Horizontal and vertial LED layout.
Webserver to control the clock.
2 Transitions for timechange.
Room and external Temperature.
Room and external Humidity.
16 Front languages.
25 Colors.
Timer.
2 Alarmtimes.
NTP timesync.
RTC timesync as backup.
Timezones.
Daylight saving time.
Over-the-air updates.
WiFi manager for initial setup via accesspoint.
```
### Standard modes
```
Time.
AM/PM.
Seconds.
Weekday.
Date.
Room temperature (with RTC or DHT22 only).
Room humidity (with DHT22 only).
Temperature (ext.) (Yahoo weather).
Humidity (ext.) (Yahoo weather).
Timer.
Alarm 1.
Alarm 2.
```
### Extended modes
```
Brightness.
Colorchange.
Color.
Transition.
Fallbacktime.
Front language.
Set time.
"It is" on/off.
Set date.
Night off time.
Day on time.
Testmode.
```
### Needed libraries:
```
via Arduino:
ArduinoOTA by Ivan Grokhotkov
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
https://github.com/markszabo/IRremoteESP8266 (>= version 2.0.0)
https://github.com/ch570512/LPD8806RGBW
https://github.com/arduino-libraries/RestClient
https://github.com/arcao/Syslog
https://github.com/PaulStoffregen/Time
https://github.com/JChristensen/Timezone
https://github.com/tzapu/WiFiManager

There is a warning from FastLED about SPI when compiling. Just ignore it.
```

# QLOCKWORK
### A firmware for the DIY-QLOCKTWO.

Qlockwork is a ESP8266 (NodeMCU or WeMos D1 mini) firmware for a so called "word-clock" like the Qlocktwo.

Sie gleicht die Zeit einmal pro Stunde per NTP mit einem Zeitserver im Internet ab.
Wenn eine RTC installiert ist, wird die Zeit jede Minute mit dieser verglichen und bei einer Abweichung angepasst.
Auf der Web-Seite kann man die Uhr steuern.
Updates sind OTA moeglich. Dazu im Arduino IDE den ESP als Port auswaehlen. It has to be on the same local network.
Nach dem Einschalten macht die Uhr einen kurzen Selbsttest. Die Reihenfolge der Farben ist: weiss, rot, gruen und blau.
Wenn die Reihenfolge der Farben anders ist, ist der LED-Treiber falsch eingestellt. Sollte weiss nicht sauber sein, reicht evtl. der Strom nicht aus.
In diesem Fall kann eine zusaetzliche Stromversorgung in der Mitte des Streifens helfen.

WLAN Manager: Wenn die Uhr sich beim Start mit keinem WLAN verbinden kann schaltet sie einen AccessPoint ein. Dann ein Handy oder Tablet mit diesem verbinden und die WLAN Daten eingeben. WiFi leuchtet in dieser Zeit weiss auf dem Display. Wenn kein WLAN verbunden wird oder der Timeout abgelaufen ist, gibt es einen langern Ton und WiFi wird rot. Bei Erfolg gibt es drei kurze Toene und WiFi wird gruen. WLAN wird nicht zwingend benoetigt. Nach dem WLAN-Timeout funktioniert die Uhr auch ohne NTP. Dazu benutzt sie die optionale RTC oder muss von Hand gestellt werden.

At first startup and if there is a new version of the settings the EEPROM of the clock is written with default values. From there on the settings are read from EEPROM at startup.
```
Defaults are:
language = LANGUAGE_EN
useLdr = false
brightness = MAX_BRIGHTNESS
color = WHITE
colorChange = COLORCHANGE_NO
transition = TRANSITION_FADE
timeout = 5
itIs = true
alarm1 = false
alarmTime1 = 0
alarm2 = false
alarmTime2 = 0
nightOffTime = 0
dayOnTime = 0
```
You will find a circuit diagram and a partslist in the /misc/ directory. All sensors, the RTC and the buzzer are optional. The clock will run with the ESP8266 module itself only.

Disclaimer: Qlockwork uses lots of third party libraries. I can not guarantee the integrity of these libraries. So use Qlockwork at your own risk.

You can download the latest version of the firmware here: http://tmw-it.ch/qlockwork/

### Standard Modi
```
Time:             The default mode of the clock. Shows the actual time. :)
                  + or - to move directly to the next or previous category.
Display AM/PM:    Zeigt an, ob es vormittags (AM) oder nachmittags (PM) ist.
Seconds:          Shows the seconds.
Weekday:          Shows the weekday in the selected language.
Date:             Shows day and month.
Title (TE MP):    + or - to move directly to the next or previous category.
Temperature:      Anzeige der gemessenen Temperatur im Raum (nur mit RTC oder DHT22).
Humidity:         Anzeige der gemessenen Luftfeuchtigkeit im Raum (nur mit DHT22).
Temperature:      Anzeige der Temperatur fuer einen Ort (Yahoo Weather).
Humidity:         Anzeige der Luftfeuchtigkeit fuer den gewaehlen Ort (Yahoo Weather).
Title (AL RM):    + or - to move directly to the next or previous category.
Timer (TI):       Setzt den Minuten-Timer. Null schaltet den Timer ab.
                  Anzeige der Restzeit wenn ein Timer gesetzt ist.
                  + or - to set the timer and start it.
Alarm1 (A1):      Enable (EN) or disable (DA) alarm 1.
Alarm1:           Set the first 24 hour alarm if A1 is EN.
Alarm2 (A2):      Enable (EN) or disable (DA) alarm 2.
Alarm2:           Set the second 24 hour alarm if A2 is EN.
```
### Erweiterte Modi
```
Title (MA IN):                   + or - to move directly to the next or previous category.
Autom. Helligkeitsregelung (LD): Enabled (EN) or disabled (DA).
Helligkeitsregelung:             Press + or - to change the brightness.
Color Change (CC):               Change the color in intervalls.
                                 Do not change (NO), every 5 minutes (FI), every hour (HR), every day (DY).
Color (CO):                      0: white, 1: red, 2: green, 3: blue and so on.
Transition (TR):                 Normal (NO) / Fade (FD).
Fallback (FB nn):                Time in seconds to change back to time. (0: disabled, default: 5)
Front language (DE/CH/EN/...):   Select the correct language for your frontpanel.
Title (TI ME):                   + or - to move directly to the next or previous category.
Set time:                        + or - to set the time manually. + sets the hour, - the minute.
                                 The seconds jump to zero every time you hit + or -.
"It is" (IT):                    Enable (EN) or disable (DA) "It is". It will be shown at every half and full hour anyway.
Set day (DD nn):                 + or - to set the day.
Set month (MM nn):               + or - to set the month.
Set year (YY nn):                + or - to set the year.
Title (NI OF):                   + or - or mode to set the time.
Night off time:                  + or - to set the time the clocks turns itself off at night. + sets the hour, - the minute.
Title (DY ON):                   + or - or mode to set the time.
Day on time:                     + or - to set the time the clocks turns itself on at day. + sets the hour, - the minute.
Title (TE ST):                   + or - to move directly to the next or previous category.
LED-Test:                        Moves a horizontal bar across the display.
```
### Configuration.h
```
#define CONFIG_*            Simple support of different configurations in one file.
#define HOSTNAME            The name of the watch.
#define WIFI_AP_TIMEOUT     Time in seconds for the WiFiManager to set-up/search the WLAN.
#define OTA_PASS            Password for "Over the Air" updates.
#define NTP_SERVER          NTP server to be queried.
#define RTC_BACKUP          Use an RTC as backup.
#define RTC_TEMP_OFFSET     Sets how many degrees the measured room temperature (+ or -) should be corrected.
#define SENSOR_DHT22        Use a DHT22 sensor module (not the plain sensor) for room temperature and humidity.
#define SELFTEST            Test LEDs at startup. Colors are: white, red, green, blue. In this order.
#define SHOW_IP             Show local IP at startup. Use this in the browser to access the clocks menue.
#define ESP_LED             Displays the function using the LED on the ESP. It flashes once a second.
#define LDR                 Use an LDR for brightness control.
#define LDR_INVERSE         Inverse the value of the LDR.
#define LDR_HYSTERESIS      Brightness control from a deviation in the range from 0 to 1023. Default: 30.
#define MIN_BRIGHTNESS      Minimum brightness of LEDs ranging from 0 to 255. Default: 10.
#define MAX_BRIGHTNESS      Maximum brightness of LEDs ranging from 0 to 255. Default 255.
#define NONE_TECHNICAL_ZERO Displays the zero without the diagonal line.
#define BUZZER              Use a buzzer to make noise for alarmtime and timer.
#define BUZZTIME_ALARM_1    Maximum time in seconds that turns alarm 1 on when not turned off manually.
#define BUZZTIME_ALARM_2    Maximum time in seconds that turns alarm 2 on when not turned off manually.
#define BUZZTIME_TIMER      Maximum time in seconds that turns the timer alarm on when not turned off manually..
#define LOCATION            Location for the weather conditions as entered on the Yahoo site.
                            (Only letters, ' ', and ',' are allowed).
#define LANGUAGE_*          Language of the clock. The front itself is set in the clocks menue.
#define UPDATE_INFO_*       The update info periodically anonymously checks if there is a firmwareupdate
                            available. No user data is send to the host.
#define TIMEZONE_*          The time zone in which the clock is located. Important for the UTC offset and the
                            summer / winter time changeover.
#define IR_REMOTE           Use an IR remote control.
#define IR_LETTER_OFF       Schaltet die LED hinter dem IR-Sensor dauerhaft ab. Das verbessert den IR-Empfang.
                            Hier das K vor Uhr.
#define IR_CODE_*           Jede Fernbedienung kann verwendet werden. Es werden 6 Tasten unterstuetzt.
                            Um die Fernbedienung anzulernen "#define DEBUG" einschalten und einen Knopf auf der
                            Fernbedienung druecken. Den in der seriellen Konsole angezeigten Code fuer die Taste
                            dann in die Datei "Configuration.h" schreiben.

#define LED_LAYOUT_HORIZONTAL Horizontal and corner LEDs at the end of the strip. (Seen from the front.)

111                 114                 112
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
110                                     113

#define LED_LAYOUT_VERTICAL   Vertical and corner LEDs within the strip. (Seen from the front.)

000                 114                 102
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
011                                     113

#define LED_LIBRARY_LPD8806RGBW LED Driver for LPD8806 RGBW LEDs.

#define LED_LIBRARY_NEOPIXEL    LED Driver for NeoPixel LEDs.
#define LED_DRIVER_NEO_*        Gibt in Verbindung mit LED_LIBRARAY_NEOPIXEL den Typ der NeoPixel an.
                                400kHz, 800kHz, GRB, RGB, GRBW und RGBW.

#define LED_LIBRARY_FASTLED     FastLED Driver for LEDs.
#define LED_DRIVER_FAST_*       Gibt in Verbindung mit LED_LIBRARAY_FASTLED den Typ der LEDs an.
                                Alle von FAST-LED unterstuetzten LED-Treiber (nur RGB) koennen verwendet
                                werden:
                                APA102, APA104, APA106, DOTSTAR, DMXSIMPLE, GW6205, GW6205_400, LPD1886,
                                LPD1886_8BIT, LPD8806, NEOPIXEL, P9813, PL9823, SK6812, SK6822, SK9822,
                                SM16716, TM1803, TM1804, TM1809, TM1812, TM1829, UCS1903, UCS1903B,
                                UCS1904, UCS2903, WS2801, WS2803, WS2811, WS2811_400, WS2812, WS2812B,
                                WS2813, WS2852.

#define SERIAL_SPEED  Serial port speed for the console.
#define DEBUG         Outputs technical information in the serial console.
#define DEBUG_WEBSITE Provides technical information on the web page.
#define DEBUG_MATRIX  Renders the output of the matrix for the German front in the serial console.
#define DEBUG_FPS     Number of loops per second.

Hardware setup of the ESP:

D0 = GPIO16 = NodeMCU_LED
D1 = GPIO05 = PIN_WIRE_SCL
D2 = GPIO04 = PIN_WIRE_SDA
D3 = GPIO00 = PIN_IR_RECEIVER
D4 = GPIO02 = ESP8266_LED
D5 = GPIO14 = PIN_BUZZER
D6 = GPIO12 = PIN_DHT22
D7 = GPIO13 = PIN_LEDS_CLOCK
D8 = GPIO15 = PIN_LEDS_DATA
A0 = ADC0   = PIN_LDR

```
