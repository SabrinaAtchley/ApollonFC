#ifndef APOLLON_FC_SRC_ESC_H
#define APOLLON_FC_SRC_ESC_H

#include "../configuration.h"
#include <Servo.h>
#include "macros.h"

/* Handles interfacing with all drone ESCs
 *
 * Chips with multiple ESCs, such as 4 in 1 ESCs, have the same number of signal
 * wires and so can be treated as just multiple ESCs
 */

namespace SpeedController {

  #define SPEED_CONTROLLER_WRITE_ALL(SIGNALS, S) for (uint8_t i = 0; i < MOTOR_AMOUNT; i++) {SIGNALS[i] = S;}; SpeedController::write(SIGNALS);
  #define SPEED_CONTROLLER_CALIBRATE(SIGNALS) SPEED_CONTROLLER_WRITE_ALL(SIGNALS, INPUT_MOTOR_MAX); delay(6000); SPEED_CONTROLLER_WRITE_ALL(SIGNALS, INPUT_MOTOR_MIN); delay(3000);

  extern Servo escs[MOTOR_AMOUNT];

  // Initialize ESCs
  void setup();
  // Send signal data to ESCs
  void write(const uint16_t *signals);

} /* SpeedController */

#endif
