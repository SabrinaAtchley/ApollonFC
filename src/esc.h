#ifndef FLIGHT_CONTROLLER_SRC_ESC_H
#define FLIGHT_CONTROLLER_SRC_ESC_H

#include "../configuration.h"
#include <Servo.h>

/* Handles interfacing with all drone ESCs
 *
 * Chips with multiple ESCs, such as 4 in 1 ESCs, have the same number of signal
 * wires and so can be treated as just multiple ESCs
 */

class SpeedController {
  Servo escs[MOTOR_AMOUNT];

public:
  SpeedController() {
    uint8_t i = 0;
    // Initialize ESCs
    for (const uint8_t pin : ESC_PINS) {
      escs[i].attach(pin, INPUT_MOTOR_MIN, INPUT_MOTOR_MAX);
      i++;
    }
  }

  void write(const double *signals) {
    for (const uint8_t i = 0; i < MOTOR_AMOUNT; i++) {
      escs[i].writeMicroseconds(signals[i]);
    }
  }
}

#endif
