#ifndef APOLLON_FC_SRC_PID_H
#define APOLLON_FC_SRC_PID_H

#include "../configuration.h"
#include "q16x16/q16x16.h"

class PID {
private:
  const Q16x16 kp, ki, kd, iMin, iMax,
               pScale = 1, iScale = 1, dScale = 1,
               alpha = ftoq16x16(0.5), beta, // beta = 1 - alpha
               pGain, iGain, dGain,
               outputMin, outputMax;

  Q16x16 previousError = 0, proportional = 0, integral = 0, derivative = 0,
         s1 = 0, s2 = 0;
  bool dInit = false;

public:
  bool enableIntegration = true;

  PID(
    const Q16x16 _kp, const Q16x16 _ki, const Q16x16 _kd,
    const Q16x16 _pScale, const Q16x16 _iScale, const Q16x16 _dScale,
    const Q16x16 _iMin, const Q16x16 _iMax, const Q16x16 _alpha
  );

  Q16x16 update(const Q16x16 setPoint, const Q16x16 value, const Q16x16 deltaT);

  const Q16x16 getLastPTerm() const;
  const Q16x16 getLastITerm() const;
  const Q16x16 getLastDTerm() const;
};

#endif /* APOLLON_FC_SRC_PID_H */
