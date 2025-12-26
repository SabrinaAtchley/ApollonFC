#ifndef APOLLON_FC_UNIT_TESTS_TEST_ESC_H
#define APOLLON_FC_UNIT_TESTS_TEST_ESC_H

#include "../src/receivers.h"
#include "../src/esc.h"

void unitESC() {
  RECEIVER_T receiver;
  Serial.print("Setting up ESCs... ");
  SpeedController::setup();
  Serial.println("Done.");
  uint16_t signals[MOTOR_AMOUNT];
  bool isArmed = false;

  #define IS_ARM_SWITCH_DOWN (receiver.getChannel(INPUT_CHANNEL_SWA) == INPUT_MOTOR_MAX)
  #define IS_THROTTLE_DOWN (receiver.getChannel(INPUT_CHANNEL_THROTTLE) == INPUT_MOTOR_MIN)

  Serial.print("Calibrating... ");
  SPEED_CONTROLLER_CALIBRATE(signals);
  Serial.println("Done.");

  // Throttle response test
  Serial.println("Begining main loop");
  while(true) {
    receiver.update();

    isArmed = (isArmed && IS_ARM_SWITCH_DOWN) || (IS_ARM_SWITCH_DOWN && IS_THROTTLE_DOWN);
    SPEED_CONTROLLER_WRITE_ALL(
      signals,
      isArmed ? receiver.getChannel(INPUT_CHANNEL_THROTTLE) : INPUT_MOTOR_MIN
    );

    Serial.println(receiver.getChannel(INPUT_CHANNEL_THROTTLE));
    SpeedController::write(signals);
  }
} /* unitEsc */

#endif
