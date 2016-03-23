#ifndef TIME_OP
#define TIME_OP

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "config.h"

extern const char weekdayCharJap[8][4];

void setupTime();
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
String inTwoDigits(int n);

void sendNTPpacket(IPAddress &address);

#endif

