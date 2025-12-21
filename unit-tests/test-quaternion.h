#ifndef APOLLON_FC_UNIT_TESTS_TEST_QUATERNION_H
#define APOLLON_FC_UNIT_TESTS_TEST_QUATERNION_H

#include "test-framework.h"
#include "../src/quaternion.h"

// NOTE: Auto-generated tests. Eventually these should probably be replaced with
//       more readable handwritten ones

// Handy Q16.16 constants
static constexpr Q16x16 Q0      = 0x00000000;
static constexpr Q16x16 QHALF   = 0x00008000;  // 0.5
static constexpr Q16x16 Q1      = 0x00010000;  // 1.0
static constexpr Q16x16 Q2      = 0x00020000;  // 2.0
static constexpr Q16x16 QN1     = (Q16x16)0xFFFF0000;  // -1.0
static constexpr Q16x16 QN2     = (Q16x16)0xFFFE0000;  // -2.0

static constexpr Q16x16 Q16_MIN = (Q16x16)0x80000000;
static constexpr Q16x16 Q16_MAX = (Q16x16)0x7FFFFFFF;

// Helper: exact component equality (no tolerance)
static inline bool quatEq(const Quaternion& a, const Quaternion& b) {
  return a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z;
}

// Helper: "represents same rotation" check: q == p OR q == -p
static inline bool quatEqUpToSign(const Quaternion& a, const Quaternion& b) {
  if (quatEq(a, b)) return true;
  Quaternion nb = b;
  nb.flipSign();
  return quatEq(a, nb);
}

