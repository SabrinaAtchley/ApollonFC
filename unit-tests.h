#ifndef FLIGHT_CONTROLLER_UNIT_TESTS_H
#define FLIGHT_CONTROLLER_UNIT_TESTS_H

/* Handles Unit testing of all modules and other units
 * Run as a single suite of tests
 *
 */

#include "unit-tests/test-vector16.h"

/* Flag(s) for which tests should be run (running all at once can run
 * into program memory issues on low-memory boards such as the Arduino Uno)
 * Options:
 *  FLIGHT_CONTROLLER_UNIT_TEST_VECTOR16
 *  FLIGHT_CONTROLLER_UNIT_TEST_RECEIVERS
 */
#define FLIGHT_CONTROLLER_UNIT_TEST_VECTOR16


void setup() {
  Serial.begin(9600);

  #ifdef FLIGHT_CONTROLLER_UNIT_TEST_VECTOR16
    unitVector16();
  #endif
  #ifdef FLIGHT_CONTROLLER_UNIT_TEST_RECEIVERS
    unitReceivers();
  #endif
}

void loop(){};

#endif
