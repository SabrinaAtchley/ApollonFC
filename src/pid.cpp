#include "pid.h"

PID::PID(
  const float _kp, const float _ki, const float _kd,
  const float _pScale, const float _iScale, const float _dScale,
  const float _iMin, const float _iMax
) : kp(_kp), ki(_ki), kd(_kd),
    pScale(_pScale), iScale(_iScale), dScale(_dScale),
    iMin(_iMin / _iScale), iMax(_iMax / _iScale) {}

void PID::setCoefficients(
  const float newKp, const float newKi, const float newKd
) {
  kp = newKp;
  ki = newKi;
  kd = newKd;
}

void PID::setIntegralBounds(const float newIMin, const float newIMax) {
  iMin = newIMin / iScale;
  iMax = newIMax / iScale;
}

const float PID::update(const float error, const unsigned long deltaT) {
  proportional = error;

  /* Integral
   * For simplicity and to save clock cycles, we will use a trapezoidal method for now
   * Consider revisiting with a fourth-order Runge-Kutta if more accuracy is needed
   */
  integral += (previousError + error) * deltaT / 2;
  integral = CLAMP(integral, iMin, iMax);


  /* Derivative
   * For now, we will just do a simple 2 term derivative (slope of a line).
   * Maybe revisit this with a polynomial interpolation later
   */

  // Since the difference in error between timesteps is so small, a large scaling factor
  // is needed here to make the derivative term significant
  derivative = (error - previousError) / deltaT;
  previousError = error;

  return CLAMP(
      (kp * pScale * proportional)
    + (ki * iScale * integral)
    + (kd * dScale * derivative),
    PID_OUTPUT_RANGE_MIN,
    PID_OUTPUT_RANGE_MAX
  );
}

const float PID::getLastPTerm() const {
  return kp * pScale * proportional;
}

const float PID::getLastITerm() const {
  return ki * iScale * integral;
}

const float PID::getLastDTerm() const {
  return kd * dScale * derivative;
}
