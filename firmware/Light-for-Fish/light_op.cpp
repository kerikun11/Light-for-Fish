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

