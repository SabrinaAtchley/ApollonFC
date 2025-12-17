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
#include "q16x16/q16x16.h"

class PilotControl {
private:
  // Takes a raw control value, accesses its LUT, and performs a LERP
  inline Q16x16 interpolate(const Q16x16* lut, const uint16_t v);

public:
  // Updates drone target state using receiver data
  void update(DroneState &state, RECEIVER_T &receiver);
};

#endif /* APOLLON_FC_SRC_PILOT_CONTROL_H */
