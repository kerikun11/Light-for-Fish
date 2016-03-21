#include "light_op.h"

light red(PIN_RED);
light green(PIN_GREEN);
light blue(PIN_BLUE);
light white(PIN_WHITE);

void light::set(uint16_t value) {
  pulseWidth = value;
  value = (uint32_t)value * value * 4 / PWMRANGE / 5;
  analogWrite(pin, value);
}

bool timeSyncMode = true;

void timeSync() {
  white.set(0);
  uint32_t s = (uint32_t) hour() * 3600 + minute() * 60 + second();
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
    red.set(3072 - (s + 1));
    green.set(3072 - (s + 1));
    blue.set(3072 - (s + 1));
  } else {
    red.set(0);
    green.set(0);
    blue.set(0);
  }
}

void lightTask() {
  if (timeSyncMode == true) {
    static int prev;
    if (second() != prev) {
      prev = second();
      timeSync();
    }
  }
}

