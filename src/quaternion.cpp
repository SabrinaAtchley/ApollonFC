#include "quaternion.h"

Quaternion& Quaternion::flipSign() {
  w = q16x16_negate(w);
  x = q16x16_negate(x);
  y = q16x16_negate(y);
  z = q16x16_negate(z);
  return *this;
}

Quaternion& Quaternion::matchHemisphere(const Quaternion &prev) {
  Q16x16 dotProduct = Quaternion::dot(*this, prev);
  if (dotProduct < 0) {
    return this->flipSign();
  }
  return *this;
}

Quaternion& Quaternion::conjugate() {
  x = q16x16_negate(x);
  y = q16x16_negate(y);
  z = q16x16_negate(z);
  return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion &rhs) {
  w = q16x16_add_s(w, rhs.w);
  x = q16x16_add_s(x, rhs.x);
  y = q16x16_add_s(y, rhs.y);
  z = q16x16_add_s(z, rhs.z);
  return *this;

}

Quaternion& Quaternion::operator-=(const Quaternion &rhs) {
  w = q16x16_sub_s(w, rhs.w);
  x = q16x16_sub_s(x, rhs.x);
  y = q16x16_sub_s(y, rhs.y);
  z = q16x16_sub_s(z, rhs.z);
  return *this;
}

Quaternion& Quaternion::normalize() {
  Q16x16 normSquared = q16x16_add_s(
    q16x16_add_s(
      q16x16_mul_s(w, w),
      q16x16_mul_s(x, x)
    ),
    q16x16_add_s(
      q16x16_mul_s(y, y),
      q16x16_mul_s(z, z)
    )
  );
  if (normSquared == 0) {
    w = 0x00010000;
    x = 0;
    y = 0;
    z = 0;
    return *this;
  }
  Q16x16 invNorm = q16x16_invsqrt(normSquared);

  w = q16x16_mul_s(invNorm, w);
  x = q16x16_mul_s(invNorm, x);
  y = q16x16_mul_s(invNorm, y);
  z = q16x16_mul_s(invNorm, z);

  return *this;
}

Quaternion& Quaternion::scale(const Q16x16 s) {
  w = q16x16_mul_s(w, s);
  x = q16x16_mul_s(x, s);
  y = q16x16_mul_s(y, s);
  z = q16x16_mul_s(z, s);

  return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion &q) {
  Q16x16 pw, px, py, pz;
  // pw*qw - px*qx - py*qy - pz*qz
  pw = q16x16_mul_s(w, q.w);
  pw = q16x16_sub_s(pw, q16x16_mul_s(x, q.x));
  pw = q16x16_sub_s(pw, q16x16_mul_s(y, q.y));
  pw = q16x16_sub_s(pw, q16x16_mul_s(z, q.z));

  // pw*qx + px*qw + py*qz - pz*qy
  px = q16x16_mul_s(w, q.x);
  px = q16x16_add_s(px, q16x16_mul_s(x, q.w));
  px = q16x16_add_s(px, q16x16_mul_s(y, q.z));
  px = q16x16_sub_s(px, q16x16_mul_s(z, q.y));

  // pw*qy - px*qz + py*qw + pz*qx
  py = q16x16_mul_s(w, q.y);
  py = q16x16_sub_s(py, q16x16_mul_s(x, q.z));
  py = q16x16_add_s(py, q16x16_mul_s(y, q.w));
  py = q16x16_add_s(py, q16x16_mul_s(z, q.x));

  // pw*qz + px*qy - py*qx + pz*qw
  pz = q16x16_mul_s(w, q.z);
  pz = q16x16_add_s(pz, q16x16_mul_s(x, q.y));
  pz = q16x16_sub_s(pz, q16x16_mul_s(y, q.x));
  pz = q16x16_add_s(pz, q16x16_mul_s(z, q.w));

  w = pw;
  x = px;
  y = py;
  z = pz;
  return *this;
}

// Non-mutating functions
Quaternion Quaternion::mul(Quaternion a, const Quaternion &b) {
  return a *= b;
}

Quaternion Quaternion::add(Quaternion a, const Quaternion &b) {
  return a += b;
}

Quaternion Quaternion::sub(Quaternion a, const Quaternion &b) {
  return a -= b;
}

Q16x16 Quaternion::dot(const Quaternion &a, const Quaternion &b) {
  Q16x16 result = q16x16_mul_s(a.w, b.w);
  result = q16x16_add_s(result, q16x16_mul_s(a.x, b.x));
  result = q16x16_add_s(result, q16x16_mul_s(a.y, b.y));
  result = q16x16_add_s(result, q16x16_mul_s(a.z, b.z));

  return result;
}
