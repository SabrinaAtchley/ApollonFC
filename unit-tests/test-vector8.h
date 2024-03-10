#ifndef APOLLON_FC_UNIT_TESTS_TEST_VECTOR8_H
#define APOLLON_FC_UNIT_TESTS_TEST_VECTOR8_H

#include "test-framework.h"
#include "../src/vector8.h"
#include "../src/macros.h"

void unitVector8() {
  /* Vector declarations */

  // Equality and Inequality Functions
  static constexpr uint8_t v1_1[] = {2, 4, 8};
  static constexpr uint8_t v1_2[] = {2, 4, 8};
  static constexpr uint8_t v1_3[] = {2, 4, 8};
  static constexpr uint8_t v1_4[] = {2, 5, 8};
  static constexpr uint8_t v1_5[] = {1, 4, 8};

  // Vector Arithmetic
  static constexpr int16_t a[5] = {3, 4, 5, 6, 7};
  static constexpr int16_t b[5] = {7, 6, 5, 4, 3};
  static constexpr int16_t c[5] = {2, 4, 8, 16, 32};
  static constexpr int16_t d[5] = {32, 16, 8, 4, 2};

  static constexpr int16_t _0[5] = {0, 0, 0, 0, 0};
  static constexpr int16_t _5a[5] = {15, 20, 25, 30, 35};
  static constexpr int16_t _5b[5] = {35, 30, 25, 20, 15};
  static constexpr int16_t _5c[5] = {10, 20, 40, 80, 160};
  static constexpr int16_t _5d[5] = {160, 80, 40, 20, 10};
  static constexpr int16_t __5a[5] = {-15, -20, -25, -30, -35};
  static constexpr int16_t __5b[5] = {-35, -30, -25, -20, -15};
  static constexpr int16_t __5c[5] = {-10, -20, -40, -80, -160};
  static constexpr int16_t __5d[5] = {-160, -80, -40, -20, -10};

  static constexpr int16_t _b[5] = {-7, -6, -5, -4, -3};
  static constexpr int16_t _c[5] = {-2, -4, -8, -16, -32};
  static constexpr int16_t _d[5] = {-32, -16, -8, -4, -2};

  static constexpr int16_t ab[5] = {10, 10, 10, 10, 10};
  static constexpr int16_t ac[5] = {5, 8, 13, 22, 39};
  static constexpr int16_t ad[5] = {35, 20, 13, 10, 9};
  static constexpr int16_t a_b[5] = {-4, -2, 0, 2, 4};
  static constexpr int16_t a_c[5] = {1, 0, -3, -10, -25};
  static constexpr int16_t a_d[5] = {-29, -12, -3, 2, 5};
  static const int16_t adb = 115;
  static const int16_t adc = 382;
  static const int16_t add = 238;

  static constexpr int16_t bc[5] = {9, 10, 13, 20, 35};
  static constexpr int16_t bd[5] = {39, 22, 13, 8, 5};
  static constexpr int16_t b_c[5] = {5, 2, -3, -12, -29};
  static constexpr int16_t b_d[5] = {-25, -10, -3, 0, 1};
  static const int bdc = 238;
  static const int bdd = 382;

  static constexpr int16_t cd[5] = {34, 20, 16, 20, 34};
  static constexpr int16_t c_d[5] = {-30, -12, 0, 12, 30};
  static const int16_t cdd = 320;

  /* Tests */

  TEST_UNIT(Vector16, {
    TEST_SECTION(Equality and Inequality Functions, {
      TEST_SECTION(Equality Function, {
        TEST_CASE(1.1.1, vectorEquivalent(v1_1, v1_2, 3));
        TEST_CASE(1.1.2, vectorEquivalent(v1_1, v1_3, 3));
        TEST_FALSE(1.1.4, vectorEquivalent(v1_1, v1_4, 3));
        TEST_FALSE(1.1.5, vectorEquivalent(v1_1, v1_5, 3));
      }); /* Equality Operator */

      TEST_SECTION(Inequality Operator, {
        TEST_FALSE(1.2.1, vectorNotEquivalent(v1_1, v1_2, 3));
        TEST_FALSE(1.2.2, vectorNotEquivalent(v1_1, v1_3, 3));
        TEST_CASE(1.2.4, vectorNotEquivalent(v1_1, v1_4, 3));
        TEST_CASE(1.2.5, vectorNotEquivalent(v1_1, v1_5, 3));
      }); /* Inequality Operator */
    }); /* Equality and Inequality Operator */

    TEST_SECTION(Copy Function, {
      uint8_t v_copy[3];
      TEST_CASE(2.1.1, vectorNotEquivalent(v1_1, v_copy, 3));
      vectorCopy(v_copy, v1_1, 3);
      TEST_CASE(2.1.2, vectorEquivalent(v1_1, v_copy, 3));
      TEST_CASE(2.1.3, vectorNotEquivalent(v1_4, v_copy, 3));
      vectorCopy(v_copy, v1_4, 3);
      TEST_CASE(2.1.4, vectorEquivalent(v1_4, v_copy, 3));
    }); /* Copy Function */

    TEST_SECTION(Vector Arithmetic, {
      TEST_SECTION(Vector Addition, {
        int16_t result[5];

        // a
        vectorCopy(result, a, 5);
        vectorAddition(result, b, 5);
        TEST_CASE(2.1.1, vectorEquivalent(result, ab, 5));

        vectorCopy(result, a, 5);
        vectorAddition(result, c, 5);
        TEST_CASE(2.1.2, vectorEquivalent(result, ac, 5));

        vectorCopy(result, a, 5);
        vectorAddition(result, d, 5);
        TEST_CASE(2.1.3, vectorEquivalent(result, ad, 5));

        vectorCopy(result, a, 5);
        vectorAddition(result, _b, 5);
        TEST_CASE(2.1.4, vectorEquivalent(result, a_b, 5));

        vectorCopy(result, a, 5);
        vectorAddition(result, _c, 5);
        TEST_CASE(2.1.5, vectorEquivalent(result, a_c, 5));

        vectorCopy(result, a, 5);
        vectorAddition(result, _d, 5);
        TEST_CASE(2.1.6, vectorEquivalent(result, a_d, 5));

        // b
        vectorCopy(result, b, 5);
        vectorAddition(result, c, 5);
        TEST_CASE(2.1.7, vectorEquivalent(result, bc, 5));

        vectorCopy(result, b, 5);
        vectorAddition(result, d, 5);
        TEST_CASE(2.1.8, vectorEquivalent(result, bd, 5));

        vectorCopy(result, b, 5);
        vectorAddition(result, _c, 5);
        TEST_CASE(2.1.9, vectorEquivalent(result, b_c, 5));

        vectorCopy(result, b, 5);
        vectorAddition(result, _d, 5);
        TEST_CASE(2.1.10, vectorEquivalent(result, b_d, 5));

        // c

        vectorCopy(result, c, 5);
        vectorAddition(result, d, 5);
        TEST_CASE(2.1.11, vectorEquivalent(result, cd, 5));

        vectorCopy(result, c, 5);
        vectorAddition(result, _d, 5);
        TEST_CASE(2.1.12, vectorEquivalent(result, c_d, 5));
      }); /* Vector Addition */

      TEST_SECTION(Vector Subtraction, {
        int16_t result[5];

        // a
        vectorCopy(result, a, 5);
        vectorSubtract(result, _b, 5);
        TEST_CASE(2.2.1, vectorEquivalent(result, ab, 5));

        vectorCopy(result, a, 5);
        vectorSubtract(result, _c, 5);
        TEST_CASE(2.2.2, vectorEquivalent(result, ac, 5));

        vectorCopy(result, a, 5);
        vectorSubtract(result, _d, 5);
        TEST_CASE(2.2.3, vectorEquivalent(result, ad, 5));

        vectorCopy(result, a, 5);
        vectorSubtract(result, b, 5);
        TEST_CASE(2.2.4, vectorEquivalent(result, a_b, 5));

        vectorCopy(result, a, 5);
        vectorSubtract(result, c, 5);
        TEST_CASE(2.2.5, vectorEquivalent(result, a_c, 5));

        vectorCopy(result, a, 5);
        vectorSubtract(result, d, 5);
        TEST_CASE(2.2.6, vectorEquivalent(result, a_d, 5));

        // b
        vectorCopy(result, b, 5);
        vectorSubtract(result, _c, 5);
        TEST_CASE(2.2.7, vectorEquivalent(result, bc, 5));

        vectorCopy(result, b, 5);
        vectorSubtract(result, _d, 5);
        TEST_CASE(2.2.8, vectorEquivalent(result, bd, 5));

        vectorCopy(result, b, 5);
        vectorSubtract(result, c, 5);
        TEST_CASE(2.2.9, vectorEquivalent(result, b_c, 5));

        vectorCopy(result, b, 5);
        vectorSubtract(result, d, 5);
        TEST_CASE(2.2.10, vectorEquivalent(result, b_d, 5));

        // c

        vectorCopy(result, c, 5);
        vectorSubtract(result, _d, 5);
        TEST_CASE(2.2.11, vectorEquivalent(result, cd, 5));

        vectorCopy(result, c, 5);
        vectorSubtract(result, d, 5);
        TEST_CASE(2.2.12, vectorEquivalent(result, c_d, 5));
      }); /* Vector Subtraction */

      TEST_SECTION(Scalar Product, {
        int16_t result[5];

        // Scale by 0
        vectorCopy(result, a, 5);
        vectorScalarProduct(0, result, 5);
        TEST_CASE(2.3.1, vectorEquivalent(result, _0, 5));

        vectorCopy(result, b, 5);
        vectorScalarProduct(0, result, 5);
        TEST_CASE(2.3.2, vectorEquivalent(result, _0, 5));

        vectorCopy(result, c, 5);
        vectorScalarProduct(0, result, 5);
        TEST_CASE(2.3.3, vectorEquivalent(result, _0, 5));

        vectorCopy(result, d, 5);
        vectorScalarProduct(0, result, 5);
        TEST_CASE(2.3.4, vectorEquivalent(result, _0, 5));

        // Scale by 5
        vectorCopy(result, a, 5);
        vectorScalarProduct(5, result, 5);
        TEST_CASE(2.3.5, vectorEquivalent(result, _5a, 5));

        vectorCopy(result, b, 5);
        vectorScalarProduct(5, result, 5);
        TEST_CASE(2.3.6, vectorEquivalent(result, _5b, 5));

        vectorCopy(result, c, 5);
        vectorScalarProduct(5, result, 5);
        TEST_CASE(2.3.7, vectorEquivalent(result, _5c, 5));

        vectorCopy(result, d, 5);
        vectorScalarProduct(5, result, 5);
        TEST_CASE(2.3.8, vectorEquivalent(result, _5d, 5));

        // Scale by -5
        vectorCopy(result, a, 5);
        vectorScalarProduct(-5, result, 5);
        TEST_CASE(2.3.9, vectorEquivalent(result, __5a, 5));

        vectorCopy(result, b, 5);
        vectorScalarProduct(-5, result, 5);
        TEST_CASE(2.3.10, vectorEquivalent(result, __5b, 5));

        vectorCopy(result, c, 5);
        vectorScalarProduct(-5, result, 5);
        TEST_CASE(2.3.11, vectorEquivalent(result, __5c, 5));

        vectorCopy(result, d, 5);
        vectorScalarProduct(-5, result, 5);
        TEST_CASE(2.3.12, vectorEquivalent(result, __5d, 5));
      }); /* Scalar Product */

      TEST_SECTION(Dot Product, {
        int16_t v_copy[5];

        // a
        vectorCopy(v_copy, a, 5);
        TEST_CASE(2.4.1, vectorDotProduct(v_copy, b, 5) == adb);

        vectorCopy(v_copy, a, 5);
        TEST_CASE(2.4.2, vectorDotProduct(v_copy, c, 5) == adc);

        vectorCopy(v_copy, a, 5);
        TEST_CASE(2.4.3, vectorDotProduct(v_copy, d, 5) == add);

        // b
        vectorCopy(v_copy, b, 5);
        TEST_CASE(2.4.4, vectorDotProduct(v_copy, c, 5) == bdc);

        vectorCopy(v_copy, b, 5);
        TEST_CASE(2.4.5, vectorDotProduct(v_copy, d, 5) == bdd);

        // c
        vectorCopy(v_copy, c, 5);
        TEST_CASE(2.4.6, vectorDotProduct(v_copy, d, 5) == cdd);
      }); /* Dot Product */

    }); /* Vector Arithmetic */
  }); /* Vector16 */

}/* setup() */

#endif
