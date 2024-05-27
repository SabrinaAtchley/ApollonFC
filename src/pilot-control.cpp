#include "pilot-control.h"

#define IS_SWITCH_DOWN(CHANNEL) (receiver.getChannel(CHANNEL) == INPUT_MOTOR_MAX)

#define _PILOT_INTERPOLATION_E1(K1, K2, C1) (K1 * K2 - sqrt(pow(K1 * K2, 2) - 4 * pow(C1, 2) * K1 * K2)) / (2 * C1 * K1)
#define PILOT_INTERPOLATION_E1(MIN, MID, MAX, C1) _PILOT_INTERPOLATION_E1((MIN - MID), (MAX - MID), C1)
#define PILOT_INTERPOLATION_E2(MIN, MID, MAX, C1) -_PILOT_INTERPOLATION_E1((MAX - MID), (MIN - MID), C1)


// Interpolation constants
constexpr double yawSpeed_c1      = exp(PILOT_YAWSPEED_K);
constexpr double yawSpeed_e1      = PILOT_INTERPOLATION_E1(PILOT_YAWSPEED_MIN,      PILOT_YAWSPEED_MID,      PILOT_YAWSPEED_MAX,      yawSpeed_c1);
constexpr double yawSpeed_e2      = PILOT_INTERPOLATION_E2(PILOT_YAWSPEED_MIN,      PILOT_YAWSPEED_MID,      PILOT_YAWSPEED_MAX,      yawSpeed_c1);

constexpr double rollSpeed_c1     = exp(PILOT_ROLLSPEED_K);
constexpr double rollSpeed_e1     = PILOT_INTERPOLATION_E1(PILOT_ROLLSPEED_MIN,     PILOT_ROLLSPEED_MID,     PILOT_ROLLSPEED_MAX,     rollSpeed_c1);
constexpr double rollSpeed_e2     = PILOT_INTERPOLATION_E2(PILOT_ROLLSPEED_MIN,     PILOT_ROLLSPEED_MID,     PILOT_ROLLSPEED_MAX,     rollSpeed_c1);

constexpr double pitchSpeed_c1    = exp(PILOT_PITCHSPEED_K);
constexpr double pitchSpeed_e1    = PILOT_INTERPOLATION_E1(PILOT_PITCHSPEED_MIN,    PILOT_PITCHSPEED_MID,    PILOT_PITCHSPEED_MAX,    pitchSpeed_c1);
constexpr double pitchSpeed_e2    = PILOT_INTERPOLATION_E2(PILOT_PITCHSPEED_MIN,    PILOT_PITCHSPEED_MID,    PILOT_PITCHSPEED_MAX,    pitchSpeed_c1);

constexpr double rollAngle_c1     = exp(PILOT_ROLLANGLE_K);
constexpr double rollAngle_e1     = PILOT_INTERPOLATION_E1(PILOT_ROLLANGLE_MIN,     PILOT_ROLLANGLE_MID,     PILOT_ROLLANGLE_MAX,     rollAngle_c1);
constexpr double rollAngle_e2     = PILOT_INTERPOLATION_E2(PILOT_ROLLANGLE_MIN,     PILOT_ROLLANGLE_MID,     PILOT_ROLLANGLE_MAX,     rollAngle_c1);

constexpr double pitchAngle_c1    = exp(PILOT_PITCHANGLE_K);
constexpr double pitchAngle_e1    = PILOT_INTERPOLATION_E1(PILOT_PITCHANGLE_MIN,    PILOT_PITCHANGLE_MID,    PILOT_PITCHANGLE_MAX,    pitchAngle_c1);
constexpr double pitchAngle_e2    = PILOT_INTERPOLATION_E2(PILOT_PITCHANGLE_MIN,    PILOT_PITCHANGLE_MID,    PILOT_PITCHANGLE_MAX,    pitchAngle_c1);

constexpr double verticalSpeed_c1 = exp(PILOT_VERTICALSPEED_K);
constexpr double verticalSpeed_e1 = PILOT_INTERPOLATION_E1(PILOT_VERTICALSPEED_MIN, PILOT_VERTICALSPEED_MID, PILOT_VERTICALSPEED_MAX, verticalSpeed_c1);
constexpr double verticalSpeed_e2 = PILOT_INTERPOLATION_E2(PILOT_VERTICALSPEED_MIN, PILOT_VERTICALSPEED_MID, PILOT_VERTICALSPEED_MAX, verticalSpeed_c1);


