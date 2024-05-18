#ifndef APOLLON_FC_SRC_PID_H
#define APOLLON_FC_SRC_PID_H

#include "arrayQueue.h"
#include "../configuration.h"

class PID {
private:
  float kp, ki, kd, iMin, iMax;
  // Used for scaling coefficients to easier to work with numbers
  const float pScale = 1, iScale = 1, dScale = 1;
  float previousError = 0.0;
  float proportional = 0.0, integral = 0.0, derivative = 0.0;
  float integralSum = 0.0;
  float s1 = 0.0, s2 = 0.0; // Used for derivative linear exponential smoothing
  float alpha = 0.5; // 0 < alpha < 1, used for derivative LES

public:
  bool enableIntegration = true;

  PID(
    const float _kp, const float _ki, const float _kd,
    const float _pScale, const float _iScale, const float _dScale,
    const float _iMin, const float _iMax
  );

  void setCoefficients(const float newKp, const float newKi, const float newKd);
  void setIntegralBounds(const float newIMin, const float newIMax);

  // Takes a new error value and its delta-t and returns a new control value
  const float update(const float setPoint, const float value, const unsigned long deltaT);

  // Return the last P,I,D terms used to make the output, for tuning and debug purposes
  // Includes coefficients
  const float getLastPTerm() const;
  const float getLastITerm() const;
  const float getLastDTerm() const;
};

#endif /* APOLLON_FC_SRC_PID_H */
