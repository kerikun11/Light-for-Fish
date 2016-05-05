#ifndef TIME_OP_H
#define TIME_OP_H

#include <ESP8266WiFi.h>

extern const char weekdayCharJap[8][4];

void setupTime();
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
String inTwoDigits(int n);

void sendNTPpacket(IPAddress &address);

#endif

