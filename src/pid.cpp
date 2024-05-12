#include "pid.h"

PID::PID(
  const float _kp, const float _ki, const float _kd,
  const float _iMin, const float _iMax
) : errors(__errors, PID_ERROR_BUFFER_SIZE), times(__times, PID_ERROR_BUFFER_SIZE),
    kp(_kp), ki(_ki), kd(_kd), iMin(_iMin), iMax(_iMax) {}

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

const float PID::update(const float error, const unsigned long deltaT) {
  Serial.println("Unimplemented");
}
