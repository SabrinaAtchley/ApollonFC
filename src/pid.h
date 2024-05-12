#ifndef APOLLON_FC_SRC_PID_H
#define APOLLON_FC_SRC_PID_H

#include "arrayQueue.h"
#include "../configuration.h"

class PID {
private:
  float kp, ki, kd, iMin, iMax;
  float __errors[PID_ERROR_BUFFER_SIZE];
  unsigned long __times[PID_ERROR_BUFFER_SIZE];
  ArrayQueue<float> errors;
  ArrayQueue<unsigned long> times;

public:
  bool enableIntegration = true;

  PID(
    const float _kp, const float _ki, const float _kd,
    const float _iMin, const float _iMax
  );

  void setCoefficients(const float newKp, const float newKi, const float newKd);
  void setIntegralBounds(const float newIMin, const float newIMax);

  // Takes a new error value and its delta-t and returns a new control value
  const float update(const float error, const unsigned long deltaT);
};

#endif /* APOLLON_FC_SRC_PID_H */
