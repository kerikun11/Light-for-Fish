#ifndef SERVER_OPERATION
#define SERVER_OPERATION

#include <ESP8266WiFi.h>
#include "config.h"
#include "WiFi_op.h"
#include "light_op.h"
#include "time_op.h"

// TCP server at port 80 will respond to HTTP requests
extern ESP8266WebServer server;
extern String mdns_address;

void setupAPServer();
String generateAPHtml(String status);
String handleAPRequest();
void handleAPRoot();

void setupServer();
String generateHtml(String status);
String handleRequest();
void handleRoot();

void serverTask();

// replace a number to the symbol
void charEncode(String &s);

#endif

