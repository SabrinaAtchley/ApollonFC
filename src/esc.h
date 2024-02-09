#ifndef FLIGHT_CONTROLLER_SRC_ESC_H
#define FLIGHT_CONTROLLER_SRC_ESC_H

#include "../configuration.h"
#include <Servo.h>
#include "macros.h"

/* Handles interfacing with all drone ESCs
 *
 * Chips with multiple ESCs, such as 4 in 1 ESCs, have the same number of signal
 * wires and so can be treated as just multiple ESCs
 */

namespace SpeedController {

  Servo escs[MOTOR_AMOUNT];
  // Initialize ESCs
  uint8_t i = 0;

  void setup() {
    #define SRC_ESC_SPEEDCONTROLLER_ESC_INIT(P) escs[i++].attach(P, INPUT_MOTOR_MIN, INPUT_MOTOR_MAX)
    MAP(SRC_ESC_SPEEDCONTROLLER_ESC_INIT, ESC_PINS);
  }

  void write(const uint16_t *signals) {
    for (uint8_t i = 0; i < MOTOR_AMOUNT; i++)
      escs[i].writeMicroseconds(signals[i]);
  }

} /* SpeedController */

#endif
