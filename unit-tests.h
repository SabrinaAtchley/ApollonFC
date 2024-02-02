#ifndef FLIGHT_CONTROLLER_UNIT_TESTS_H
#define FLIGHT_CONTROLLER_UNIT_TESTS_H

/* Handles Unit testing of all modules and other units
 * Run as a single suite of tests
 *
 */

#include "unit-tests/test-vector16.h"


void setup() {
  Serial.begin(9600);

  unitVector16();
}

void loop(){};

#endif
