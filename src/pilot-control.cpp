#include "pilot-control.h"

#define IS_SWITCH_DOWN(CHANNEL) (receiver.getChannel(CHANNEL) == INPUT_MOTOR_MAX)

#define _RESPONSE_CURVE_E1(K1, K2, C1) (K1 * K2 - sqrt(pow(K1 * K2, 2) - 4 * pow(C1, 2) * K1 * K2)) / (2 * C1 * K1)
#define RESPONSE_CURVE_E1(MIN, MID, MAX, C1) _RESPONSE_CURVE_E1((MIN - MID), (MAX - MID), C1)
#define RESPONSE_CURVE_E2(MIN, MID, MAX, C1) -_RESPONSE_CURVE_E1((MAX - MID), (MIN - MID), C1)
#define RESPONSE_CURVE_T(I) (((double) (2 * i)) / ((1 << PILOT_RESPONSE_CURVE_LUT_SIZE) - 1) - 1.0)
#define GENERATE_LUT_DATA(AXIS) MAKE_INIT_LIST(PILOT_RESPONSE_CURVE_LUT_SIZE, responseCurveLUTValue, CAT(AXIS, _MIN), CAT(AXIS, _MID), CAT(AXIS, _MAX), CAT(AXIS, _K))

static constexpr float responseCurveLUTValue(
  const uint16_t i, // LUT index
  const double min, // minimum output value
  const double mid, // neutral output value
  const double max, // maximum output value
  const double k // neutral response sensitivity gain
) {
  return (float) (
      exp(k) * (pow(
          RESPONSE_CURVE_E1(min, mid, max, exp(k)),
          RESPONSE_CURVE_T(i)
      ) - pow(
          RESPONSE_CURVE_E2(min, mid, max, exp(k)),
          -RESPONSE_CURVE_T(i)
      )) + mid
  );
}

// Response curve lookup tables
static constexpr PROGMEM float yawSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_YAWSPEED)};
static constexpr PROGMEM float rollSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_ROLLSPEED)};
static constexpr PROGMEM float pitchSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_PITCHSPEED)};
static constexpr PROGMEM float rollAngleLUT[] = {GENERATE_LUT_DATA(PILOT_ROLLANGLE)};
static constexpr PROGMEM float pitchAngleLUT[] = {GENERATE_LUT_DATA(PILOT_PITCHANGLE)};
static constexpr PROGMEM float verticalSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_VERTICALSPEED)};

inline float PilotControl::interpolate(
    const float* lut,
    const uint16_t v
  ) {
  const uint16_t entries = (1 << PILOT_RESPONSE_CURVE_LUT_SIZE);
  const float index = (
      ((uint32_t)(v - INPUT_MOTOR_MIN))
    * (entries - 1)
    / ((float) (INPUT_MOTOR_MAX - INPUT_MOTOR_MIN))
  );
  const uint16_t i0 = (uint16_t) index; // truncate
  const uint16_t i1 = min(i0 + 1, entries - 1);
  const float frac = index - i0;

  // LERP between i0 and i1
  return (pgm_read_float(&lut[i0]) + frac * (
    pgm_read_float(&lut[i1]) - pgm_read_float(&lut[i0])
  ));
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
        verticalSpeedLUT,
        receiver.getChannel(INPUT_CHANNEL_THROTTLE)
      );

      state.yawSpeed.target = interpolate(
        yawSpeedLUT,
        receiver.getChannel(INPUT_CHANNEL_YAW)
      );

      state.pitchAngle.target = interpolate(
        pitchAngleLUT,
        receiver.getChannel(INPUT_CHANNEL_PITCH)
      );

      state.rollAngle.target = interpolate(
        rollAngleLUT,
        receiver.getChannel(INPUT_CHANNEL_ROLL)
      );
      break;

    case FLIGHT_MODE_ACRO:
      state.throttle.target = interpolate(
        verticalSpeedLUT,
        receiver.getChannel(INPUT_CHANNEL_THROTTLE)
      );

      state.yawSpeed.target = interpolate(
        yawSpeedLUT,
        receiver.getChannel(INPUT_CHANNEL_YAW)
      );

      state.pitchSpeed.target = interpolate(
        pitchSpeedLUT,
        receiver.getChannel(INPUT_CHANNEL_PITCH)
      );

      state.rollSpeed.target = interpolate(
        rollSpeedLUT,
        receiver.getChannel(INPUT_CHANNEL_ROLL)
      );
      break;
  }

}
