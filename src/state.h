#ifndef APOLLON_FC_SRC_STATE_H
#define APOLLON_FC_SRC_STATE_H

/* Defines a struct for storing drone state information
 * such as target throttle, estimated angular velocity, etc
 *
 * Useful for passing around to PID controllers and other modules
 */

struct StateEstimate {
  float target;
  float estimate;
};

struct DroneState {
  StateEstimate yawSpeed;
  StateEstimate pitchSpeed;
  StateEstimate rollSpeed;
  StateEstimate throttle;
};

#endif /* APOLLON_FC_SRC_STATE_H */
