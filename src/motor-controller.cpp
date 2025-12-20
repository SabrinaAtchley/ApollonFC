#include "motor-controller.h"

MotorController::MotorController()
  : pidThrottle(PID_THROTTLE_COEFFICIENTS, PID_THROTTLE_SCALE, PID_THROTTLE_INTEGRAL_LIMITS, 0x00008000),
    pidYaw(PID_YAW_COEFFICIENTS, PID_YAW_SCALE, PID_YAW_INTEGRAL_LIMITS, 0x00008000),
    pidPitch(PID_PITCH_COEFFICIENTS, PID_PITCH_SCALE, PID_PITCH_INTEGRAL_LIMITS, 0x00008000),
    pidRoll(PID_ROLL_COEFFICIENTS, PID_ROLL_SCALE, PID_ROLL_INTEGRAL_LIMITS, 0x00008000)
{
  SpeedController::setup();

  // Automatic ESC calibration, recommended off
  #ifdef ESC_DO_CALIBRATION
  SPEED_CONTROLLER_CALIBRATE(signals);
  #endif

  // Initialize motor mixing matrix
  const float mixingMatrixf[MOTOR_AMOUNT][4] = MOTOR_MIXING_MATRIX;
  for (uint8_t i = 0; i < MOTOR_AMOUNT; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      mixingMatrix[i][j] = ftoq16x16(mixingMatrixf[i][j]);
    }
  }

  throttleDown(); // initialize signals to INPUT_MOTOR_MIN
}

// Set throttle to INPUT_MOTOR_MIN
inline void MotorController::throttleDown() {
  SPEED_CONTROLLER_WRITE_ALL(signals, INPUT_MOTOR_MIN);
}

// Send last stored motor signals to the controller
inline void MotorController::loop() {
  SpeedController::write(signals);
}

void MotorController::update(const DroneState &state, const unsigned long &deltaT) {
  control[0] = pidThrottle.update(state.throttle.target,   state.throttle.estimate,   deltaT);
  control[1] =     pidRoll.update(state.rollSpeed.target,  state.rollSpeed.estimate,  deltaT);
  control[2] =    pidPitch.update(state.pitchSpeed.target, state.pitchSpeed.estimate, deltaT);
  control[3] =      pidYaw.update(state.yawSpeed.target,   state.yawSpeed.estimate,   deltaT);

  // Multiply control vector by the motor mixing matrix to get the signal vector
  for (uint8_t i = 0; i < MOTOR_AMOUNT; i++) {
    // signals[i] = control * mixingMatrix[i] (dot product)
    signals[i] = q16x16_mul_s(control[0], mixingMatrix[i][0]);
    signals[i] = q16x16_add_s(signals[i], q16x16_mul_s(control[1], mixingMatrix[i][1]));
    signals[i] = q16x16_add_s(signals[i], q16x16_mul_s(control[2], mixingMatrix[i][2]));
    signals[i] = q16x16_add_s(signals[i], q16x16_mul_s(control[3], mixingMatrix[i][3]));
  }
}
