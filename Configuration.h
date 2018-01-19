/******************************************************************************
Configuration.h
See README.txt for help.
******************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/******************************************************************************
Software settings.
******************************************************************************/

#define HOSTNAME "QLOCKWORK"
#define WIFI_AP_TIMEOUT 120
#define WIFI_AP_PASS "12345678"
#define OTA_PASS "1234"
#define NTP_SERVER "pool.ntp.org"
#define NTP_TIMEOUT 2000
#define SHOW_IP
//#define NONE_TECHNICAL_ZERO
#define AUTO_MODECHANGE_TIME 60
#define FEED_SPEED 120
#define EVENT_TIME 300
#define ALARM_LED_COLOR RED
//#define ABUSE_CORNER_LED_FOR_ALARM
//#define DEDICATION "The only reason for time is so that everything doesn't happen at once.<br>(Albert Einstein)"

#define SELFTEST
//--------------
#define BRIGHTNESS_SELFTEST 100

#define LOCATION "Berlin, BE, DE"
//#define LOCATION "Zurich, ZH, CH"
//#define LOCATION "Singapur, SG"

#define FRONTCOVER_EN
//#define FRONTCOVER_DE_DE
//#define FRONTCOVER_DE_SW
//#define FRONTCOVER_DE_BA
//#define FRONTCOVER_DE_SA
//#define FRONTCOVER_DE_MKF_DE
//#define FRONTCOVER_DE_MKF_SW
//#define FRONTCOVER_DE_MKF_BA
//#define FRONTCOVER_DE_MKF_SA
//#define FRONTCOVER_D3
//#define FRONTCOVER_CH
//#define FRONTCOVER_CH_GS
//#define FRONTCOVER_ES
//#define FRONTCOVER_FR
//#define FRONTCOVER_IT
//#define FRONTCOVER_NL

#define SHOW_MODE_AMPM
#define SHOW_MODE_SECONDS
#define SHOW_MODE_WEEKDAY
#define SHOW_MODE_DATE
#define SHOW_MODE_MOONPHASE
#define SHOW_MODE_TEST

/******************************************************************************
Timezone.
******************************************************************************/

//#define TIMEZONE_IDLW  // IDLW  Internation Date Line West UTC-12
//#define TIMEZONE_SST   // SST   Samoa Standard Time UTC-11
//#define TIMEZONE_HST   // HST   Hawaiian Standard Time UTC-10
//#define TIMEZONE_AKST  // AKST  Alaska Standard Time UTC-9
//#define TIMEZONE_USPST // USPST Pacific Standard Time (USA) UTC-8
//#define TIMEZONE_USMST // USMST Mountain Standard Time (USA) UTC-7
//#define TIMEZONE_USAZ  // USAZ  Mountain Standard Time (USA) UTC-7 (no DST)
//#define TIMEZONE_USCST // USCST Central Standard Time (USA) UTC-6
//#define TIMEZONE_USEST // USEST Eastern Standard Time (USA) UTC-5
//#define TIMEZONE_AST   // AST   Atlantic Standard Time UTC-4
//#define TIMEZONE_BST   // BST   Eastern Brazil Standard Time UTC-3
//#define TIMEZONE_VTZ   // VTZ   Greenland Eastern Standard Time UTC-2
//#define TIMEZONE_AZOT  // AZOT  Azores Time UTC-1
//#define TIMEZONE_GMT   // GMT   Greenwich Mean Time UTC
#define TIMEZONE_CET     // CET   Central Europe Time UTC+1
//#define TIMEZONE_EST   // EST   Eastern Europe Time UTC+2
//#define TIMEZONE_MSK   // MSK   Moscow Time UTC+3 (no DST)
//#define TIMEZONE_GST   // GST   Gulf Standard Time UTC+4
//#define TIMEZONE_PKT   // PKT   Pakistan Time UTC+5
//#define TIMEZONE_BDT   // BDT   Bangladesh Time UTC+6
//#define TIMEZONE_JT    // JT    Java Time UTC+7
//#define TIMEZONE_CNST  // CNST  China Standard Time UTC+8
//#define TIMEZONE_HKT   // HKT   Hong Kong Time UTC+8
//#define TIMEZONE_PYT   // PYT   Pyongyang Time (North Korea) UTC+8.5
//#define TIMEZONE_CWT   // CWT   Central West Time (Australia) UTC+8.75
//#define TIMEZONE_JST   // JST   Japan Standard Time UTC+9
//#define TIMEZONE_ACST  // ACST  Australian Central Standard Time UTC+9.5
//#define TIMEZONE_AEST  // AEST  Australian Eastern Standard Time UTC+10
//#define TIMEZONE_LHST  // LHST  Lord Howe Standard Time UTC+10.5
//#define TIMEZONE_SBT   // SBT   Solomon Islands Time UTC+11
//#define TIMEZONE_NZST  // NZST  New Zealand Standard Time UTC+12

/******************************************************************************
Hardware settings.
******************************************************************************/

#define ESP_LED

//#define SENSOR_DHT22
//--------------------
#define DHT_TEMPERATURE_OFFSET 0.3
#define DHT_HUMIDITY_OFFSET 0.4

//#define RTC_BACKUP
//------------------
#define RTC_TEMPERATURE_OFFSET 0.25

//#define LDR
//-----------
//#define LDR_IS_INVERSE
#define LDR_HYSTERESIS 40
#define MIN_BRIGHTNESS 20
#define MAX_BRIGHTNESS 255

//#define BUZZER
//--------------
#define BUZZTIME_ALARM_1 30
#define BUZZTIME_ALARM_2 30
#define BUZZTIME_TIMER 30

