#ifndef TIME_OP
#define TIME_OP

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "light_op.h"

#define MINUTE ((uint32_t)60)
#define HOUR ((uint32_t)60*60)
#define DAY ((uint32_t)60*60*24)

extern uint32_t epoch;
extern bool timeSyncMode;

uint32_t getTimeNTP(void);

void timeTask(void);
void timeSync(void);

#endif

