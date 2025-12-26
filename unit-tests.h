#ifndef APOLLON_FC_UNIT_TESTS_H
#define APOLLON_FC_UNIT_TESTS_H

/* Handles Unit testing of all modules and other units
 * Runs as a single unit at a time to save progmem
 */

#include "unit-tests/test-vector8.h"
#include "unit-tests/test-receivers.h"
#include "unit-tests/test-esc.h"
#include "unit-tests/test-sensors.h"
#include "unit-tests/test-array-queue.h"
#include "unit-tests/test-pid.h"
#include "unit-tests/test-pilot-control.h"
#include "unit-tests/test-q16x16.h"
#include "unit-tests/test-quaternion.h"
#include "unit-tests/test-madgwick.h"

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
 *  APOLLON_FC_UNIT_TEST_Q16X16
 *  APOLLON_FC_UNIT_TEST_QUATERNION
 *  APOLLON_FC_UNIT_TEST_MADGWICK
 */
#define APOLLON_FC_UNIT_TEST_ESC


namespace Tests {
  void setup() {
    Serial.println("Beginning unit tests");

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
    #ifdef APOLLON_FC_UNIT_TEST_Q16X16
      unitQ16x16();
    #endif
    #ifdef APOLLON_FC_UNIT_TEST_QUATERNION
      unitQuaternion();
    #endif
    #ifdef APOLLON_FC_UNIT_TEST_MADGWICK
      unitMadgwick();
    #endif
  }
  void loop(){};
}

#endif
