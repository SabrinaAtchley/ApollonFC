#include "pilot-control.h"

#define IS_SWITCH_DOWN(CHANNEL) (receiver.getChannel(CHANNEL) == INPUT_MOTOR_MAX)

#define _RESPONSE_CURVE_E1(K1, K2, C1) (K1 * K2 - sqrt(pow(K1 * K2, 2) - 4 * pow(C1, 2) * K1 * K2)) / (2 * C1 * K1)
#define RESPONSE_CURVE_E1(MIN, MID, MAX, C1) _RESPONSE_CURVE_E1((MIN - MID), (MAX - MID), C1)
#define RESPONSE_CURVE_E2(MIN, MID, MAX, C1) -_RESPONSE_CURVE_E1((MAX - MID), (MIN - MID), C1)
#define RESPONSE_CURVE_T(I) (((double) (2 * i)) / ((1 << PILOT_RESPONSE_CURVE_LUT_SIZE) - 1) - 1.0)
#define GENERATE_LUT_DATA(AXIS) MAKE_INIT_LIST(PILOT_RESPONSE_CURVE_LUT_SIZE, responseCurveLUTValue, CAT(AXIS, _MIN), CAT(AXIS, _MID), CAT(AXIS, _MAX), CAT(AXIS, _K))

static constexpr Q16x16 responseCurveLUTValue(
  const uint16_t i, // LUT index
  const double min, // minimum output value
  const double mid, // neutral output value
  const double max, // maximum output value
  const double k // neutral response sensitivity gain
) {
  return ftoq16x16(
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
static constexpr PROGMEM Q16x16 yawSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_YAWSPEED)};
static constexpr PROGMEM Q16x16 rollSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_ROLLSPEED)};
static constexpr PROGMEM Q16x16 pitchSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_PITCHSPEED)};
static constexpr PROGMEM Q16x16 rollAngleLUT[] = {GENERATE_LUT_DATA(PILOT_ROLLANGLE)};
static constexpr PROGMEM Q16x16 pitchAngleLUT[] = {GENERATE_LUT_DATA(PILOT_PITCHANGLE)};
static constexpr PROGMEM Q16x16 verticalSpeedLUT[] = {GENERATE_LUT_DATA(PILOT_VERTICALSPEED)};

inline Q16x16 PilotControl::interpolate(
    const Q16x16* lut,
    const uint16_t v
  ) {
  const uint16_t entries = (1 << PILOT_RESPONSE_CURVE_LUT_SIZE);
  // (v - MIN) / (MAX - MIN) * (entries - 1)
  const Q16x16 index = q16x16_mul_s(
    itoq16x16(entries - 1),
    q16x16_div_s(
      itoq16x16(v - INPUT_MOTOR_MIN),
      itoq16x16(INPUT_MOTOR_MAX - INPUT_MOTOR_MIN)
    )
  );
  const uint16_t i0 = index >> 16; // truncate
  const uint16_t i1 = min(i0 + 1, entries - 1);
  const Q16x16 frac = q16x16_sub_s(
    index,
    itoq16x16(i0)
  );


  // LERP between i0 and i1
  // i0 + frac * (i1 - i0)
  return q16x16_add_s( pgm_read_q16x16(&lut[i0]),
    q16x16_mul_s(frac, q16x16_sub_s(
      pgm_read_q16x16(&lut[i1]),
      pgm_read_q16x16(&lut[i0])
    ))
  );
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
      // TODO: This should likely take the raw PWM output as the "Base" throttle
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
