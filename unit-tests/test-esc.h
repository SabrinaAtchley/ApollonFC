#ifndef APOLLON_FC_UNIT_TESTS_TEST_ESC_H
#define APOLLON_FC_UNIT_TESTS_TEST_ESC_H

#include "../src/receivers.h"
#include "../src/esc.h"

void unitESC() {
  RECEIVER_T receiver;
  SpeedController::setup();
  uint16_t channels[INPUT_CHANNELS];
  uint16_t signals[MOTOR_AMOUNT];
  bool isArmed = false;

  #define IS_ARM_SWITCH_DOWN (channels[INPUT_CHANNEL_SWA - 1] == INPUT_MOTOR_MAX)
  #define IS_THROTTLE_DOWN (channels[INPUT_CHANNEL_THROTTLE - 1] == INPUT_MOTOR_MIN)

  while(true) {
    receiver.getControls(channels);
    for (uint8_t i = 0; i < MOTOR_AMOUNT; i++) {
      isArmed = (isArmed && IS_ARM_SWITCH_DOWN) || (IS_ARM_SWITCH_DOWN && IS_THROTTLE_DOWN);

      if (isArmed)
        signals[i] = channels[INPUT_CHANNEL_THROTTLE - 1];
      else
        signals[i] = 1000;
    }
    Serial.println(channels[INPUT_CHANNEL_THROTTLE - 1]);
    SpeedController::write(signals);
  }
} /* unitEsc */

#endif
