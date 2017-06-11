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
2 Transitions for timechange (Normal, Fade).
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
External Temperature (Yahoo weather).
Timer.
Alarm 1.
Alarm 2.
```
### Extended modes
```
Brightnesscontrol.
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

WiFi Manager: Wenn die Uhr sich beim Start mit keinem WLAN verbinden kann, schaltet sie einen AccessPoint ein. Dann ein Handy oder Tablet mit diesem verbinden und die WLAN Daten eingeben. WiFi wird nicht zwingend benoetigt. Nach dem WiFi-Timeout funktioniert die Uhr auch ohne NTP. Dazu benutzt sie die optionale RTC oder muss von Hand gestellt werden.

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
Farbe (CO):                      0: Weiss, 1: Rot, 2: Gruen, 3: Blau usw. Wenn die Reihenfolge der Farben abweicht,
                                 ist die Anordnung der RGB-LEDs im Streifen anders.
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
LED-Test:                        Laesst einen waagerechten Streifen ueber das Display wandern.
```
### Configuration.h
```

#define CONFIG_*            Einfache Unterstuetzung verschiedener Konfigurationen in einem File.
#define HOSTNAME            Der Name der Uhr.
#define WIFI_AP_TIMEOUT     Zeit in Sekunden fuer die der AP zum einrichten/suchen des WLANs aktiv ist.
#define OTA_PASS            Kennwort fuer "Over the Air" Updates.
#define NTP_SERVER          Abzufragender NTP-Server.
#define RTC_BACKUP          Eine RTC als Backup verwenden.
#define RTC_TEMP_OFFSET     Gibt an, um wieviel Grad die gemessene Temperatur (+ oder -) korrigiert werden soll.
#define ESP_LED             Zeigt mit Hilfe der LED auf dem ESP die Funktion an. Sie blinkt einmal pro Sekunde.
#define LDR                 Einen LDR fuer die Helligkeitsregelung verwenden.
#define LDR_HYSTERESIS      Helligkeitsregelung ab einer Abweichung im Bereich von 0 bis 1023. Default: 30.
#define MIN_BRIGHTNESS      Minimale Helligkeit der LEDs im Bereich von 0 bis 255. Default: 10.
#define MAX_BRIGHTNESS      Maximale Helligkeit der LEDs im Bereich von 0 bis 255. Default 255.
#define NONE_TECHNICAL_ZERO Zeigt die Null ohne den diagonalen Strich.
#define BUZZER              Einen Buzzer verwenden.
#define BUZZTIME_ALARM_1    Maximale Zeit in Sekunden, die Alarm 1 Laerm macht wenn nicht manuell abgestellt.
#define BUZZTIME_ALARM_2    Maximale Zeit in Sekunden, die Alarm 2 Laerm macht wenn nicht manuell abgestellt.
#define BUZZTIME_TIMER      Maximale Zeit in Sekunden, die der Timer Laerm macht wenn nicht manuell abgestellt.
#define YAHOO_WEATHER       Ort fuer die Temperatur wie er auf der Yahoo-Wetter-Site eingegeben wird.
                            (Nur Buchstaben, ' ', und ',').
#define LANG_*              Bezeichnung der Buttons auf der Web-Seite.
#define TIMEZONE_*          Die Zeitzone in der sich die Uhr befindet. Wichtig fuer den UTC-Versatz und die
                            Sommer-/Winterzeitumstellung.
#define IR_REMOTE           Eine IR-Fernbedienung verwenden.
#define IR_CODE_*           Jede Fernbedienung kann verwendet werden. Es werden 6 Tasten unterstuetzt.
                            Um die Fernbedienung anzulernen "#define DEBUG" einschalten und einen Knopf auf der
                            Fernbedienung druecken. Den in der seriellen Konsole angezeigten Code fuer die Taste
                            dann in die Datei "Configuration.h" schreiben.
#define IR_LETTER_OFF       Schaltet die LED hinter dem IR-Sensor dauerhaft ab. Das verbessert den IR-Empfang.
                            Hier das K vor Uhr.

#define LED_LAYOUT_HORIZONTAL Waagerecht und Eck-LEDs am Ende des Stripes. (Von vorne gesehen.)

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

#define LED_LAYOUT_VERTICAL   Senkrecht und Eck-LEDs innerhalb des Stripes. (Von vorne gesehen.)

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

#define LED_LIBRARY_LPD8806RGBW LED Driver fuer LPD8806 RGBW LEDs.

#define LED_LIBRARY_NEOPIXEL    LED Driver fuer NeoPixel LEDs.
#define LED_DRIVER_NEO_*        Gibt in Verbindung mit LED_LIBRARAY_NEOPIXEL den Typ der NeoPixel an.
                                400kHz, 800kHz, GRB, RGB und RGBW.

#define LED_LIBRARY_FASTLED     FastLED Driver fuer LEDs.
#define LED_DRIVER_FAST_*       Gibt in Verbindung mit LED_LIBRARAY_FASTLED den Typ der LEDs an.
                                Alle von FAST-LED unterstuetzten LED-Treiber (nur RGB) koennen verwendet
                                werden:
                                APA102, APA104, APA106, DOTSTAR, DMXSIMPLE, GW6205, GW6205_400, LPD1886,
                                LPD1886_8BIT, LPD8806, NEOPIXEL, P9813, PL9823, SK6812, SK6822, SK9822,
                                SM16716, TM1803, TM1804, TM1809, TM1812, TM1829, UCS1903, UCS1903B,
                                UCS1904, UCS2903, WS2801, WS2803, WS2811, WS2811_400, WS2812, WS2812B,
                                WS2813, WS2852.

#define SERIAL_SPEED  Geschwindigkeit der seriellen Schnittstelle fuer die serielle Konsole.
#define DEBUG         Gibt technische Informationen in der seriellen Konsole aus.
#define DEBUG_WEBSITE Gibt technische Informationen auf der Web-Seite aus.
#define DEBUG_MATRIX  Rendert die Ausgabe der Matrix fuer die deutsche Front in der seriellen Konsole.
#define DEBUG_FPS     Durchlaeufe der loop() pro Sekunde.

Hardwarebelegung des ESP:

D0 = GPIO16 = NodeMCU_LED
D1 = GPIO05 = PIN_WIRE_SCL
D2 = GPIO04 = PIN_WIRE_SDA
D3 = GPIO00 = PIN_IR_RECEIVER
D4 = GPIO02 = ESP8266_LED
D5 = GPIO14 = PIN_BUZZER
D6 = GPIO12 = nc
D7 = GPIO13 = PIN_LEDS_CLOCK
D8 = GPIO15 = PIN_LEDS_DATA
A0 = ADC0   = PIN_LDR

```
