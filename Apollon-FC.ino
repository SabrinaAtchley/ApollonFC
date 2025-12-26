// #define TEST_MODE

#ifdef TEST_MODE
#include "unit-tests.h"
#else
#include "main.h"
#endif

void setup() {
  #ifdef TEST_MODE
  Tests::setup();
  #else
  Main::setup();
  #endif
}

void loop() {
  #ifdef TEST_MODE
  Tests::loop();
  #else
  Main::loop();
  #endif
}
