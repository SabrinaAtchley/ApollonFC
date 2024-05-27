#ifndef APOLLON_FC_UNIT_TESTS_H
#define APOLLON_FC_UNIT_TESTS_H

/* Handles Unit testing of all modules and other units
 * Run as a single suite of tests
 *
 */

#include "unit-tests/test-vector8.h"
#include "unit-tests/test-receivers.h"
#include "unit-tests/test-esc.h"
#include "unit-tests/test-sensors.h"
#include "unit-tests/test-array-queue.h"
#include "unit-tests/test-pid.h"
#include "unit-tests/test-pilot-control.h"

/* Flag(s) for which tests should be run (running all at once can run
 * into program memory issues on low-memory boards such as the Arduino Uno)
 * Options:
 *  APOLLON_FC_UNIT_TEST_VECTOR8`
 *  APOLLON_FC_UNIT_TEST_RECEIVERS
 *  APOLLON_FC_UNIT_TEST_ESC
 *  APOLLON_FC_UNIT_TEST_SENSORS
 *  APOLLON_FC_UNIT_TEST_ARRAY_QUEUE
 *  APOLLON_FC_UNIT_TEST_PID
 *  APOLLON_FC_UNIT_TEST_PILOT_CONTROL
 */
#define APOLLON_FC_UNIT_TEST_PILOT_CONTROL


void setup() {
  Serial.begin(9600);

  #ifdef APOLLON_FC_UNIT_TEST_VECTOR8
    unitVector8();
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
  #ifdef APOLLON_FC_UNIT_TEST_ARRAY_QUEUE
    unitArrayQueue();
  #endif
  #ifdef APOLLON_FC_UNIT_TEST_PID
    unitPID();
  #endif
  #ifdef APOLLON_FC_UNIT_TEST_PILOT_CONTROL
    unitPilotControl();
  #endif
}

void loop(){};

#endif
