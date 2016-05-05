#include "server_op.h"

#include <ESP8266mDNS.h>
#include "config.h"
#include "WiFi_op.h"
#include "light_op.h"
#include "time_op.h"

const String html_head =
  "<!DOCTYPE HTML><html><head>"
  "<style type=\"text/css\">"
  "body{background-color:#BBDDFF;margin:5%;text-align:center;font-size:2em}"
  ".mode button{font-size:1em;width:45%;}"
  ".color button{font-size:1em;width:8%;padding:0;}"
  "button#grey{background-color:#888;color:white;}"
  "button#red{background-color:red;color:white;}"
  "button#green{background-color:green;color:white;}"
  "button#blue{background-color:blue;color:white;}"
  "button#white{background-color:white;color:black;}"
  "</style>"
  "<link rel=\"shortcut icon\" href=\"http://kerikeri.top/esp8266.png\"/>"
  "<title>Killifish Light</title></head>\r\n"
  "<body><h1>--- Killifish Light ---</h1>";
const String html_tail = "</body></html>\r\n";

// TCP server at port 80 will respond to HTTP requests
ESP8266WebServer server(80);
String mdns_address = DEFAULT_MDNS_ADDRESS;

void setupAPServer() {
  // Set up mDNS responder:
  print_dbg("mDNS address: ");
  println_dbg("http://" + mdns_address + ".local");
  if (!MDNS.begin(DEFAULT_MDNS_ADDRESS, WiFi.softAPIP())) {
    println_dbg("Error setting up MDNS responder!");
  } else {
    println_dbg("mDNS responder started");
  }
  MDNS.addService("http", "tcp", 80);

  server.on("/", handleAPRoot);

  // Start TCP (HTTP) server
  server.begin();
  println_dbg("TCP server started");
  println_dbg("Listening");
}

String generateAPHtml(String status) {
  String html_ssidpassform = "";
  html_ssidpassform +=  "<form method=\"get\"><p>SSID:<select name=\"ssid\"/>";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    html_ssidpassform += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
  }
  html_ssidpassform +=
    "</select></p>"
    "<p>Password:<input type=\"password\" name=\"pass\"/></p>"
    "<p>URL:<input type=\"text\" name=\"url\"/></p>"
    "<p><button type=\"submit\" name=\"End\">OK</button></p></form>";

  String html_status = "";
  html_status += status;

  return html_head + html_status + html_ssidpassform + html_tail;
}

String handleAPRequest() {
  String status = "<p>";
  // Match the request
  if (server.argName(0) == "ssid") {
    target_ssid = server.arg(0);
    target_pass = server.arg(1);
    mdns_address = server.arg(2);
    if (mdns_address == "") {
      mdns_address = DEFAULT_MDNS_ADDRESS;
    }
    println_dbg("Target SSID: " + target_ssid);
    println_dbg("Target Password: " + target_pass);
    println_dbg("mDNS Address: " + mdns_address);

    status += "Connecteding to " + target_ssid;
  } else {
    status += "Please fill up the blanks bellow.";
  }
  status += "</p>";
  return status;
}

void handleAPRoot() {
  // Request detail
  println_dbg("");
  println_dbg("New Request");
  //println_dbg("URI: " + server.uri());
  //println_dbg("Method: " + (server.method() == HTTP_GET) ? "GET" : "POST");
  println_dbg("Arguments: " + String(server.args()));
  for (uint8_t i = 0; i < server.args(); i++) {
    println_dbg("  " + server.argName(i) + " = " + server.arg(i));
  }

  // Prepare the response
  String status = handleAPRequest();

  // Prepare the response
  String response = generateAPHtml(status);

  // Send the response
  server.send(200, "text/html", response);
  delay(100);

  // if WiFi is configured, reboot
  if (server.argName(0) == "ssid") {
    wifiBackupToFile();
    closeAP();
    RESET();
  }
}

void setupServer() {
  // Set up mDNS responder:
  print_dbg("mDNS address: ");
  println_dbg("http://" + mdns_address + ".local");
  if (!MDNS.begin(mdns_address.c_str(), WiFi.localIP())) {
    println_dbg("Error setting up MDNS responder!");
  } else {
    println_dbg("mDNS responder started");
  }

  server.on("/", handleRoot);

  // Start TCP (HTTP) server
  server.begin();
  println_dbg("TCP server started");
  println_dbg("Listening");
}

