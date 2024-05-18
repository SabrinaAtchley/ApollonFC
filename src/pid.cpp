#include "pid.h"

PID::PID(
  const float _kp, const float _ki, const float _kd,
  const float _pScale, const float _iScale, const float _dScale,
  const float _iMin, const float _iMax
) : kp(_kp), ki(_ki), kd(_kd),
    pScale(_pScale), iScale(_iScale), dScale(_dScale),
    iMin(_iMin), iMax(_iMax) {}

void PID::setCoefficients(
  const float newKp, const float newKi, const float newKd
) {
  kp = newKp;
  ki = newKi;
  kd = newKd;
}

void PID::setIntegralBounds(const float newIMin, const float newIMax) {
  iMin = newIMin;
  iMax = newIMax;
}

const float PID::update(const float setPoint, const float value, const unsigned long deltaT) {
  const float error = setPoint - value;
  proportional = kp * pScale * error;

  /* Integral
   * For simplicity and to save clock cycles, we will use a trapezoidal method for now
   * Consider revisiting with a fourth-order Runge-Kutta if more accuracy is needed
   */
  integralSum += (previousError + error) * deltaT / 2;
  integralSum = CLAMP(integralSum, iMin / ki / iScale, iMax / ki / iScale);
  integral = ki * iScale * integralSum;

  /* Derivative
   * Uses Brown's Linear Exponential Smoothing to estimate the derivative
   * Estimates the derivative of the process variable instead of the derivative
   * of the error, to help combat derivative kick
   */
  if (s1 == 0.0 && s2 == 0.0) {
    s1 = value;
    s2 = value;
    derivative = 0.0;
  }  else {
    s1 = alpha * value + (1 - alpha) * s1;
    s2 = alpha * s1 + (1 - alpha) * s2;
    derivative = -kd * dScale * (s1 - s2) / (1 - alpha) * alpha;
  }

  previousError = error;
  return CLAMP(
    proportional + integral + derivative,
    PID_OUTPUT_RANGE_MIN,
    PID_OUTPUT_RANGE_MAX
  );
}

const float PID::getLastPTerm() const {
  return proportional;
}

const float PID::getLastITerm() const {
  return integral;
}

const float PID::getLastDTerm() const {
  return derivative;
}