inline double PilotControl::interpolate(
  const double c1,
  const double e1,
  const double e2,
  const float mid,
  const uint16_t v
) const {
  const double t = RANGEMAP(
    v,
    INPUT_MOTOR_MIN, INPUT_MOTOR_MAX,
    (double) -1.0, (double) 1.0
  );

  return c1 * (pow(e1, t) - pow(e2, -t)) + mid;
}

void PilotControl::update(DroneState &state, RECEIVER_T &receiver) {
  /* Check if drone is armed
   * If drone is armed: isArmed = isArmSwitchDown
   * If drone is not armed: isArmed = isArmSwitchDown && isThrottleDown
   */

  state.isArmed =
        IS_SWITCH_DOWN(INPUT_SOFT_ARM_SWITCH)
    && (state.isArmed || receiver.getChannel(INPUT_CHANNEL_THROTTLE) == INPUT_MOTOR_MIN);

  if (!state.isArmed) {
    // Set all targets to 0 and return, drone is disarmed
    // (A second check is made in the motor controller before sending signals to ESCs)
    state.yawSpeed.target = 0;
    state.pitchSpeed.target = 0;
    state.rollSpeed.target = 0;
    state.throttle.target = 0;
    state.pitchAngle.target = 0;
    state.rollAngle.target = 0;
    return;
  }

  // Check for flight mode change
  if (IS_SWITCH_DOWN(INPUT_FLIGHT_MODE_SWITCH)) {
    state.flightMode = FLIGHT_MODE_SWITCH_MODE;
  } else {
    state.flightMode = FLIGHT_MODE_DEFAULT;
  }

  switch(state.flightMode) {
    case FLIGHT_MODE_ANGLE:
      state.throttle.target = interpolate(
        verticalSpeed_c1,
        verticalSpeed_e1,
        verticalSpeed_e2,
        PILOT_VERTICALSPEED_MID,
        receiver.getChannel(INPUT_CHANNEL_THROTTLE)
      );

      state.yawSpeed.target = interpolate(
        yawSpeed_c1,
        yawSpeed_e1,
        yawSpeed_e2,
        PILOT_YAWSPEED_MID,
        receiver.getChannel(INPUT_CHANNEL_YAW)
      );

      state.pitchAngle.target = interpolate(
        pitchAngle_c1,
        pitchAngle_e1,
        pitchAngle_e2,
        PILOT_PITCHANGLE_MID,
        receiver.getChannel(INPUT_CHANNEL_PITCH)
      );

      state.rollAngle.target = interpolate(
        rollAngle_c1,
        rollAngle_e1,
        rollAngle_e2,
        PILOT_ROLLANGLE_MID,
        receiver.getChannel(INPUT_CHANNEL_ROLL)
      );
      break;

    case FLIGHT_MODE_ACRO:
      state.throttle.target = interpolate(
        verticalSpeed_c1,
        verticalSpeed_e1,
        verticalSpeed_e2,
        PILOT_VERTICALSPEED_MID,
        receiver.getChannel(INPUT_CHANNEL_THROTTLE)
      );

      state.yawSpeed.target = interpolate(
        yawSpeed_c1,
        yawSpeed_e1,
        yawSpeed_e2,
        PILOT_YAWSPEED_MID,
        receiver.getChannel(INPUT_CHANNEL_YAW)
      );

      state.pitchSpeed.target = interpolate(
        pitchSpeed_c1,
        pitchSpeed_e1,
        pitchSpeed_e2,
        PILOT_PITCHSPEED_MID,
        receiver.getChannel(INPUT_CHANNEL_PITCH)
      );

      state.rollSpeed.target = interpolate(
        rollSpeed_c1,
        rollSpeed_e1,
        rollSpeed_e2,
        PILOT_ROLLSPEED_MID,
        receiver.getChannel(INPUT_CHANNEL_ROLL)
      );
      break;
  }

}
