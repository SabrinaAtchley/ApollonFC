#include "motor-controller.h"

MotorController::MotorController()
  : pidThrottle(PID_THROTTLE_COEFFICIENTS, PID_THROTTLE_SCALE, PID_THROTTLE_INTEGRAL_LIMITS),
    pidYaw(PID_YAW_COEFFICIENTS, PID_YAW_SCALE, PID_YAW_INTEGRAL_LIMITS),
    pidPitch(PID_PITCH_COEFFICIENTS, PID_PITCH_SCALE, PID_PITCH_INTEGRAL_LIMITS),
    pidRoll(PID_ROLL_COEFFICIENTS, PID_ROLL_SCALE, PID_ROLL_INTEGRAL_LIMITS)
{
  SpeedController::setup();

  // Automatic ESC calibration, recommended off
  #ifdef ESC_DO_CALIBRATION
  SPEED_CONTROLLER_CALIBRATE(signals);
  #endif

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
    signals[i] = vectorDotProduct(control, mixingMatrix[0], 4);
  }
}