void unitQuaternion() {
  TEST_UNIT(Quaternion, {

    TEST_SECTION(Flip Sign, {
      TEST_CASE(1.1, [](){
        Quaternion q(Q1, QHALF, (Q16x16)0x0000C000 /*0.75*/, (Q16x16)0xFFFF4000 /*-0.75*/);
        Quaternion orig = q;
        q.flipSign();
        Quaternion expect(
          q16x16_negate(orig.w),
          q16x16_negate(orig.x),
          q16x16_negate(orig.y),
          q16x16_negate(orig.z)
        );
        return quatEq(q, expect);
      }());

      TEST_CASE(1.2, [](){
        Quaternion q(Q1, QHALF, Q0, QN1);
        Quaternion orig = q;
        q.flipSign().flipSign();
        return quatEq(q, orig);
      }());

      TEST_CASE(1.3, [](){
        Quaternion q(Q16_MIN, Q0, Q0, Q0);
        q.flipSign(); // MIN should clamp to MAX in your q16x16_negate()
        Quaternion expect(Q16_MAX, Q0, Q0, Q0);
        return quatEq(q, expect);
      }());
    }); /* Flip Sign */

    TEST_SECTION(Match Hemisphere, {
      TEST_CASE(2.1, [](){
        // Same quaternion => dot positive => no flip
        Quaternion prev(Q1, Q0, Q0, Q0);
        Quaternion q(Q1, Q0, Q0, Q0);
        q.matchHemisphere(prev);
        return quatEq(q, Quaternion(Q1, Q0, Q0, Q0));
      }());

      TEST_CASE(2.2, [](){
        // Opposite sign => dot negative => flip to match prev
        Quaternion prev(Q1, QHALF, Q0, Q0);
        Quaternion q = prev;
        q.flipSign();               // now q == -prev
        q.matchHemisphere(prev);       // should flip back
        return quatEq(q, prev);
      }());

      TEST_CASE(2.3, [](){
        // Orthogonal (dot == 0) => should NOT flip (since dot < 0 is false)
        Quaternion prev(Q1, Q0, Q0, Q0);
        Quaternion q(Q0, Q1, Q0, Q0);
        q.matchHemisphere(prev);
        return quatEq(q, Quaternion(Q0, Q1, Q0, Q0));
      }());
    }); /* Match Hemisphere */

    TEST_SECTION(Conjugate, {
      TEST_CASE(3.1, [](){
        Quaternion q(Q1, QHALF, (Q16x16)0x0000C000 /*0.75*/, (Q16x16)0xFFFF4000 /*-0.75*/);
        Quaternion orig = q;
        q.conjugate();
        Quaternion expect(orig.w,
          q16x16_negate(orig.x),
          q16x16_negate(orig.y),
          q16x16_negate(orig.z)
        );
        return quatEq(q, expect);
      }());

      TEST_CASE(3.2, [](){
        Quaternion q(Q1, QHALF, QN1, Q2);
        Quaternion orig = q;
        q.conjugate().conjugate();
        return quatEq(q, orig);
      }());
    }); /* Conjugate */

    TEST_SECTION(Normalize, {
      TEST_CASE(4.1, [](){
        // Zero quaternion normalizes to identity per your implementation
        Quaternion q(Q0, Q0, Q0, Q0);
        q.normalize();
        return quatEq(q, Quaternion(Q1, Q0, Q0, Q0));
      }());

      TEST_CASE(4.2, [](){
        // Identity stays identity
        Quaternion q(Q1, Q0, Q0, Q0);
        q.normalize();
        return quatEq(q, Quaternion(Q1, Q0, Q0, Q0));
      }());

      TEST_CASE(4.3, [](){
        // Simple scale: (2,0,0,0) -> (1,0,0,0)
        Quaternion q(Q2, Q0, Q0, Q0);
        q.normalize();
        return quatEq(q, Quaternion(Q1, Q0, Q0, Q0));
      }());

      TEST_CASE(4.4, [](){
        // After normalize, dot(q,q) should be ~ 1.0 in Q16.16.
        // We allow small error due to invsqrt approximation.
        Quaternion q((Q16x16)0x00018000 /*1.5*/, (Q16x16)0xFFFF8000 /*-0.5*/, QHALF, Q0);
        q.normalize();
        Q16x16 d = Quaternion::dot(q, q);
        // Accept within about +/- 0.01
        const Q16x16 tol = (Q16x16)0x0000028F; // ≈ 0.01 * 65536 = 655
        return (d >= (Q1 - tol)) && (d <= (Q1 + tol));
      }());
    }); /* Normalize */

    TEST_SECTION(Product Assignment, {
      TEST_CASE(5.1, [](){
        // q * identity == q
        Quaternion q((Q16x16)0x0000C000 /*0.75*/, QHALF, Q0, QHALF);
        Quaternion id(Q1, Q0, Q0, Q0);
        Quaternion orig = q;
        q *= id;
        return quatEq(q, orig);
      }());

      TEST_CASE(5.2, [](){
        // identity * q == q
        Quaternion q((Q16x16)0x0000C000 /*0.75*/, QHALF, Q0, QHALF);
        Quaternion id(Q1, Q0, Q0, Q0);
        Quaternion res = id;
        res *= q;
        return quatEq(res, q);
      }());

      TEST_CASE(5.3, [](){
        // q * q_conj should yield (||q||^2, 0,0,0). If q is unit, -> identity.
        Quaternion q((Q16x16)0x0000C000 /*0.75*/, QHALF, Q0, QHALF);
        q.normalize();
        Quaternion qc = q;
        qc.conjugate();
        Quaternion prod = q;
        prod *= qc;
        // Allow tiny error on scalar due to normalize approximation.
        const Q16x16 tol = (Q16x16)0x0000028F; // ~0.01
        bool scalar_ok = (prod.w >= (Q1 - tol)) && (prod.w <= (Q1 + tol));
        bool vec_ok = (prod.x == 0) && (prod.y == 0) && (prod.z == 0);
        return scalar_ok && vec_ok;
      }());
    }); /* Product Assignment */

    TEST_SECTION(Sum Assignment, {
      TEST_CASE(6.1, [](){
        Quaternion q(Q1, Q0, QHALF, QN1);
        Quaternion rhs(QHALF, Q1, QHALF, Q1);
        Quaternion expect(
          q16x16_add_s(q.w, rhs.w),
          q16x16_add_s(q.x, rhs.x),
          q16x16_add_s(q.y, rhs.y),
          q16x16_add_s(q.z, rhs.z)
        );
        q += rhs;
        return quatEq(q, expect);
      }());

      TEST_CASE(6.2, [](){
        // adding zero changes nothing
        Quaternion q(Q1, QHALF, Q0, QN2);
        Quaternion orig = q;
        Quaternion zero(Q0, Q0, Q0, Q0);
        q += zero;
        return quatEq(q, orig);
      }());
    }); /* Sum Assignment */

    TEST_SECTION(Difference Assignment, {
      TEST_CASE(7.1, [](){
        Quaternion q(Q1, Q0, QHALF, QN1);
        Quaternion rhs(QHALF, Q1, QHALF, Q1);
        Quaternion expect(
          q16x16_sub_s(q.w, rhs.w),
          q16x16_sub_s(q.x, rhs.x),
          q16x16_sub_s(q.y, rhs.y),
          q16x16_sub_s(q.z, rhs.z)
        );
        q -= rhs;
        return quatEq(q, expect);
      }());

      TEST_CASE(7.2, [](){
        // subtracting zero changes nothing
        Quaternion q(Q1, QHALF, Q0, QN2);
        Quaternion orig = q;
        Quaternion zero(Q0, Q0, Q0, Q0);
        q -= zero;
        return quatEq(q, orig);
      }());
    }); /* Difference Assignment */

    TEST_SECTION(Mul, {
      TEST_CASE(8.1, [](){
        Quaternion a((Q16x16)0x0000C000 /*0.75*/, QHALF, Q0, QHALF);
        Quaternion id(Q1, Q0, Q0, Q0);
        Quaternion r = Quaternion::mul(a, id);
        return quatEq(r, a);
      }());

      TEST_CASE(8.2, [](){
        // mul should not mutate its first argument by contract (it takes by value)
        Quaternion a(Q1, QHALF, Q0, Q0);
        Quaternion b(QHALF, Q0, QHALF, Q0);
        Quaternion orig = a;
        (void)Quaternion::mul(a, b);
        return quatEq(a, orig);
      }());
    }); /* Mul */

    TEST_SECTION(Add, {
      TEST_CASE(9.1, [](){
        Quaternion a(Q1, Q0, QHALF, QN1);
        Quaternion b(QHALF, Q1, QHALF, Q1);
        Quaternion r = Quaternion::add(a, b);
        Quaternion expect(
          q16x16_add_s(a.w, b.w),
          q16x16_add_s(a.x, b.x),
          q16x16_add_s(a.y, b.y),
          q16x16_add_s(a.z, b.z)
        );
        return quatEq(r, expect);
      }());

      TEST_CASE(9.2, [](){
        Quaternion a(Q1, QHALF, Q0, QN2);
        Quaternion zero(Q0, Q0, Q0, Q0);
        Quaternion r = Quaternion::add(a, zero);
        return quatEq(r, a);
      }());
    }); /* Add */

    TEST_SECTION(Sub, {
      TEST_CASE(10.1, [](){
        Quaternion a(Q1, Q0, QHALF, QN1);
        Quaternion b(QHALF, Q1, QHALF, Q1);
        Quaternion r = Quaternion::sub(a, b);
        Quaternion expect(
          q16x16_sub_s(a.w, b.w),
          q16x16_sub_s(a.x, b.x),
          q16x16_sub_s(a.y, b.y),
          q16x16_sub_s(a.z, b.z)
        );
        return quatEq(r, expect);
      }());

      TEST_CASE(10.2, [](){
        Quaternion a(Q1, QHALF, Q0, QN2);
        Quaternion r = Quaternion::sub(a, a);
        return quatEq(r, Quaternion(Q0, Q0, Q0, Q0));
      }());
    }); /* Sub */

    TEST_SECTION(Dot, {
      TEST_CASE(11.1, [](){
        Quaternion a(Q1, Q0, Q0, Q0);
        Quaternion b(Q1, Q0, Q0, Q0);
        return Quaternion::dot(a, b) == Q1;
      }());

      TEST_CASE(11.2, [](){
        // dot(q, -q) == -dot(q,q)
        Quaternion q((Q16x16)0x0000C000 /*0.75*/, QHALF, Q0, QHALF);
        Quaternion nq = q;
        nq.flipSign();
        Q16x16 d1 = Quaternion::dot(q, q);
        Q16x16 d2 = Quaternion::dot(q, nq);
        return d2 == q16x16_negate(d1);
      }());

      TEST_CASE(11.3, [](){
        // orthogonal basis: dot([1,0,0,0], [0,1,0,0]) == 0
        Quaternion a(Q1, Q0, Q0, Q0);
        Quaternion b(Q0, Q1, Q0, Q0);
        return Quaternion::dot(a, b) == Q0;
      }());
    }); /* Dot */

  }); /* Quaternion */
} /* unitQuaternion() */

#endif /* APOLLON_FC_UNIT_TESTS_TEST_QUATERNION_H */
