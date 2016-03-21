/*
   config.h
*/
#ifndef CONFIG
#define CONFIG


/* Hardware Mapping */
#define IR_IN                   (5)
#define IR_OUT                  (14)
#define PIN_RED                 (12)
#define PIN_GREEN               (15)
#define PIN_BLUE                (13)
#define PIN_WHITE               (4)
#define Indicate_LED            (16)
#define ERROR_LED               (16)


/* Software */
// for Remocon
#define IR_CH_SIZE              (0)

// SPIFFS saving path
#define IR_DATA_PATH(i)         ("/data/" + String(i,DEC) + ".txt")
#define WIFI_DATA_PATH          ("/wifi.txt")

// SSID & Password of ESP8266 Access Point Mode
#define SOFTAP_SSID             "Light-for-Fish"
#define SOFTAP_PASS             ""

#define WIFI_CONNECT_TIMEOUT    (10) // seconds

// http://DEFAULT_MDNS_ADDRESS.local
#define DEFAULT_MDNS_ADDRESS    "esp8266"

// OTA
#define OTA_HOST_NAME           "fish"
#define OTA_PASSWORD            "1234"

// Software Reset
#define RESET()                 while(true); // WDT RESET

#define ERROR_ON()              do{analogWrite(PIN_WHITE, 511);}while(0)
#define ERROR_OFF()             do{analogWrite(PIN_WHITE, 0);}while(0)

/* for Debug */
#define SERIAL_DEBUG            true

#if SERIAL_DEBUG == true
#define print_dbg               Serial.print
#define println_dbg             Serial.println
#else
#define print_dbg               // No Operation
#define println_dbg             // No Operation
#endif

#endif

