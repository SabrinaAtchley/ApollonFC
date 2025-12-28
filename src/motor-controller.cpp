#include "motor-controller.h"

const Q16x16 pidAlpha = 0x00008000;

// Helpers for converting arg list macros into Q16x16 numbers
void threeArgListToQ16x16(
    const Q16x16 a, const Q16x16 b, const Q16x16 c,
    Q16x16 &arg1, Q16x16 &arg2, Q16x16 &arg3
) {
  arg1 = ftoq16x16(a);
  arg2 = ftoq16x16(b);
  arg3 = ftoq16x16(c);
}

void twoArgListToQ16x16(
    const Q16x16 a, const Q16x16 b,
    Q16x16 &arg1, Q16x16 &arg2
) {
  arg1 = ftoq16x16(a);
  arg2 = ftoq16x16(b);
}

MotorController::MotorController() {
  Q16x16 pidRollP, pidRollI, pidRollD;
  Q16x16 pidRollScaleP, pidRollScaleI, pidRollScaleD;
  Q16x16 pidRollIntegralMin, pidRollIntegralMax;
  threeArgListToQ16x16(PID_ROLL_COEFFICIENTS, pidRollP, pidRollI, pidRollD);
  threeArgListToQ16x16(PID_ROLL_SCALE, pidRollScaleP, pidRollScaleI, pidRollScaleD);
  twoArgListToQ16x16(PID_ROLL_INTEGRAL_LIMITS, pidRollIntegralMin, pidRollIntegralMax);

  Q16x16 pidPitchP, pidPitchI, pidPitchD;
  Q16x16 pidPitchScaleP, pidPitchScaleI, pidPitchScaleD;
  Q16x16 pidPitchIntegralMin, pidPitchIntegralMax;
  threeArgListToQ16x16(PID_ROLL_COEFFICIENTS, pidPitchP, pidPitchI, pidPitchD);
  threeArgListToQ16x16(PID_ROLL_SCALE, pidPitchScaleP, pidPitchScaleI, pidPitchScaleD);
  twoArgListToQ16x16(PID_ROLL_INTEGRAL_LIMITS, pidPitchIntegralMin, pidPitchIntegralMax);

  Q16x16 pidYawP, pidYawI, pidYawD;
  Q16x16 pidYawScaleP, pidYawScaleI, pidYawScaleD;
  Q16x16 pidYawIntegralMin, pidYawIntegralMax;
  threeArgListToQ16x16(PID_ROLL_COEFFICIENTS, pidYawP, pidYawI, pidYawD);
  threeArgListToQ16x16(PID_ROLL_SCALE, pidYawScaleP, pidYawScaleI, pidYawScaleD);
  twoArgListToQ16x16(PID_ROLL_INTEGRAL_LIMITS, pidYawIntegralMin, pidYawIntegralMax);

  static PID pidRollObj(
    pidRollP, pidRollI, pidRollD,
    pidRollScaleP, pidRollScaleI, pidRollScaleD,
    pidRollIntegralMin, pidRollIntegralMax,
    pidAlpha
  );
  pidRoll = &pidRollObj;
  static PID pidPitchObj(
    pidPitchP, pidPitchI, pidPitchD,
    pidPitchScaleP, pidPitchScaleI, pidPitchScaleD,
    pidPitchIntegralMin, pidPitchIntegralMax,
    pidAlpha
  );
  pidPitch = &pidPitchObj;
  static PID pidYawObj(
    pidYawP, pidYawI, pidYawD,
    pidYawScaleP, pidYawScaleI, pidYawScaleD,
    pidYawIntegralMin, pidYawIntegralMax,
    pidAlpha
  );
  pidYaw = &pidYawObj;

  // Initialize ESCs

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
void MotorController::loop() {
  SpeedController::write(signals);
}

void MotorController::update(const DroneState &state, const unsigned long &deltaT) {
  // Convert Q16x16 to integer part (truncate fractional bits)
  control[0] =    state.throttle.target >> 16;
  control[1] =     pidRoll->update(state.rollSpeed.target,  state.rollSpeed.estimate,  deltaT) >> 16;
  control[2] =    pidPitch->update(state.pitchSpeed.target, state.pitchSpeed.estimate, deltaT) >> 16;
  control[3] =      pidYaw->update(state.yawSpeed.target,   state.yawSpeed.estimate,   deltaT) >> 16;

  // Serial.print(state.rollSpeed.target);
  // Serial.print(",");
  // Serial.print(state.rollSpeed.estimate);
  // Serial.print(",");
  // Serial.println(q16x16_sub_s(state.rollSpeed.target, state.rollSpeed.estimate));

  // Serial.print(pidRoll->getLastPTerm());
  // Serial.print(",");
  // Serial.print(pidRoll->getLastITerm());
  // Serial.print(",");
  // Serial.println(pidRoll->getLastDTerm());

  // Serial.print(control[0]);
  // Serial.print(",");
  // Serial.print(control[1]);
  // Serial.print(",");
  // Serial.print(control[2]);
  // Serial.print(",");
  // Serial.println(control[3]);

  // Multiply control vector by the motor mixing matrix to get the signal vector
  for (uint8_t i = 0; i < MOTOR_AMOUNT; i++) {
    // signals[i] = control * mixingMatrix[i] (dot product)
    signals[i] = q16x16_mul_s(control[0], mixingMatrix[i][0]);
    signals[i] = q16x16_add_s(signals[i], q16x16_mul_s(control[1], mixingMatrix[i][1]));
    signals[i] = q16x16_add_s(signals[i], q16x16_mul_s(control[2], mixingMatrix[i][2]));
    signals[i] = q16x16_add_s(signals[i], q16x16_mul_s(control[3], mixingMatrix[i][3]));
    signals[i] = CLAMP(signals[i], INPUT_IDLE_SIG, INPUT_MOTOR_MAX);
  }
}
