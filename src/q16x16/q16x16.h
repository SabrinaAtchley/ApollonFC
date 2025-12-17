#ifndef APOLLON_FC_SRC_Q16X16_H
#define APOLLON_FC_SRC_Q16X16_H

#include <Arduino.h>

/* Library for fixed-point arithmetic
 *
 *
 */

typedef int32_t Q16x16; // Signed Q16.16 integer
const Q16x16 q16x16_max = 0x7FFFFFFF;
const Q16x16 q16x16_min = 0x80000000;

// Basic arithmetic
// Add two Q16.16 numbers and perform overflow clamping
Q16x16 q16x16_add_s(const Q16x16, const Q16x16);
// Subtract two Q16.16 numbers and perform overflow clamping
Q16x16 q16x16_sub_s(const Q16x16, const Q16x16);
// Multiplies two Q16.16 numbers and performs overflow clamping
Q16x16 q16x16_mul_s(const Q16x16, const Q16x16);
Q16x16 q16x16_div_s(const Q16x16, const Q16x16);

static constexpr Q16x16 ftoq16x16(const float x) {
  return (int32_t) (x * 65536);
}

static constexpr Q16x16 itoq16x16(const float x) {
  return (Q16x16) ((int32_t) x << 16);
}

static constexpr float q16x16tof(const Q16x16 x) {
  return ((float) x) / 65536.0;
}

static inline Q16x16 pgm_read_q16x16(const Q16x16 *p) {
  return (Q16x16) pgm_read_dword(p);
}


#endif /* APOLLON_FC_SRC_Q16X16_H */
