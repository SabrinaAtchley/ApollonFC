#ifndef APOLLON_FC_UNIT_TESTS_TEST_Q16X16_H
#define APOLLON_FC_UNIT_TESTS_TEST_Q16X16_H

#include "test-framework.h"
#include "../src/q16x16/q16x16.h"

namespace {
  // Convert an integer to Q16x16
  Q16x16 i2q(Q16x16 const a) {
    return (int32_t) a << 16;
  }
}

void unitQ16x16() {
  Q16x16 q16x16_pi = 205887; // 3.14159265
  Q16x16 q16x16_neg_four = -262144; // -4

  TEST_UNIT(Q16x16, {
    TEST_SECTION(Safe Addition, {
      TEST_CASE(1.1, q16x16_add_s(q16x16_pi, q16x16_neg_four) == -56257); // -0.858413696
      TEST_CASE(1.2, q16x16_add_s(q16x16_pi, q16x16_max) == q16x16_max); // Overflow test
      TEST_CASE(1.3, q16x16_add_s(q16x16_neg_four, q16x16_min) == q16x16_min); // Underflow test
      TEST_CASE(1.4, q16x16_add_s(q16x16_max, 0) == q16x16_max);        // MAX + 0
      TEST_CASE(1.5, q16x16_add_s(q16x16_min, 0) == q16x16_min);        // MIN + 0
      TEST_CASE(1.6, q16x16_add_s(0, q16x16_max) == q16x16_max);        // 0 + MAX (commutative)
      TEST_CASE(1.7, q16x16_add_s(0, q16x16_min) == q16x16_min);        // 0 + MIN
      // Overflow boundary tests
      TEST_CASE(1.8, q16x16_add_s(q16x16_max, 0) == q16x16_max); // MAX + 0
      TEST_CASE(1.9, q16x16_add_s(q16x16_max, 0x00010000) == q16x16_max); // MAX + 1 → clamp
      TEST_CASE(1.10, q16x16_add_s(q16x16_max, q16x16_max) == q16x16_max); // MAX + MAX → clamp

      // Underflow boundary tests
      TEST_CASE(1.11, q16x16_add_s(q16x16_min, 0) == q16x16_min); // MIN + 0
      TEST_CASE(1.12, q16x16_add_s(q16x16_min, 0xFFFF0000) == q16x16_min); // MIN + -1 → clamp
      TEST_CASE(1.13, q16x16_add_s(q16x16_min, q16x16_min) == q16x16_min); // MIN + MIN → clamp

      // Positive/negative combination tests
      TEST_CASE(1.14, q16x16_add_s(0x40000000, 0x40000000) == q16x16_max); // Large positive + large positive = no overflow
      TEST_CASE(1.15, q16x16_add_s(0x40000000, 0xC0000000) == 0); // Large positive + large negative = 0 (no overflow)
      TEST_CASE(1.16, q16x16_add_s(0xC0000000, 0xC0000000) == q16x16_min); // Large negative + large negative = underflow

      // Specific Q16.16 value tests (with decimal equivalents)
      TEST_CASE(1.17, q16x16_add_s(0x00010000, 0x00010000) == 0x00020000); // 1.0 + 1.0 = 2.0
      TEST_CASE(1.18, q16x16_add_s(0x00018000, 0x00008000) == 0x00020000); // 1.5 + 0.5 = 2.0
      TEST_CASE(1.19, q16x16_add_s(0xFFFF0000, 0x00010000) == 0); // -1.0 + 1.0 = 0.0

      // Carry propagation test
      TEST_CASE(1.20, q16x16_add_s(0x0000FFFF, 0x00000001) == 0x00010000); // 0.999... + 0.000015 = 1.0
      TEST_CASE(1.21, q16x16_add_s(0xFFFFFFFF, 0x00000001) == 0); // -0.000015 + 0.000015 = 0.0

      // Symmetry tests
      TEST_CASE(1.22, q16x16_add_s(0x12345678, 0x9ABCDEF0) == q16x16_add_s(0x9ABCDEF0, 0x12345678));
      TEST_CASE(1.23, q16x16_add_s(q16x16_neg_four, q16x16_pi) == q16x16_add_s(q16x16_pi, q16x16_neg_four)); // Commutative property
    }); /* Safe Addition */

    TEST_SECTION(Safe Subtraction, {
      TEST_CASE(2.1, q16x16_sub_s(6553600, 1638400) == 4915200); // 100 - 25 = 75
      TEST_CASE(2.2, q16x16_sub_s(q16x16_pi, q16x16_min) == q16x16_max); // pi - min = max
      TEST_CASE(2.3, q16x16_sub_s(q16x16_neg_four, q16x16_max) == q16x16_min); // -4 - max = min

      // Basic identity tests
      TEST_CASE(2.4, q16x16_sub_s(0x00010000, 0) == 0x00010000); // 1.0 - 0 = 1.0
      TEST_CASE(2.5, q16x16_sub_s(0, 0x00010000) == 0xFFFF0000); // 0 - 1.0 = -1.0
      TEST_CASE(2.6, q16x16_sub_s(0x00010000, 0x00010000) == 0); // 1.0 - 1.0 = 0

      // MIN/MAX boundary tests
      TEST_CASE(2.7, q16x16_sub_s(q16x16_max, 0) == q16x16_max);          // MAX - 0
      TEST_CASE(2.8, q16x16_sub_s(q16x16_min, 0) == q16x16_min);          // MIN - 0
      TEST_CASE(2.9, q16x16_sub_s(0, q16x16_max) == 0x80000001);          // 0 - MAX = MIN + 1
      TEST_CASE(2.10, q16x16_sub_s(0, q16x16_min) == q16x16_max);         // 0 - MIN = MAX (overflow)

      // Critical overflow/underflow cases
      TEST_CASE(2.11, q16x16_sub_s(q16x16_max, 0xFFFF0000) == q16x16_max); // MAX - (-1) → overflow → clamp MAX
      TEST_CASE(2.12, q16x16_sub_s(q16x16_max, q16x16_min) == q16x16_max); // MAX - MIN → overflow → clamp MAX
      TEST_CASE(2.13, q16x16_sub_s(q16x16_min, q16x16_max) == q16x16_min); // MIN - MAX → underflow → clamp MIN
      TEST_CASE(2.14, q16x16_sub_s(q16x16_min, 0x00010000) == q16x16_min); // MIN - 1 → underflow → clamp MIN

      // Special Q16.16 subtraction tests
      TEST_CASE(2.15, q16x16_sub_s(0x00020000, 0x00010000) == 0x00010000); // 2.0 - 1.0 = 1.0
      TEST_CASE(2.16, q16x16_sub_s(0x00010000, 0x00020000) == 0xFFFF0000); // 1.0 - 2.0 = -1.0
      TEST_CASE(2.17, q16x16_sub_s(0xFFFF0000, 0xFFFF0000) == 0); // -1.0 - (-1.0) = 0
      TEST_CASE(2.18, q16x16_sub_s(0xFFFF0000, 0x00010000) == 0xFFFE0000); // -1.0 - 1.0 = -2.0

      // Borrow/carry propagation tests
      TEST_CASE(2.19, q16x16_sub_s(0x00010000, 0x00000001) == 0x0000FFFF); // 1.0 - 0.000015 = 0.999985
      TEST_CASE(2.20, q16x16_sub_s(0, 0x00000001) == 0xFFFFFFFF); // 0 - 0.000015 = -0.000015

      // Inverse operation tests (a - b should equal -(b - a) when no overflow)
      Q16x16 test_val1 = 0x12345678;
      Q16x16 test_val2 = 0x23456789;
      //TEST_CASE(2.21, q16x16_sub_s(test_val1, test_val2) == q16x16_negate(q16x16_sub_s(test_val2, test_val1)));

      // Test the property: a - b = a + (-b)
      TEST_CASE(2.22, q16x16_sub_s(0x30000000, 0x10000000) == q16x16_add_s(0x30000000, 0xF0000000));

      // Edge case: subtracting from itself
      TEST_CASE(2.23, q16x16_sub_s(0x55555555, 0x55555555) == 0);
      TEST_CASE(2.24, q16x16_sub_s(q16x16_max, q16x16_max) == 0);
      TEST_CASE(2.25, q16x16_sub_s(q16x16_min, q16x16_min) == 0);

      // Large magnitude differences
      TEST_CASE(2.26, q16x16_sub_s(0x70000000, 0x90000000) == q16x16_max); // Large positive - large negative
      TEST_CASE(2.27, q16x16_sub_s(0x90000000, 0x70000000) == q16x16_min); // Large negative - large positive
    }); /* Safe Subtraction */

    TEST_SECTION(Safe Multiplication, {
      TEST_CASE(3.1, q16x16_mul_s(10 << 16, 5 << 16) == 50 << 16);
      TEST_CASE(3.2, q16x16_mul_s(0x610000, 0x22666) == 13667494); // 97 * 2.15 = 208.549
      TEST_CASE(3.3, q16x16_mul_s(-0x610000, 0x22666) == -13667494);
      TEST_CASE(3.4, q16x16_mul_s(0x80112233, 0x00FFFFFF) == q16x16_min); // -32785.133590698 * 255.999984741 = MIN
      TEST_CASE(3.5, q16x16_mul_s(0x7FEEDDCC, 0x00FFFFFF) == q16x16_max); // 32785.133590698 * 255.999984741 = MAX
      TEST_CASE(3.6, q16x16_mul_s(q16x16_min, 0x00010000) == q16x16_min); // MIN × 1
      TEST_CASE(3.7, q16x16_mul_s(q16x16_min, 0xFFFF0000) == q16x16_max); // MIN × -1 → Should clamp to MAX
      TEST_CASE(3.8, q16x16_mul_s(0xFFFF0000, 0xFFFF0000) == 0x00010000); // -1 × -1 = 1
      TEST_CASE(3.9, q16x16_mul_s(0xFFFE0000, 0x00020000) == 0xFFFC0000); // -2 × 2 = -4
      TEST_CASE(3.10, q16x16_mul_s(0x00020000, 0xFFFE0000) == 0xFFFC0000); // 2 × -2 = -4 (commutative)
      TEST_CASE(3.11, q16x16_mul_s(0xFFFF0000, 0x00010000) == 0xFFFF0000); // -1 × 1 = -1
      TEST_CASE(3.12, q16x16_mul_s(0x00010000, 0xFFFF0000) == 0xFFFF0000); // 1 × -1 = -1
    }); /* Safe Multiplication */

    TEST_SECTION(Safe Division, {
      // Basic integer results
      TEST_CASE(4.1, q16x16_div_s(i2q(10), i2q(5)) == i2q(2));      // 10 / 5 = 2
      TEST_CASE(4.2, q16x16_div_s(i2q(5), i2q(10)) == 0x00008000);     // 5 / 10 = 0.5
      TEST_CASE(4.3, q16x16_div_s(i2q(7), i2q(2)) == 0x00038000);      // 7 / 2 = 3.5
      TEST_CASE(4.4, q16x16_div_s(i2q(1), i2q(3)) == 0x00005555);      // 1 / 3 ≈ 0.333328 (trunc)

      // Sign handling
      TEST_CASE(4.5, q16x16_div_s(-i2q(10), i2q(5)) == -i2q(2));  // -10 / 5 = -2
      TEST_CASE(4.6, q16x16_div_s(i2q(10), -i2q(5)) == -i2q(2));  // 10 / -5 = -2
      TEST_CASE(4.7, q16x16_div_s(-i2q(10), -i2q(5)) == i2q(2));// -10 / -5 = 2
      TEST_CASE(4.8, q16x16_div_s(i2q(1), -i2q(2)) == 0xFFFF8000);   // 1 / -2 = -0.5

      // Division by zero: clamp based on sign of dividend
      TEST_CASE(4.9,  q16x16_div_s( i2q(1), 0) == q16x16_max);         // + / 0 -> MAX
      TEST_CASE(4.10, q16x16_div_s(-i2q(1), 0) == q16x16_min);         // - / 0 -> MIN
      TEST_CASE(4.11, q16x16_div_s(0, 0) == q16x16_max);                  // 0 / 0 -> MAX (a is non-negative)

      // Identity / simple denominators
      TEST_CASE(4.12, q16x16_div_s(0x00010000, 0x00010000) == 0x00010000);// 1 / 1 = 1
      TEST_CASE(4.13, q16x16_div_s(0x00020000, 0x00010000) == 0x00020000);// 2 / 1 = 2
      TEST_CASE(4.14, q16x16_div_s(0x00010000, 0x00020000) == 0x00008000);// 1 / 2 = 0.5
      TEST_CASE(4.15, q16x16_div_s(0xFFFF0000, 0x00010000) == 0xFFFF0000);// -1 / 1 = -1
      TEST_CASE(4.16, q16x16_div_s(0xFFFF0000, 0xFFFF0000) == 0x00010000);// -1 / -1 = 1

      // Saturation on overflow/underflow
      TEST_CASE(4.17, q16x16_div_s(q16x16_max, 0x00010000) == q16x16_max);// MAX / 1 = MAX
      TEST_CASE(4.18, q16x16_div_s(q16x16_min, 0x00010000) == q16x16_min);// MIN / 1 = MIN
      TEST_CASE(4.19, q16x16_div_s(q16x16_max, 0x00008000) == q16x16_max);// MAX / 0.5 -> overflow -> MAX
      TEST_CASE(4.20, q16x16_div_s(q16x16_min, 0x00008000) == q16x16_min);// MIN / 0.5 -> underflow -> MIN

      // The “MIN / -1” edge case: should clamp to MAX (like mul)
      TEST_CASE(4.21, q16x16_div_s(q16x16_min, 0xFFFF0000) == q16x16_max);// MIN / -1 -> MAX

      // A couple non-trivial fractional checks (exact in Q16.16)
      TEST_CASE(4.22, q16x16_div_s(0x00610000, 0x00022666) == 0x002D1DCC);// 97 / 2.15 ≈ 45.116 (trunc)
      TEST_CASE(4.23, q16x16_div_s(-0x00610000, 0x00022666) == 0xFFD2E234);// -97 / 2.15 ≈ -45.116 (trunc)

      // Small numerator, big denominator (goes toward 0 with correct sign)
      TEST_CASE(4.24, q16x16_div_s(1, 0x7FFF0000) == 0);                  // ~1/32767 -> 0
      TEST_CASE(4.25, q16x16_div_s(-1, 0x7FFF0000) == 0);                 // ~-1/32767 -> 0 (trunc toward 0)
    }); /* Safe Division */

    TEST_SECTION(Float to Q16x16, {
      TEST_CASE(5.1, ftoq16x16(2.5) == 163840);
      TEST_CASE(5.2, ftoq16x16(3.14159265) == 0x0003243F);
      TEST_CASE(5.3, ftoq16x16(-2.5) == 0xFFFD8000);
    }); /* Float to Q16x16 */
  }); /* Q16x16 */
}

#endif /* APOLLON_FC_UNIT_TESTS_TEST_Q16X16_H */
