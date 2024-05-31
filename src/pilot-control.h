#ifndef APOLLON_FC_SRC_PILOT_CONTROL_H
#define APOLLON_FC_SRC_PILOT_CONTROL_H

/* Converts signals from the rf receiver into the target drone state
 *
 * Uses:
 *  Radio receiver data
 *  Drone mode setting
 *  Max/min acceleration and angle settings
 *
 * Outputs:
 *  Target output state
 *  isArmed :: bool
 */

#include <Arduino.h>
#include "macros.h"
#include "receivers.h"
#include "../configuration.h"
#include "state.h"

class PilotControl {
private:
  static inline double interpolate(
    const double c1,
    const double e1,
    const double e2,
    const float mid,
    const uint16_t v
  );

public:
  // Updates drone target state using receiver data
  static void update(DroneState &state, RECEIVER_T &receiver);
};

#endif /* APOLLON_FC_SRC_PILOT_CONTROL_H */
