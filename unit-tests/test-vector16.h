#ifndef APOLLON_FC_UNIT_TESTS_TEST_VECTOR16_H
#define APOLLON_FC_UNIT_TESTS_TEST_VECTOR16_H

#include "test-framework.h"
#include "../src/vector16.h"

// e.g. VECTOR16(uint16_t, v1_1, 5, {2, 4, 8, 16, 32});
#define VECTOR16(T, v, n, list...) T CAT(CAT(__, v), _data)[] = list; Vector16<T> v(CAT(CAT(__, v), _data), n)

void unitVector16() {
  /* Vector declarations */

  // Length constructor
  Vector16<uint8_t> v1_1(5);

  // Elements constructor and Indexing operator
  VECTOR16(uint8_t, v2_1, 5, {2, 4, 8, 16, 32});

  // Equality and Inequality operators
  VECTOR16(uint8_t, v3_1, 3, {2, 4, 8});
  VECTOR16(uint8_t, v3_2, 3, {2, 4, 8});
  VECTOR16(uint8_t, v3_3, 3, {2, 4, 8});
  VECTOR16(uint8_t, v3_4, 4, {2, 4, 8, 16});
  VECTOR16(uint8_t, v3_5, 3, {2, 5, 8});
  VECTOR16(uint8_t, v3_6, 3, {1, 4, 8});

  // Copy assignment
  VECTOR16(uint8_t, v4_1, 5, {2,4,8,16,32});
  Vector16<uint8_t> v4_2(5);

  // Vector Arithmetic
  VECTOR16(int, a, 5, {3, 4, 5, 6, 7});
  VECTOR16(int, b, 5, {7, 6, 5, 4, 3});
  VECTOR16(int, c, 5, {2, 4, 8, 16, 32});
  VECTOR16(int, d, 5, {32, 16, 8, 4, 2});

  VECTOR16(int, _0, 5, {0, 0, 0, 0, 0});
  VECTOR16(int, _5a, 5, {15, 20, 25, 30, 35});
  VECTOR16(int, _5c, 5, {10, 20, 40, 80, 160});
  VECTOR16(int, __5a, 5, {-15, -20, -25, -30, -35});
  VECTOR16(int, __5c, 5, {-10, -20, -40, -80, -160});

  VECTOR16(int, _b, 5, {-7, -6, -5, -4, -3});
  VECTOR16(int, _c, 5, {-2, -4, -8, -16, -32});
  VECTOR16(int, _d, 5, {-32, -16, -8, -4, -2});

  VECTOR16(int, ab, 5, {10, 10, 10, 10, 10});
  VECTOR16(int, ac, 5, {5, 8, 13, 22, 39});
  VECTOR16(int, ad, 5, {35, 20, 13, 10, 9});
  VECTOR16(int, a_b, 5, {-4, -2, 0, 2, 4});
  VECTOR16(int, a_c, 5, {1, 0, -3, -10, -25});
  VECTOR16(int, a_d, 5, {-29, -12, -3, 2, 5});
  const int adb = 115;
  const int adc = 382;
  const int add = 238;

  VECTOR16(int, bc, 5, {9, 10, 13, 20, 35});
  VECTOR16(int, bd, 5, {39, 22, 13, 8, 5});
  VECTOR16(int, b_c, 5, {5, 2, -3, -12, -29});
  VECTOR16(int, b_d, 5, {-25, -10, -3, 0, 1});
  const int bdc = 238;
  const int bdd = 382;

  VECTOR16(int, cd, 5, {34, 20, 16, 20, 34});
  VECTOR16(int, c_d, 5, {-30, -12, 0, 12, 30});
  const int cdd = 320;


  /* Tests */

  TEST_UNIT(Vector16, {
    TEST_SECTION(Length Constructor, {
      //REQUIRE_NOTHROWS(1.1, new Vector16<uint8_t>(5));
      REQUIRE(1.2, v1_1.length() == 5);
    }); /* Length Constructor */

    TEST_SECTION(Elements Constructor, {
      //REQUIRE_THROWS(2.1, new Vector16<uint8_t>([1,2,3,4,5], 5));
      REQUIRE(2.2, v2_1.length() == 5);

      TEST_SECTION(Indexing Operator, {
        TEST_CASE(2.4.1, v2_1[0] == 2);
        TEST_CASE(2.4.2, v2_1[1] == 4);
        TEST_CASE(2.4.3, v2_1[2] == 8);
        TEST_CASE(2.4.4, v2_1[3] == 16);
        TEST_CASE(2.4.5, v2_1[4] == 32);
      }); /* Indexing Operator */
    }); /* Elements Constructor */

    TEST_SECTION(Equality and Inequality Operator, {
      TEST_SECTION(Equality Operator, {
        TEST_CASE(3.1.1, v3_1 == v3_2);
        TEST_CASE(3.1.2, v3_1 == v3_3);
        TEST_FALSE(3.1.3, v3_1 == v3_4);
        TEST_FALSE(3.1.4, v3_1 == v3_5);
        TEST_FALSE(3.1.5, v3_1 == v3_6);
      }); /* Equality Operator */

      TEST_SECTION(Inequality Operator, {
        TEST_FALSE(3.2.1, v3_1 != v3_2);
        TEST_FALSE(3.2.2, v3_1 != v3_3);
        TEST_CASE(3.2.3, v3_1 != v3_4);
        TEST_CASE(3.2.4, v3_1 != v3_5);
        TEST_CASE(3.2.5, v3_1 != v3_6);
      }); /* Inequality Operator */
    }); /* Equality and Inequality Operator */

    TEST_SECTION(Copy Assignment, {
      v4_2 = v4_1;
      //REQUIRE_THROW(4.1, Vector16<uint8_t> v2 = v1);
      TEST_CASE(4.2, v4_2.length() == v4_1.length());
      TEST_CASE(4.3, v4_2 == v4_1);
      TEST_CASE(4.4, &v4_2 != &v4_1);
    }); /* Copy Assignment */

    TEST_SECTION(Vector Arithmetic, {

      TEST_SECTION(Vector Addition, {
        TEST_CASE(5.1.1, a + b == ab);
        TEST_CASE(5.1.2, a + c == ac);
        TEST_CASE(5.1.3, a + d == ad);
        TEST_CASE(5.1.4, a + _b == a_b);
        TEST_CASE(5.1.5, a + _c == a_c);
        TEST_CASE(5.1.6, a + _d == a_d);

        TEST_CASE(5.1.7, b + c == bc);
        TEST_CASE(5.1.8, b + d == bd);
        TEST_CASE(5.1.9, b + _c == b_c);
        TEST_CASE(5.1.10, b + _d == b_d);

        TEST_CASE(5.1.11, c + d == cd);
        TEST_CASE(5.1.12, c + _d == c_d);
      }); /* Vector Addition */

      TEST_SECTION(Vector Subtraction, {
        TEST_CASE(5.2.1, a - b == a_b);
        TEST_CASE(5.2.2, a - c == a_c);
        TEST_CASE(5.2.3, a - d == a_d);
        TEST_CASE(5.2.4, a - _b == ab);
        TEST_CASE(5.2.5, a - _c == ac);
        TEST_CASE(5.2.6, a - _d == ad);

        TEST_CASE(5.2.7, b - c == b_c);
        TEST_CASE(5.2.8, b - d == b_d);
        TEST_CASE(5.2.9, b - _c == bc);
        TEST_CASE(5.2.10, b - _d == bd);

        TEST_CASE(5.2.11, c - d == c_d);
        TEST_CASE(5.2.12, c - _d == cd);
      }); /* Vector Subtraction */

      TEST_SECTION(Scalar Product, {
        TEST_CASE(5.3.1, 0 * a == _0);
        TEST_CASE(5.3.2, a * 0 == _0);
        TEST_CASE(5.3.3, 5 * a == _5a);
        TEST_CASE(5.3.4, a * 5 == _5a);
        TEST_CASE(5.3.5, -5 * a == __5a);
        TEST_CASE(5.3.6, a * -5 == __5a);

        TEST_CASE(5.3.7, 0 * c == _0);
        TEST_CASE(5.3.8, c * 0 == _0);
        TEST_CASE(5.3.9, 5 * c == _5c);
        TEST_CASE(5.3.10, c * 5 == _5c);

        TEST_CASE(5.3.11, -5 * c == __5c);
        TEST_CASE(5.3.12, c * -5 == __5c);
      }); /* Scalar Product */

      TEST_SECTION(Dot Product, {
        TEST_CASE(5.4.1, a * b == adb);
        TEST_CASE(5.4.2, a * c == adc);
        TEST_CASE(5.4.3, a * d == add);
        TEST_CASE(5.4.4, a * _b == -adb);
        TEST_CASE(5.4.5, a * _c == -adc);
        TEST_CASE(5.4.6, a * _d == -add);

        TEST_CASE(5.4.7, b * c == bdc);
        TEST_CASE(5.4.8, b * d == bdd);
        TEST_CASE(5.4.9, b * _c == -bdc);
        TEST_CASE(5.4.10, b * _d == -bdd);

        TEST_CASE(5.4.11, c * d == cdd);
        TEST_CASE(5.4.12, c * _d == -cdd);
      }); /* Dot Product */

    }); /* Vector Arithmetic */
  }); /* Vector16 */

}/* setup() */

#endif
