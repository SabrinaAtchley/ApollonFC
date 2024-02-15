#ifndef APOLLON_FC_SRC_MOTOR_CONTROLLER_H
#define APOLLON_FC_SRC_MOTOR_CONTROLLER_H

#include "../configuration.h"
#include "esc.h"

/* Handles sending PID process control vector into individual motors
 *
 * Takes PID vector output and sends it through the control matrix equation
 * Filters signal to remove high frequency noise
 * Sends signals into motor ESCs
 */

class MotorController {
  SpeedController sc();


}

#endif
