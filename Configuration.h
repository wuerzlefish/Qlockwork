/******************************************************************************
   Configuration.h
******************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONFIG_DEFAULT
//#define CONFIG_QLOCKDEV
//#define CONFIG_CLT2

#ifdef CONFIG_DEFAULT

#define HOSTNAME "QLOCKWORK"
#define WIFI_AP_TIMEOUT 120
#define OTA_PASS "1234"
#define NTP_SERVER "pool.ntp.org"

#define RTC_BACKUP
#define RTC_TEMP_OFFSET 0
#define ESP_LED
#define LDR
#define LDR_HYSTERESIS 30
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 255
//#define NONE_TECHNICAL_ZERO
#define BUZZTIME_ALARM_1 60
#define BUZZTIME_ALARM_2 60
#define BUZZTIME_TIMER 30

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

//#define IR_REMOTE
//#define IR_LETTER_OFF
#define IR_CODE_ONOFF   16769565 // HX1838 Remote CH+
#define IR_CODE_TIME    16753245 // HX1838 Remote CH-
#define IR_CODE_MODE    16736925 // HX1838 Remote CH
#define IR_CODE_EXTMODE 16748655 // HX1838 Remote EQ
#define IR_CODE_PLUS    16754775 // HX1838 Remote +
#define IR_CODE_MINUS   16769055 // HX1838 Remote -

#define LED_LAYOUT_HORIZONTAL
//#define LED_LAYOUT_VERTICAL

//#define LED_LIBRARY_LPD8806RGBW

#define LED_LIBRARY_NEOPIXEL
#define LED_DRIVER_NEO_800KHZ_GRB
//#define LED_DRIVER_NEO_800KHZ_RGB
//#define LED_DRIVER_NEO_800KHZ_RGBW
//#define LED_DRIVER_NEO_400KHZ_GRB
//#define LED_DRIVER_NEO_400KHZ_RGB

//#define LED_LIBRARY_FASTLED
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
//#define LED_DRIVER_FAST_NEOPIXEL
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

//#define DEBUG
//#define DEBUG_WEBSITE
//#define DEBUG_MATRIX
//#define DEBUG_FPS

#endif // CONFIG_DEFAULT

#ifdef CONFIG_QLOCKDEV
#define HOSTNAME "QLOCKDEV"
#define WIFI_AP_TIMEOUT 60
#define OTA_PASS "1234"
#define NTP_SERVER "nostromo"
#define RTC_BACKUP
#define RTC_TEMP_OFFSET 0
#define ESP_LED
#define LDR
#define LDR_HYSTERESIS 150
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 255
#define BUZZTIME_ALARM_1 60
#define BUZZTIME_ALARM_2 3
#define BUZZTIME_TIMER 30
#define TIMEZONE_CET
#define IR_REMOTE
#define IR_CODE_ONOFF   16769565
#define IR_CODE_TIME    16753245
#define IR_CODE_MODE    16736925
#define IR_CODE_EXTMODE 16748655
#define IR_CODE_PLUS    16754775
#define IR_CODE_MINUS   16769055
#define LED_LAYOUT_HORIZONTAL
//#define LED_LIBRARY_FASTLED
//#define LED_DRIVER_FAST_NEOPIXEL
#define LED_LIBRARY_NEOPIXEL
#define LED_DRIVER_NEO_800KHZ_GRB
//#define LED_LIBRARY_LPD8806RGBW
#define DEBUG
#define DEBUG_WEBSITE
//#define DEBUG_MATRIX
//#define DEBUG_FPS
#endif // CONFIG_QLOCKDEV

#ifdef CONFIG_CLT2
#define HOSTNAME "CLT2"
#define WIFI_AP_TIMEOUT 60
#define OTA_PASS "1234"
#define NTP_SERVER "nostromo"
#define RTC_BACKUP
#define RTC_TEMP_OFFSET -4.25
#define LDR
#define LDR_HYSTERESIS 30
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 255
#define BUZZTIME_ALARM_1 60
#define BUZZTIME_ALARM_2 1
#define BUZZTIME_TIMER 30
#define TIMEZONE_CET
#define IR_REMOTE
#define IR_LETTER_OFF
#define IR_CODE_ONOFF   16769055
#define IR_CODE_TIME    16752735
#define IR_CODE_MODE    16720095
#define IR_CODE_EXTMODE 16736415
#define IR_CODE_PLUS    16734375
#define IR_CODE_MINUS   16730295
#define LED_LAYOUT_VERTICAL
#define LED_LIBRARY_LPD8806RGBW
#define DEBUG_WEBSITE
#endif // CONFIG_CLT2

#define FIRMWARE_VERSION "qw20170604"
#define SERIAL_SPEED 115200
#define NUM_LEDS 115
#define COLOR_COUNT 36
#define TRANSITION_COUNT 1
#define LANGUAGE_COUNT 15
#define PIN_IR_RECEIVER D3
#define PIN_LED         D4
#define PIN_BUZZER      D5
#define PIN_LEDS_CLOCK  D7
#define PIN_LEDS_DATA   D8
#define PIN_LDR         A0

#endif
