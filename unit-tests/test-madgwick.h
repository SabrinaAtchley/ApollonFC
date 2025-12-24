#ifndef APOLLON_FC_UNIT_TESTS_TEST_MADGWICK_H
#define APOLLON_FC_UNIT_TESTS_TEST_MADGWICK_H

#include "../src/sensors/MPU6050.h"
#include "../src/quaternion.h"
#include "../src/q16x16/q16x16.h"
#include "../src/madgwick.h"

void unitMadgwick() {
  Sensor_MPU6050 imu(false, false, false, MPU6050_GYRO_SCALE_500, MPU6050_ACCEL_SCALE_4G);
  unsigned long t = micros();
  float deltaT;

  Quaternion q;

  while (true) {
      while(!imu.update()) {}
      deltaT = (micros() - t) / 1000000.0;
      t = micros();

      // Convert accel measurements from 0.001g to g
      const Q16x16 thousand = itoq16x16(1000);
      Q16x16 ax = q16x16_div_s(itoq16x16(imu.accel.x), thousand);
      Q16x16 ay = q16x16_div_s(itoq16x16(imu.accel.y), thousand);
      Q16x16 az = q16x16_div_s(itoq16x16(imu.accel.z), thousand);

      Q16x16 gx, gy, gz;
      imu.getGyroRad(gx, gy, gz);

      q = madgwickUpdate(q,
        gx, gy, gz,
        ax, ay, az,
        ftoq16x16(deltaT)
      );

      float w, x, y, z;
      w = q16x16tof(q.w);
      x = q16x16tof(q.x);
      y = q16x16tof(q.y);
      z = q16x16tof(q.z);

      float roll = atan2(2 * (w*x + y*z), 1 - 2 * (x*x + y*y));
      float pitch = asin(2 * (w*y - z*x));
      float yaw = atan2(2 * (w*z + x*y), 1 - 2 * (y*y + z*z));

      // PRINT_QUAT(q);


      Serial.print("roll:");
      Serial.print(roll * 180 / 3.14159265);
      Serial.print(",pitch:");
      Serial.print(pitch * 180 / 3.14159265);
      Serial.print(",yaw:");
      Serial.println(yaw * 180 / 3.14159265);

    }
}

#endif /* APOLLON_FC_UNIT_TESTS_TEST_MADGWICK_H */
