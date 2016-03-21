#ifndef WIFI_OPERATION
#define WIFI_OPERATION

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <FS.h>
#include "config.h"
#include "server_op.h"

extern const char softap_ssid[];
extern const char softap_pass[];
extern String target_ssid;
extern String target_pass;

void wifiSetup();

void setupAP();

void closeAP();

int connectWifi();

void wifiRestoreFromFile();

void wifiBackupToFile();

// extracts a string between "head" and "tail"
String extract(String target, String head, String tail = "&");

#endif

