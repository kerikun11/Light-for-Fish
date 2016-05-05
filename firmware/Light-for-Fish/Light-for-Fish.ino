/*
   Light-for-Fish Ver.1.0.0

   Author:  kerikun11 (Github: kerikun11)
   Date:    2016.03.21

   1. Add ESP8266 Board to Arduino IDE in Preferences.
      Put URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
   2. Tool -> Board Settings:
          Board:           ESPDuino (ESP-13 Module)
          Upload Using:    Serial
          CPU Frequency:   80MHz
          Flash Size:      4M (3M SPIFFS)
          Upload Speed:    115200
   3. Upload the program to the ESP8266 WiFi Module.
*/

#include <ESP8266WiFi.h>
#include <FS.h>
#include "config.h"
#include "OTA_op.h"
#include "WiFi_op.h"
#include "light_op.h"
#include "time_op.h"
#include "server_op.h"

void setup() {
  // Prepare Serial debug
  Serial.begin(115200);
  println_dbg("");
  println_dbg("Hello, I'm ESP-WROOM-02");

  // prepare GPIO
  analogWrite(PIN_RED, 1);
  analogWrite(PIN_GREEN, 1);
  analogWrite(PIN_BLUE, 1);
  analogWrite(PIN_WHITE, 1);

  // Setup Start
  INDICATOR_ON();

  // Prepare SPIFFS
  SPIFFS.begin();

  // Restore reserved data
  wifiRestoreFromFile();

  // WiFi setup
  wifiSetup();

  // OTA setup
  setupOTA();

  // Time setup
  setupTime();

  // WebServer Setup
  setupServer();

  // Setup Completed
  INDICATOR_OFF();
  println_dbg("Setup Completed");
}

void loop() {
  serverTask();
  OTATask();
  lightTask();
  
  if (WiFi.status() != WL_CONNECTED) INDICATOR_ON();
}

