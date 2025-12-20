#include "pid.h"

PID::PID(
  const Q16x16 _kp, const Q16x16 _ki, const Q16x16 _kd,
  const Q16x16 _pScale, const Q16x16 _iScale, const Q16x16 _dScale,
  const Q16x16 _iMin, const Q16x16 _iMax, const Q16x16 _alpha
) : kp(_kp), ki(_ki), kd(_kd),
    pScale(_pScale), iScale(_iScale), dScale(_dScale),
    iMin(_iMin), iMax(_iMax),
    alpha(_alpha), beta(q16x16_sub_s(itoq16x16(1), _alpha)),
    pGain(q16x16_mul_s(_kp, _pScale)), // pGain = kp * pScale
    iGain(q16x16_mul_s(_ki, _iScale)),
    dGain(q16x16_mul_s( // -kd * dScale * (alpha / beta)
      q16x16_mul_s(-_kd, _dScale),
      q16x16_div_s(_alpha, beta)
    )),
    outputMin(ftoq16x16(PID_OUTPUT_RANGE_MIN)),
    outputMax(ftoq16x16(PID_OUTPUT_RANGE_MAX)) {}

const Q16x16 PID::getLastPTerm() const {
  return proportional;
}

const Q16x16 PID::getLastITerm() const {
  return integral;
}

const Q16x16 PID::getLastDTerm() const {
  return derivative;
}

Q16x16 PID::update(const Q16x16 setPoint, const Q16x16 value, const Q16x16 deltaT) {
  const Q16x16 error = q16x16_sub_s(setPoint, value);

  // Serial.print("error:");
  // Serial.print(error);
  // Serial.print(", pGain:");
  // Serial.print(pGain);
  // Serial.print(", p:");

  proportional = q16x16_mul_s(error, pGain);
  // Serial.println(q16x16tof(proportional));

  /* Integral
   * For simplicity and to save clock cycles, we will use a trapezoidal method for now
   * Consider revisiting with a fourth-order Runge-Kutta if more accuracy is needed
   */
  // NOTE: It could be worth rewriting this using 64-bit math to minimize the deadband
  //       near error = 0
  // integral += iGain * (previousError + error) * deltaT / 2
  integral = q16x16_add_s(integral,
    q16x16_mul_s(iGain,
      q16x16_mul_s(
        q16x16_add_s(previousError, error),
        deltaT
      )
    ) >> 1
  );
  integral = CLAMP(integral, iMin, iMax);

  /* Derivative
   * Uses Brown's Linear Exponential Smoothing to estimate the derivative
   * Estimates the derivative of the process variable instead of the derivative
   * of the error, to help combat derivative kick
   */

   if (!dInit) {
     dInit = true;
     s1 = value;
     s2 = value;
     derivative = 0;
   } else {
     // alpha * value + beta * s1
     s1 = q16x16_add_s(
       q16x16_mul_s(alpha, value),
       q16x16_mul_s(beta, s1)
     );
     // alpha * s1 + beta * s2
     s2 = q16x16_add_s(
       q16x16_mul_s(alpha, s1),
       q16x16_mul_s(beta, s2)
     );
     // dGain * (s1 - s2)
     derivative = q16x16_mul_s(dGain,
       q16x16_sub_s(s1, s2)
     );
   }

   previousError = error;
   return CLAMP(
     q16x16_add_s(proportional, q16x16_add_s(integral, derivative)),
     outputMin,
     outputMax
   );
}
