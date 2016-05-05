#ifndef WIFI_OPERATION_H
#define WIFI_OPERATION_H

#include <ESP8266WiFi.h>

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

