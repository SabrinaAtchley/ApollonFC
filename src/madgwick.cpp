#include "madgwick.h"


Quaternion approxQuaternion(
  const Q16x16 roll, const Q16x16 pitch, const Q16x16 yaw
) {
  const Q16x16 half = 0x00008000;
  Quaternion q(
    0x00010000,
    q16x16_mul_s(roll, half),
    q16x16_mul_s(pitch, half),
    q16x16_mul_s(yaw, half)
  );

  return q;
}

// Normalizes acceleration and handles gating accelerometer
bool normalizeAccel(Q16x16 &ax, Q16x16 &ay, Q16x16 &az) {
  Q16x16 normSquared = q16x16_add_s(
    q16x16_add_s(
      q16x16_square_s(ax),
      q16x16_square_s(ay)
    ),
    q16x16_square_s(az)
  );

  // Accelerometer gating
  if (accel_gate_low > normSquared || normSquared > accel_gate_high) {
    ax = 0;
    ay = 0;
    az = 0;
    return false;
  }

  Q16x16 invNorm = q16x16_invsqrt(normSquared);
  ax = q16x16_mul_s(invNorm, ax);
  ay = q16x16_mul_s(invNorm, ay);
  az = q16x16_mul_s(invNorm, az);

  return true;
}

Quaternion calculateBetaS(
    const Quaternion &q,
    Q16x16 ax, Q16x16 ay, Q16x16 az
) {
  if (!normalizeAccel(ax, ay, az)) {
    Quaternion s(0, 0, 0, 0);
    return s;
  };
  // Invert accelerometer (measures specific force, not gravity direction)
  ax = q16x16_negate(ax);
  ay = q16x16_negate(ay);
  az = q16x16_negate(az);

  Quaternion d_g(0, 0, 0, 0x00010000); // Gravity in world frame
  // q* x <0, 0, 0, 1> x q
  Quaternion g_b = q; // Gravity in body frame
  g_b.conjugate();
  g_b *= d_g;
  g_b *= q;

  // Calculate objective function f
  Q16x16 fx = q16x16_sub_s(g_b.x, ax);
  Q16x16 fy = q16x16_sub_s(g_b.y, ay);
  Q16x16 fz = q16x16_sub_s(g_b.z, az);

  // Multiply f by the Jacobian to get the gradient of f
  // fx = 2 * (q.x*fy - q.y*fx)
  Q16x16 gradFw = q16x16_double_s(q16x16_sub_s(
    q16x16_mul_s(q.x, fy),
    q16x16_mul_s(q.y, fx)
  ));

  // 2 * (q.z*fx + q.w*fy - 2*q.x*fz)
  Q16x16 gradFx = q16x16_double_s(q16x16_sub_s(
    q16x16_add_s(
      q16x16_mul_s(q.z, fx),
      q16x16_mul_s(q.w, fy)
    ),
    q16x16_double_s(q16x16_mul_s(q.x, fz)) // 2 * q.x * fz
  ));

  // 2 * (q.z*fy - (q.w*fx + 2*q.y*fz))
  Q16x16 gradFy = q16x16_double_s(q16x16_sub_s(
    q16x16_mul_s(q.z, fy),
    q16x16_add_s(
      q16x16_mul_s(q.w, fx),
      q16x16_double_s(q16x16_mul_s(q.y, fz))
    )
  ));

  // 2 * (q.x*fx + q.y*fy)
  Q16x16 gradFz = q16x16_double_s(q16x16_add_s(
    q16x16_mul_s(q.x, fx),
    q16x16_mul_s(q.y, fy)
  ));


  // Normalize the gradient to get s

  const Q16x16 fNormSquared = q16x16_add_s(
    q16x16_add_s(
      q16x16_square_s(gradFw),
      q16x16_square_s(gradFx)
    ),
    q16x16_add_s(
      q16x16_square_s(gradFy),
      q16x16_square_s(gradFz)
    )
  );

  if (fNormSquared == 0) {
    Quaternion s(0, 0, 0, 0);
    return s;
  }

  const Q16x16 fScale = q16x16_mul_s(beta, q16x16_invsqrt(fNormSquared));

  Quaternion s(
    q16x16_mul_s(fScale, gradFw),
    q16x16_mul_s(fScale, gradFx),
    q16x16_mul_s(fScale, gradFy),
    q16x16_mul_s(fScale, gradFz)
  );

  return s;
}

Quaternion madgwickUpdate(
    const Quaternion &qPrev,
    const Q16x16 gx, const Q16x16 gy, const Q16x16 gz,
    Q16x16 ax, Q16x16 ay, Q16x16 az,
    const Q16x16 deltaT
) {

  // qDot = q/2 x <0, ω>
  Quaternion qDot(
    qPrev.w >> 1,
    qPrev.x >> 1,
    qPrev.y >> 1,
    qPrev.z >> 1
  );
  Quaternion gyroQ(0, gx, gy, gz);
  qDot *= gyroQ;

  Quaternion betaS = calculateBetaS(qPrev, ax, ay, az);
  qDot -= betaS;


  // Integrate q, q <- q + qDot * deltaT
  qDot.scale(deltaT);

  Quaternion q = Quaternion::add(qPrev, qDot);
  q.normalize();
  q.matchHemisphere(qPrev);


  return q;
}
