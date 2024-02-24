#ifndef APOLLON_FC_UNIT_TESTS_H
#define APOLLON_FC_UNIT_TESTS_H

/* Handles Unit testing of all modules and other units
 * Run as a single suite of tests
 *
 */

#include "unit-tests/test-vector16.h"
#include "unit-tests/test-receivers.h"
#include "unit-tests/test-esc.h"
#include "unit-tests/test-sensors.h"

/* Flag(s) for which tests should be run (running all at once can run
 * into program memory issues on low-memory boards such as the Arduino Uno)
 * Options:
 *  APOLLON_FC_UNIT_TEST_VECTOR16
 *  APOLLON_FC_UNIT_TEST_RECEIVERS
 *  APOLLON_FC_UNIT_TEST_ESC
 *  APOLLON_FC_UNIT_TEST_SENSORS
 */
#define APOLLON_FC_UNIT_TEST_SENSORS


void setup() {
  Serial.begin(9600);

  #ifdef APOLLON_FC_UNIT_TEST_VECTOR16
    unitVector16();
  #endif
  #ifdef APOLLON_FC_UNIT_TEST_RECEIVERS
    unitReceivers();
  #endif
  #ifdef APOLLON_FC_UNIT_TEST_ESC
    unitESC();
  #endif
  #ifdef APOLLON_FC_UNIT_TEST_SENSORS
    unitSensors();
  #endif
}

void loop(){};

#endif
