# QLOCKWORK
### A firmware for the DIY-QLOCKTWO.

### Top Features:
```
Almost no electronics needed.
Only ESP8266 (NodeMCU or WeMos D1 mini) and an LED-stripe.
Optional support for WiFi, RTC, LDR, Buzzer and IR-remote.
Supports more than 30 types of LED stripes.
FastLED (RGB), LPD8806 (RGBW), NeoPixel (RGB and RGBW).
Horizontal and vertial LED layout.
Webserver to control the clock.
2 Transitions for timechange.
Room and external Temperature.
16 Languages.
37 Colors.
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
Room Temperature (with RTC only).
Temperature (ext.) (Yahoo weather).
Humidity (ext.) (Yahoo weather).
Timer.
Alarm 1.
Alarm 2.
```
### Extended modes
```
Brightnesscontrol.
Colorchange.
Color.
Transition.
Fallbacktime.
Language.
Set Time.
"It is" on/off.
Set Date.
Nightmode on/off.
Show local IP.
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
https://github.com/JChristensen/DS3232RTC
https://github.com/ch570512/LPD8806RGBW
https://github.com/PaulStoffregen/Time
https://github.com/JChristensen/Timezone
https://github.com/markszabo/IRremoteESP8266 (Version 2.0.0)
https://github.com/arduino-libraries/RestClient
https://github.com/bblanchon/ArduinoJson
https://github.com/tzapu/WiFiManager
https://github.com/adafruit/Adafruit_NeoPixel
https://github.com/FastLED/FastLED

There is a warning from FastLED about SPI when compiling. Just ignore it.
```

# QLOCKWORK
### Eine Firmware der Selbstbau-QLOCKTWO.

Sie gleicht die Zeit jede Stunde per NTP mit einem Zeitserver im Internet ab. Auf der Web-Seite kann man die Uhr steuern.
Updates sind OTA moeglich. Dazu im Arduino IDE den ESP als Port auswaehlen.

Nach dem Einschalten macht die Uhr einen kurzen Selbsttest. Die Reihenfolge der Farben ist: weiss, rot, gruen und blau bei maximaler Helligkeit (MAX_BRIGHTNESS).
Wenn die Reihenfolge der Farben anders ist, ist der LED-Treiber falsch eingestellt. Sollte weiss nicht sauber sein, reicht evtl. der Strom nicht aus.
In diesem Fall kann eine zusaetzliche Stromversorgung in der Mitte des Streifens helfen.

Einmal kurz ueber den LDR wischen startet das Einmessen der Helligkeit an die Umgebung.

WLAN Manager: Wenn die Uhr sich beim Start mit keinem WLAN verbinden kann schaltet sie einen AccessPoint ein. Dann ein Handy oder Tablet mit diesem verbinden und die WLAN Daten eingeben. WiFi leuchtet in dieser Zeit weiss auf dem Display. Wenn kein WLAN verbunden wird oder der Timeout abgelaufen ist, gibt es einen langern Ton und WiFi wird rot. Bei Erfolg gibt es drei kurze Toene und WiFi wird gruen. WLAN wird nicht zwingend benoetigt. Nach dem WLAN-Timeout funktioniert die Uhr auch ohne NTP. Dazu benutzt sie die optionale RTC oder muss von Hand gestellt werden.

Ein Schaltplan und eine Stueckliste liegen im Verzeichnis.
Die Firmware gibt es hier: https://github.com/ch570512/Qlockwork

