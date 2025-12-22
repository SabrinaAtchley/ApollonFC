#ifndef APOLLON_FC_SRC_MADGWICK_H
#define APOLLON_FC_SRC_MADGWICK_H

#include "quaternion.h"
#include "q16x16/q16x16.h"

#define PRINT_QUAT(q) {Serial.print("w:");Serial.print(q.w);Serial.print(",x:");Serial.print(q.x);Serial.print(",y:");Serial.print(q.y);Serial.print(",z:");Serial.println(q.z);}

const Q16x16 accel_gate_low = q16x16_square_s(ftoq16x16(ACCEL_GATE_LOW));
const Q16x16 accel_gate_high = q16x16_square_s(ftoq16x16(ACCEL_GATE_HIGH));\
const Q16x16 beta = ftoq16x16(MADGWICK_BETA);

Quaternion approxQuaternion(const Q16x16, const Q16x16, const Q16x16);
bool normalizeAccel(Q16x16 &ax, Q16x16 &ay, Q16x16 &az);
// Calculates normalized gradient f from previous q and normalized accelerometer
Quaternion calculateBetaS(
  const Quaternion &q,
  Q16x16 ax, Q16x16 ay, Q16x16 az
);
Quaternion madgwickUpdate(
  const Quaternion &qPrev,
  const Q16x16 gx, const Q16x16 gy, const Q16x16 gz,
  Q16x16 ax, Q16x16 ay, Q16x16 az,
  const Q16x16 deltaT
);


#endif /* APOLLON_FC_SRC_MADGWICK_H */
