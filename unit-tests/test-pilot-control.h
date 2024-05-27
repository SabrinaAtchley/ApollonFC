#ifndef APOLLON_FC_UNIT_TESTS_TEST_PILOT_CONTROL_H
#define APOLLON_FC_UNIT_TESTS_TEST_PILOT_CONTROL_H

#include "../src/pilot-control.h"
#include "../src/receivers.h"
#include "../src/state.h"

void unitPilotControl() {
  RECEIVER_T receiver;
  PilotControl pilot;
  DroneState state;

  while(true) {
    receiver.update();
    pilot.update(state, receiver);

    Serial.print("isArmed:");
    Serial.print((int) state.isArmed);
    Serial.print(",flightMode:");
    Serial.print(state.flightMode);
    Serial.print(",throttle:");
    Serial.print(state.throttle.target);
    Serial.print(",yawSpeed:");
    Serial.print(state.yawSpeed.target);
    Serial.print(",pitchSpeed:");
    Serial.print(state.pitchSpeed.target);
    Serial.print(",rollSpeed:");
    Serial.print(state.rollSpeed.target);
    Serial.print(",pitchAngle:");
    Serial.print(state.pitchAngle.target);
    Serial.print(",rollAngle:");
    Serial.println(state.rollAngle.target);
  }
} /* unitPilotControl */

#endif /* APOLLON_FC_UNIT_TESTS_TEST_PILOT_CONTROL_H */