### Standard Modi
```
Zeitanzeige:        Der Standardmodus der Uhr. Er zeigt die Zeit an. :)
                    + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu wechseln.
Anzeige AM/PM:      Zeigt an, ob es vormittags (AM) oder nachmittags (PM) ist.
Sekunden:           Anzeige der Sekunden.
Wochentag:          Zeigt den Wochentag in der eingestellten Sprache an.
Datum:              Anzeige des aktuellen Tages und Monats.
Titel TEMP:         + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu wechseln.
Raumtemperatur:     Anzeige der gemessenen Temperatur (nur mit RTC).
Externe Temperatur: Anzeige der Temperatur für einen Ort (Yahoo Weather).
Titel ALRM:         + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu wechseln.
Timer (TI):         Setzt den Minuten-Timer. Null schaltet den Timer ab.
                    Anzeige der Restzeit wenn ein Timer gesetzt ist.
                    + oder - setzt den Timer und start ihn.
Alarm1 (A1):        ein/aus
Alarm1:             Setzt den ersten 24-Stunden-Alarm wenn Alarm1 "ein" ist.
Alarm2 (A2):        ein/aus
Alarm2:             Setzt den zweiten 24-Stunden-Alarm wenn Alarm2 "ein" ist.
```
### Erweiterte Modi
```
Titel MAIN:                      + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu
                                 wechseln.
Autom. Helligkeitsregelung (LD): Ein (EN) / aus (DA)
Helligkeitsregelung:             + oder - aendern die Helligkeit.
Farbwechsel (CC):                Setzt eine zufaellige Farbe je nach Intervall.
                                 Keiner (NO), alle 5 Minuten (FI), jede Stunde (HR), jeden Tag (DY).
Farbe (CO):                      0: Weiss, 1: Rot, 2: Gruen, 3: Blau usw. Wenn die Reihenfolge der Farben
                                 abweicht, ist die Anordnung der RGB-LEDs im Streifen anders.
Transition (TR):                 Normal (NO) / Fade (FD).
Ruecksprungverzoegerung (FB nn): Wie lange soll es dauern, bis z.B. aus der Sekundenanzeige wieder zurueck in
                                 die Zeitanzeige gewechselt wird. (0: deaktiviert, default: 5)
Sprache (DE/CH/EN/...):          Die passende Sprache zur benutzten Front waehlen.
Titel TIME:                      + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu
                                 wechseln.
Zeit einstellen:                 + fuer Stunden oder - fuer Minuten druecken um die Zeit zu stellen.
                                 Die Sekunden springen mit jedem Druck auf Null.
"Es ist" (IT):                   Anzeigen (EN) oder nicht (DA).
Tag einstellen (DD nn):          + oder - druecken um den aktuellen Tag einzustellen.
Monat einstellen (MM nn):        + oder - druecken um den aktuellen Monat einzustellen.
Jahr einstellen (YY nn):         + oder - druecken um das aktuelle Jahr einzustellen.
Nacht aus (NI OF):               + oder - druecken um die Ausschaltzeit des Displays einzustellen.
Tag ein (DY ON):                 + oder - druecken um die Einschaltzeit des Displays einzustellen.

Titel IP:                        + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu
                                 wechseln.
IP-Adresse:                      Zeigt die lokale IP Adresse im WLAN an.
Titel TEST:                      + oder - druecken um direkt in die naechste bzw. vorhergehende Kategorie zu
                                 wechseln.
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
#define ESP_LED             Displays the function using the LED on the ESP. It flashes once per second.
#define LDR                 Use an LDR for brightness control.
#define LDR_HYSTERESIS      Brightness control from a deviation in the range from 0 to 1023. Default: 30.
#define MIN_BRIGHTNESS      Minimum brightness of LEDs ranging from 0 to 255. Default: 10.
#define MAX_BRIGHTNESS      Maximum brightness of LEDs ranging from 0 to 255. Default 255.
#define NONE_TECHNICAL_ZERO Displays the zero without the diagonal line.
#define BUZZER              Use a buzzer.
#define BUZZTIME_ALARM_1    Maximum time in seconds that turns alarm 1 on when not turned off manually.
#define BUZZTIME_ALARM_2    Maximum time in seconds that turns alarm 2 on when not turned off manually.
#define BUZZTIME_TIMER      Maximum time in seconds that turns the timer alarm on when not turned off manually..
#define YAHOO_WEATHER       Place for the temperature as entered on the Yahoo weather site.
                            (Only letters, ' ', and ',' are allowed).
#define UPDATE_INFO         The update info periodically anonymously checks if there is a firmwareupdate
                            available. No user data is send to the host.
#define LANG_*              Caption of the site-buttons.
#define TIMEZONE_*          The time zone in which the clock is located. Important for the UTC offset and the
                            summer / winter time changeover.
#define IR_REMOTE           Use an IR remote control.
#define IR_CODE_*           Jede Fernbedienung kann verwendet werden. Es werden 6 Tasten unterstuetzt.
                            Um die Fernbedienung anzulernen "#define DEBUG" einschalten und einen Knopf auf der
                            Fernbedienung druecken. Den in der seriellen Konsole angezeigten Code fuer die Taste
                            dann in die Datei "Configuration.h" schreiben.
#define IR_LETTER_OFF       Schaltet die LED hinter dem IR-Sensor dauerhaft ab. Das verbessert den IR-Empfang.
                            Hier das K vor Uhr.

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
D6 = GPIO12 = reserved
D7 = GPIO13 = PIN_LEDS_CLOCK
D8 = GPIO15 = PIN_LEDS_DATA
A0 = ADC0   = PIN_LDR

```
