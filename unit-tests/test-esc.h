#ifndef APOLLON_FC_UNIT_TESTS_TEST_ESC_H
#define APOLLON_FC_UNIT_TESTS_TEST_ESC_H

#include "../src/receivers.h"
#include "../src/esc.h"

void unitESC() {
  uint16_t signals[MOTOR_AMOUNT];
  SpeedController::setup();
  SPEED_CONTROLLER_CALIBRATE(signals);
  RECEIVER_T receiver;
  bool isArmed = false;

  #define IS_ARM_SWITCH_DOWN (receiver.getChannel(INPUT_SOFT_ARM_SWITCH) == INPUT_MOTOR_MAX)
  #define IS_THROTTLE_DOWN (receiver.getChannel(INPUT_CHANNEL_THROTTLE) == INPUT_MOTOR_MIN)

  // Throttle response test
  Serial.println("Begining main loop");
  while(true) {
    receiver.update();

    isArmed = (isArmed && IS_ARM_SWITCH_DOWN) || (IS_ARM_SWITCH_DOWN && IS_THROTTLE_DOWN);
    const uint16_t throttle = CLAMP(receiver.getChannel(INPUT_CHANNEL_THROTTLE), INPUT_IDLE_SIG, INPUT_MOTOR_MAX);
    SPEED_CONTROLLER_WRITE_ALL(
      signals,
      isArmed ? throttle : INPUT_MOTOR_MIN
    );

    Serial.println(receiver.getChannel(INPUT_CHANNEL_THROTTLE));
    SpeedController::write(signals);
  }
} /* unitEsc */

#endif
