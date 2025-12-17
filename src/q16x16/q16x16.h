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
// constexpr Q16x16 q16x16_mod(const Q16x16, const Q16x16);





#endif /* APOLLON_FC_SRC_Q16X16_H */
