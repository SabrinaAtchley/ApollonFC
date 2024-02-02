#ifndef FLIGHT_CONTROLLER_SRC_VECTOR16_H
#define FLIGHT_CONTROLLER_SRC_VECTOR16_H

/* Mathematical vector used for representing aircraft
 * attitude, position, kinematics, and controls.
 * Supports vectors up to n = 16
 */

#include "peripherals.h"
#include "macros.h"

template<typename T>
class Vector16 {
  const uint16_t _length;
  T *_data;

public:
  Vector16(T *elements, const uint16_t &n) : _length(n) {
    _data = elements;
  }

  Vector16(const uint16_t &n) : _length(n) {
    _data = new T[_length];
  }

  ~Vector16() {
    delete[] _data;
  }

  uint16_t length() {
    return _length;
  }


  // Operators

  constexpr T& operator[](const uint16_t &idx) const {
    if (idx < 0 || idx >= _length) {
      ERROR(1)
    }
    return _data[idx];
  }

  // Copy assignment

  Vector16<T> &operator=(const Vector16<T>& other) {
    if (this == &other)
      return *this;

    if (_length != other._length) {
      delete[] _data;
      _data = nullptr;
      //_length = 0; // Preserve invariants
      _data = new T[other._length];
      //_length = other._length;
    }
    memcpy(_data, other._data, _length * sizeof(T));
    return *this;
  }
  // Move assignment unsure how to implement lacking C++ standard library


  friend bool operator==(const Vector16<T> &l, const Vector16<T> &r) {
    if (&l == &r)
      return true;
    if (l._length != r._length)
      return false;
    for (uint16_t i = 0; i < l._length; i++) {
      if (l[i] != r[i])
        return false;
    }
    return true;
  }

  friend bool operator!=(const Vector16<T> &l, const Vector16<T> &r) {
    return !(l == r);
  }

  friend Vector16<T> operator+(const Vector16<T> &l, const Vector16<T> &r) {
    if (l._length != r._length)
      ERROR(0);

    Vector16<T> result(l._length);
    for (uint16_t i = 0; i < l._length; i++)
      result[i] = l[i] + r[i];

    return result;
  }

  friend Vector16<T> operator-(const Vector16<T> &l, const Vector16<T> &r) {
    if (l._length != r._length)
      ERROR(0);

    Vector16<T> result(l._length);
    for (uint16_t i = 0; i < l._length; i++)
      result[i] = l[i] - r[i];

    return result;
  }

  // Scalar product
  friend Vector16<T> operator*(const Vector16<T> &l, const T &r) {
    Vector16<T> result(l._length);
    for (uint16_t i = 0; i < l._length; i++)
      result[i] = l[i] * r;

    return result;
  }
  // Scalar product (scalar first)
  friend Vector16<T> operator*(const T &l, const Vector16<T> &r) {
    return r * l;
  }

  // Dot product
  friend T operator*(const Vector16<T> &l, const Vector16<T> &r) {
    if (l._length != r._length)
      ERROR(0);

    T sum = 0;
    for (uint16_t i = 0; i < l._length; i++)
      sum += l[i] * r[i];

    return sum;
  }
};
#endif