//#define IR_RECEIVER
//-------------------
//#define IR_LETTER_OFF
//#define IR_CODE_ONOFF    16769565 // HX1838 Remote CH+
//#define IR_CODE_TIME     16753245 // HX1838 Remote CH-
//#define IR_CODE_MODE     16736925 // HX1838 Remote CH
#define IR_CODE_ONOFF    0xFFE01F // CLT2 V1.1 Remote Power
#define IR_CODE_TIME     0xFFA05F // CLT2 V1.1 Remote Time
#define IR_CODE_MODE     0xFF20DF // CLT2 V1.1 Remote Region

#define LED_LAYOUT_HORIZONTAL
//#define LED_LAYOUT_VERTICAL

//#define LED_LIBRARY_LPD8806RGBW
//-------------------------------

#define LED_LIBRARY_NEOPIXEL
//----------------------------
#define LED_DRIVER_NEO_800KHZ_GRB
//#define LED_DRIVER_NEO_800KHZ_RGB
//#define LED_DRIVER_NEO_800KHZ_GRBW
//#define LED_DRIVER_NEO_800KHZ_RGBW
//#define LED_DRIVER_NEO_400KHZ_GRB
//#define LED_DRIVER_NEO_400KHZ_RGB

//#define LED_LIBRARY_FASTLED
//---------------------------
//#define LED_DRIVER_FAST_APA102
//#define LED_DRIVER_FAST_APA104
//#define LED_DRIVER_FAST_APA106
//#define LED_DRIVER_FAST_DOTSTAR
//#define LED_DRIVER_FAST_DMXSIMPLE
//#define LED_DRIVER_FAST_GW6205
//#define LED_DRIVER_FAST_GW6205_400
//#define LED_DRIVER_FAST_LPD1886
//#define LED_DRIVER_FAST_LPD1886_8BIT
//#define LED_DRIVER_FAST_LPD8806
#define LED_DRIVER_FAST_NEOPIXEL
//#define LED_DRIVER_FAST_P9813
//#define LED_DRIVER_FAST_PL9823
//#define LED_DRIVER_FAST_SK6812
//#define LED_DRIVER_FAST_SK6822
//#define LED_DRIVER_FAST_SK9822
//#define LED_DRIVER_FAST_SM16716
//#define LED_DRIVER_FAST_TM1803
//#define LED_DRIVER_FAST_TM1804
//#define LED_DRIVER_FAST_TM1809
//#define LED_DRIVER_FAST_TM1812
//#define LED_DRIVER_FAST_TM1829
//#define LED_DRIVER_FAST_UCS1903
//#define LED_DRIVER_FAST_UCS1903B
//#define LED_DRIVER_FAST_UCS1904
//#define LED_DRIVER_FAST_UCS2903
//#define LED_DRIVER_FAST_WS2801
//#define LED_DRIVER_FAST_WS2803
//#define LED_DRIVER_FAST_WS2811
//#define LED_DRIVER_FAST_WS2811_400
//#define LED_DRIVER_FAST_WS2812
//#define LED_DRIVER_FAST_WS2812B
//#define LED_DRIVER_FAST_WS2813
//#define LED_DRIVER_FAST_WS2852

/******************************************************************************
Default values for EEPROM.
******************************************************************************/

#define DEFAULT_BRIGHTNESS 50
#define DEFAULT_COLOR WHITE
#define DEFAULT_COLORCHANGE COLORCHANGE_NO
#define DEFAULT_SHOWTEMP false
#define DEFAULT_SHOWITIS true
#define DEFAULT_TRANSITION TRANSITION_FADE
#define DEFAULT_TIMEOUT 10
#define DEFAULT_USEABC true
#define DEFAULT_ALARM1 false
#define DEFAULT_ALARM1TIME 0
#define DEFAULT_ALARM1WEEKDAYS 0b11111110
#define DEFAULT_ALARM2 false
#define DEFAULT_ALARM2TIME 0
#define DEFAULT_ALARM2WEEKDAYS 0b11111110
#define DEFAULT_NIGHTOFF 3600
#define DEFAULT_DAYON 18000

/******************************************************************************
Misc.
******************************************************************************/

#define SERIAL_SPEED 115200

//#define DEBUG
//#define DEBUG_WEB
//#define DEBUG_MATRIX
//#define DEBUG_FPS

//#define SYSLOGSERVER
//------------------
#define SYSLOGSERVER_SERVER "192.168.0.1"
#define SYSLOGSERVER_PORT 514

#define UPDATE_INFO_UNSTABLE
//#define UPDATE_INFO_STABLE
//--------------------------
#define UPDATE_INFOSERVER "tmw-it.ch"
#define UPDATE_INFOFILE "/qlockwork/updateinfo.json"

// ESP8266
//#define none          D0 // GPIO 16   NodeMCU_LED
//#define PIN_WIRE_SCL  D1 // GPIO 05
//#define PIN_WIRE_SDA  D2 // GPIO 04
#define PIN_IR_RECEIVER D3 // GPIO 00 *
#define PIN_LED         D4 // GPIO 02 * ESP8266_LED
#define PIN_BUZZER      D5 // GPIO 14
#define PIN_DHT22       D6 // GPIO 12
#define PIN_LEDS_CLOCK  D7 // GPIO 13
#define PIN_LEDS_DATA   D8 // GPIO 15 *
//#define none          D9 // GPIO 03   RXD0
//#define none         D10 // GPIO 01   TXD0
#define PIN_LDR         A0 // ADC
// GPIO 06 - GPIO 11 flash memory databus
// * used to enter flash mode.

#endif
