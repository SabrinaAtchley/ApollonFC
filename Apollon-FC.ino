#define TEST_MODE

#ifdef TEST_MODE
#include "unit-tests.h"
#else
#include "main.h"
#endif

void setup() {
  Serial.begin(9600);
  while(!Serial){};

  Serial.println("Starting up...");
  #ifdef TEST_MODE
  Tests::setup();
  #else
  Main::setup();
  #endif
  Serial.println("Finished start up.");
}

void loop() {
  #ifdef TEST_MODE
  Tests::loop();
  #else
  Main::loop();
  #endif
}
