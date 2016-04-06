#include "light_op.h"

light red(PIN_RED);
light green(PIN_GREEN);
light blue(PIN_BLUE);
light white(PIN_WHITE);

void light::set(uint16_t value) {
  pulseWidth = value;
  value = (uint32_t)value * 3 / 4;
  value = (uint32_t)value * value / PWMRANGE;
  analogWrite(pin, value);
}

bool timeSyncMode = true;

void timeSync() {
  uint16_t s = (uint32_t)4 * (PWMRANGE + 1) * (hour() * 3600 + minute() * 60 + second()) / (24 * 60 * 60);
  uint16_t setValue;
  if (s < (PWMRANGE + 1)) {
    setValue = 0;
  } else if (s < 2 * (PWMRANGE + 1)) {
    setValue = s - (PWMRANGE + 1);
  } else if (s < 3 * (PWMRANGE + 1)) {
    setValue = 3 * (PWMRANGE + 1) - (s + 1);
  } else {
    setValue = 0;
  }
  red.set(setValue);
  green.set(setValue);
  blue.set(setValue);
  white.set(setValue);
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

