#ifndef LIGHT_OP
#define LIGHT_OP

#include <ESP8266WiFi.h>
#include "config.h"

class light {
  public:
    uint16_t pulseWidth;
    light(int pin_number) {
      pin = pin_number;
      pulseWidth = 0;
    }

    void set(uint16_t value);

  private:
    int pin;
};

extern light red;
extern light green;
extern light blue;
extern light white;

#endif

