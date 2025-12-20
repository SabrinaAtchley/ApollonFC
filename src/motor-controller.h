#ifndef APOLLON_FC_SRC_MOTOR_CONTROLLER_H
#define APOLLON_FC_SRC_MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "../configuration.h"
#include "esc.h"
#include "macros.h"
#include "pid.h"
#include "state.h"
#include "vector8.h"
#include "q16x16/q16x16.h"

/* Motor mixing module
 *
 * Updates PIDs and takes outputs to send it through the control matrix equation
 * Filters signal to remove high frequency noise
 * Sends signals into motor ESCs
 */

class MotorController {
private:
  uint16_t signals[MOTOR_AMOUNT];
  PID pidThrottle;
  PID pidYaw;
  PID pidPitch;
  PID pidRoll;
  Q16x16 control[4]; // yaw, pitch, roll, throttle
  Q16x16 mixingMatrix[MOTOR_AMOUNT][4] = MOTOR_MIXING_MATRIX;

public:
  MotorController();

  void throttleDown();
  void loop(); // Sends last stored motor signals to the controller
  // Updates motor signals with new PID data
  void update(const DroneState &state, const unsigned long &deltaT);
};

#endif
