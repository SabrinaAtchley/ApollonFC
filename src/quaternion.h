#ifndef APOLLON_FC_SRC_QUATERNION_H
#define APOLLON_FC_SRC_QUATERNION_H

#include <Arduino.h>
#include "q16x16/q16x16.h"

class Quaternion {
public:
  Q16x16 w, x, y, z;

  constexpr Quaternion() : w(0x00010000), x(0), y(0), z(0) {}
  constexpr Quaternion(const Q16x16 _w, const Q16x16 _x, const Q16x16 _y, const Q16x16 _z)
    : w(_w), x(_x), y(_y), z(_z) {}

  // Mutating functions
  Quaternion& flipSign();
  Quaternion& matchHemisphere(const Quaternion&);
  Quaternion& conjugate();
  Quaternion& normalize();
  Quaternion& operator*=(const Quaternion&);
  Quaternion& operator+=(const Quaternion&);
  Quaternion& operator-=(const Quaternion&);

  // Non-mutating functions
  static Quaternion mul(Quaternion, const Quaternion&);
  static Quaternion add(Quaternion, const Quaternion&);
  static Quaternion sub(Quaternion, const Quaternion&);
  static Q16x16 dot(const Quaternion&, const Quaternion&);
};


#endif /* APOLLON_FC_SRC_QUATERNION_H */
