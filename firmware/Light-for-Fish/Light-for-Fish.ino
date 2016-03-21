/*
   Light-for-Fish Ver.1.0.0
   Infrared Remote Controller with ESP8266 WiFi Module

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
  ESP.wdtFeed();
  // Prepare Serial debug
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  println_dbg("");
  println_dbg("Hello, I'm ESP-WROOM-02");

  // prepare GPIO
  pinMode(Indicate_LED, OUTPUT);
  pinMode(ERROR_LED, OUTPUT);
  pinMode(IR_IN, INPUT);
  pinMode(IR_OUT, OUTPUT);

  digitalWrite(Indicate_LED, LOW);
  digitalWrite(ERROR_LED, LOW);
  analogWrite(PIN_RED, 1);
  analogWrite(PIN_GREEN, 1);
  analogWrite(PIN_BLUE, 1);
  analogWrite(PIN_WHITE, 1);

  // Setup Start
  ERROR_ON();

  // Prepare SPIFFS
  bool res = SPIFFS.begin();
  if (!res) println_dbg("SPIFFS.begin fail");

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
  ERROR_OFF();
  println_dbg("Setup Completed");
}

void loop() {
  ESP.wdtFeed();
  serverTask();
  OTATask();
  lightTask();
  if (WiFi.status() != WL_CONNECTED) {
    ERROR_ON();
  } else {
    ERROR_OFF();
  }
}

