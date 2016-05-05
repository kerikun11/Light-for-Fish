#include "time_op.h"

#include <TimeLib.h>
#include <WiFiUdp.h>
#include "config.h"

// NTP Servers:
IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov

const int timeZone = 9;     // UTC +09:00
const char weekdayCharJap[8][4] = {"＊", "日", "月", "火", "水", "木", "金", "土"};

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void setupTime()
{
  print_dbg("IP number assigned by DHCP is ");
  println_dbg(WiFi.localIP());
  println_dbg("Starting UDP");
  Udp.begin(localPort);
  print_dbg("Local port: ");
  println_dbg(Udp.localPort());
  println_dbg("waiting for sync");
  setSyncProvider(getNtpTime);
}

void digitalClockDisplay() {
  // digital clock display of the time
  print_dbg(hour());
  printDigits(minute());
  printDigits(second());
  print_dbg(" ");
  print_dbg(day());
  print_dbg(".");
  print_dbg(month());
  print_dbg(".");
  print_dbg(year());
  println_dbg();
}

void printDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  print_dbg(":");
  if (digits < 10)
    print_dbg('0');
  print_dbg(digits);
}

String inTwoDigits(int n) {
  return String(n / 10, DEC) + String(n % 10, DEC);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  println_dbg("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      println_dbg("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  println_dbg("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

