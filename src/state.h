#ifndef APOLLON_FC_SRC_STATE_H
#define APOLLON_FC_SRC_STATE_H

/* Defines a struct for storing drone state information
 * such as target throttle, estimated angular velocity, etc
 *
 * Useful for passing around to PID controllers and other modules
 */

#include "../configuration.h"

struct StateEstimate {
  float target = 0.0;
  float estimate = 0.0;
};

struct DroneState {
  StateEstimate yawSpeed;
  StateEstimate pitchSpeed;
  StateEstimate rollSpeed;
  StateEstimate throttle;
  StateEstimate pitchAngle;
  StateEstimate rollAngle;
  byte flightMode = FLIGHT_MODE_DEFAULT;
  bool isArmed = false;
};

#endif /* APOLLON_FC_SRC_STATE_H */