String generateHtml(String status) {
  String html_set_buttons = "";
  html_set_buttons +=
    "<form class=\"mode\" method=\"get\">"
    "<p>"
    "<button type=\"submit\" name=\"mode\" value=\"0\">Set Time Sync</button>"
    "</p>"
    "</form>";

  String html_color_buttons = "";
  html_set_buttons +=
    "<form class=\"color\" method=\"get\">"
    "<p>";
  for (int i = 0; i <= 10; i++) {
    html_set_buttons +=
      "<button ";
    if ((uint32_t)(red.pulseWidth + 1) * 10 / 0x3FF == i) {
      html_set_buttons += "id=\"red\"";
    } else {
      html_set_buttons += "id=\"grey\"";
    }
    html_set_buttons +=
      "name=\"red\" value=\"" + String(i) + "\">" + String(i) + "</button>";
  }
  html_set_buttons +=
    "</p><p>";
  for (int i = 0; i <= 10; i++) {
    html_set_buttons +=
      "<button ";
    if ((uint32_t)(green.pulseWidth + 1) * 10 / 0x3FF == i) {
      html_set_buttons += "id=\"green\"";
    } else {
      html_set_buttons += "id=\"grey\"";
    }
    html_set_buttons +=
      "name=\"green\" value=\"" + String(i) + "\">" + String(i) + "</button>";
  }
  html_set_buttons +=
    "</p><p>";
  for (int i = 0; i <= 10; i++) {
    html_set_buttons +=
      "<button ";
    if ((uint32_t)(blue.pulseWidth + 1) * 10 / 0x3FF == i) {
      html_set_buttons += "id=\"blue\"";
    } else {
      html_set_buttons += "id=\"grey\"";
    }
    html_set_buttons +=
      "name=\"blue\" value=\"" + String(i) + "\">" + String(i) + "</button>";
  }
  html_set_buttons +=
    "</p><p>";
  for (int i = 0; i <= 10; i++) {
    html_set_buttons +=
      "<button ";
    if ((uint32_t)(white.pulseWidth + 1) * 10 / 0x3FF == i) {
      html_set_buttons += "id=\"white\"";
    } else {
      html_set_buttons += "id=\"grey\"";
    }
    html_set_buttons +=
      "name=\"white\" value=\"" + String(i) + "\">" + String(i) + "</button>";
  }
  html_set_buttons +=
    "</p>";

  html_set_buttons +=
    "</form>";

  String html_menu_buttons =
    "<form method=\"get\" style=\"font-size:1em;\"><p>"
    "<button type=\"submit\" name=\"chwifi\">Change WiFi</button>"
    "</p></form>";

  String html_status;
  html_status = status;
  html_status += "<p>Connecting SSID : " + target_ssid + "</p>";
  html_status += "<p>IP address : ";
  html_status += String((WiFi.localIP() >> 0) & 0xFF, DEC) + ".";
  html_status += String((WiFi.localIP() >> 8) & 0xFF, DEC) + ".";
  html_status += String((WiFi.localIP() >> 16) & 0xFF, DEC) + ".";
  html_status += String((WiFi.localIP() >> 24) & 0xFF, DEC);
  html_status += "</p>";
  html_status += "<p>URL : http://" + mdns_address + ".local</p>";

  return html_head + html_set_buttons + html_color_buttons + html_menu_buttons + html_status + html_tail;
}

String handleRequest() {
  String status = "<p>Status: ";
  // Match the request
  if (server.argName(0) == "mode") {
    uint8_t mode = server.arg(0).toInt();
    if (mode == 0) {
      timeSyncMode = true;
      timeSync();
      status += "Time Sync Mode";
    } else {
      timeSyncMode = false;
      status += "Manual Control";
    }
  } else if (server.argName(0) == "red") {
    timeSyncMode = false;
    red.set(0x3FF * server.arg(0).toInt() / 10);
    status += "Manual Control";
  } else if (server.argName(0) == "green") {
    timeSyncMode = false;
    green.set(0x3FF * server.arg(0).toInt() / 10);
    status += "Manual Control";
  } else if (server.argName(0) == "blue") {
    timeSyncMode = false;
    blue.set(0x3FF * server.arg(0).toInt() / 10);
    status += "Manual Control";
  } else if (server.argName(0) == "white") {
    timeSyncMode = false;
    white.set(0x3FF * server.arg(0).toInt() / 10);
    status += "Manual Control";
  } else if (server.argName(0) == "chwifi") {
    println_dbg("Change WiFi SSID");
    target_ssid = "NULL";
    target_pass = "NULL";
    wifiBackupToFile();
    RESET();
  } else {
    status += "Listening";
  }
  status += "</p>";
  return status;
}

void handleRoot() {
  // Request detail
  println_dbg("");
  println_dbg("New Request");
  //println_dbg("URI: " + server.uri());
  //println_dbg("Method: " + (server.method() == HTTP_GET) ? "GET" : "POST");
  println_dbg("Arguments: " + String(server.args()));
  for (uint8_t i = 0; i < server.args(); i++) {
    println_dbg("  " + server.argName(i) + " = " + server.arg(i));
  }

  // Prepare the response
  String status = handleRequest();

  // Prepare the response
  String response = generateHtml(status);

  // Send the response
  server.send(200, "text/html", response);
  delay(100);
}

void serverTask() {
  server.handleClient();
}

void charEncode(String & s) {
  s.replace("+", " ");
  s.replace("%20", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%2A", "*");
  s.replace("%2B", "+");
  s.replace("%2C", ",");
  s.replace("%2D", "-");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%3A", ":");
  s.replace("%3B", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  s.replace("%7B", "{");
  s.replace("%7C", "|");
  s.replace("%7D", "}");
  s.replace("%7E", "~");
}

