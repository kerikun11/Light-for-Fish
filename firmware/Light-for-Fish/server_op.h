#ifndef SERVER_OPERATION_H
#define SERVER_OPERATION_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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

