/*
   config.h
*/
#ifndef CONFIG_H
#define CONFIG_H


/*
   Hardware Mapping
*/
#define PIN_RED                 (12)
#define PIN_GREEN               (15)
#define PIN_BLUE                (13)
#define PIN_WHITE               (4)


/*
    Software
*/
// SPIFFS saving path
#define WIFI_DATA_PATH          ("/wifi.txt")

// SSID & Password of ESP8266 Access Point Mode
#define SOFTAP_SSID             "Light-for-Fish"
#define SOFTAP_PASS             ""

#define WIFI_CONNECT_TIMEOUT    (10) // seconds

// http://DEFAULT_MDNS_ADDRESS.local
#define DEFAULT_MDNS_ADDRESS    "fish"

// OTA
#define OTA_HOSTNAME            "fish"
#define OTA_PASSWORD            "1234"

// Software Reset
#define RESET()                 while(true); // WDT RESET

#define INDICATOR_ON()          do{analogWrite(PIN_WHITE, 511);}while(0)
#define INDICATOR_OFF()         do{analogWrite(PIN_WHITE, 0);}while(0)


/*
  for Debug
*/
#define SERIAL_DEBUG            true

#if SERIAL_DEBUG == true
#define print_dbg               Serial.print
#define printf_dbg              Serial.printf
#define println_dbg             Serial.println
#else
#define print_dbg               // No Operation
#define printf_dbg              // No Operation
#define println_dbg             // No Operation
#endif

#endif

