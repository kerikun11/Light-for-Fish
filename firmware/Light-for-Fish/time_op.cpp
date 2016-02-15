#include "time_op.h"

#define LOCAL_PORT          2390              // local port to listen for UDP packets
#define NTP_SERVER_NAME     "time.nist.gov"
#define NTP_PACKET_SIZE     48                // NTP time stamp is in the first 48 bytes of the message

uint32_t epoch;
bool timeSyncMode;

/* Don't hardwire the IP address or we won't get the benefits of the pool.
    Lookup the IP address for the host name instead */
IPAddress timeServerIP; // time.nist.gov NTP server address

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
unsigned long sendNTPpacket(IPAddress& address);

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
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
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

uint32_t getTimeNTP(void) {
  println_dbg("Starting UDP");
  udp.begin(LOCAL_PORT);
  print_dbg("Local port: ");
  println_dbg(udp.localPort());

  //get a random server from the pool
  WiFi.hostByName(NTP_SERVER_NAME, timeServerIP);

  // send an NTP packet to a time server
  sendNTPpacket(timeServerIP);
  // wait to see if a reply is available

  int cb = 0;
  int timeout = 0;
  while (cb < 48) {
    delay(10);
    cb = udp.parsePacket();
    timeout++;
    if (timeout > 50) {
      println_dbg("no packet yet");
      return 0;
    }
  }
  print_dbg("packet received, length = ");
  println_dbg(cb);
  // We've received a packet, read the data from it
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:

  uint32_t highWord = word(packetBuffer[40], packetBuffer[41]);
  uint32_t lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  uint32_t secsSince1900 = highWord << 16 | lowWord;

  // now convert NTP time into everyday time:
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const uint32_t seventyYears = 2208988800UL;
  // subtract seventy years:
  uint32_t ep = secsSince1900 - seventyYears;
  print_dbg("UNIX Time = ");
  println_dbg(ep);
  ep += HOUR * 9;
  return ep;
}

void timeTask(void) {
  uint32_t ms;
  static uint32_t pre_ms;
  static uint16_t timeUpdate;

  ms = millis();
  if (ms > pre_ms + 1000) {
    pre_ms += 1000;
    epoch++;
    //println_dbg("epoch = " + String(epoch));
    //println_dbg("Time = " + String((epoch % DAY) / HOUR) + ":" + String((epoch % HOUR) / MINUTE) + "-" + String(epoch % MINUTE));
    timeUpdate++;
    if (timeUpdate > HOUR) {
      timeUpdate = 0;
      epoch = getTimeNTP();
    }
    if (timeSyncMode == true) {
      timeSync();
    }
  }
}

void timeSync(void) {
  white.set(0);
  uint16_t s = (uint32_t) (epoch % DAY) / (DAY / 4096);
  if (s < 1024) {
    red.set(0);
    green.set(0);
    blue.set(0);
  } else if (s < 2048) {
    for (uint8_t i = 0; i < 3; i++) {
      red.set(s - 1024);
      green.set(s - 1024);
      blue.set(s - 1024);
    }
  } else if (s < 3072) {
    red.set(3072 - s);
    green.set(3072 - s);
    blue.set(3072 - s);
  } else {
    red.set(0);
    green.set(0);
    blue.set(0);
  }
}

