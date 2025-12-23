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
      Q16x16 ax, ay, az;
      float gx, gy, gz;
      ax = q16x16_div_s(itoq16x16(imu.accel.x), thousand);
      ay = q16x16_div_s(itoq16x16(imu.accel.y), thousand);
      az = q16x16_div_s(itoq16x16(imu.accel.z), thousand);

      // Convert gyro measurements from 0.001°/s to rad/s
      const float millidegToRad = 3.14159265 / 180.0 / 1000.0;
      // gx = q16x16_mul_s(itoq16x16(imu.gyro.x), millidegToRad);
      // gy = q16x16_mul_s(itoq16x16(imu.gyro.y), millidegToRad);
      // gz = q16x16_mul_s(itoq16x16(imu.gyro.z), millidegToRad);

      gx = imu.gyro.x * millidegToRad;
      gy = imu.gyro.y * millidegToRad;
      gz = imu.gyro.z * millidegToRad;

      q = madgwickUpdate(q,
        ftoq16x16(gx), ftoq16x16(gy), ftoq16x16(gz),
        ax, ay, az,
        ftoq16x16(deltaT)
      );
      Serial.print("deltaT:");
      Serial.print(deltaT * 1000);
      Serial.print(",");

      float w, x, y, z;
      w = q16x16tof(q.w);
      x = q16x16tof(q.x);
      y = q16x16tof(q.y);
      z = q16x16tof(q.z);

      float roll = atan2(2 * (w*x + y*z), 1 - 2 * (x*x + y*y));
      float pitch = asin(2 * (w*y - z*x));
      float yaw = atan2(2 * (w*z + x*y), 1 - 2 * (y*y + z*z));

      PRINT_QUAT(q);

      /*
      Serial.print("roll: ");
      Serial.print(roll * 180 / 3.14159265);
      Serial.print(", pitch: ");
      Serial.print(pitch * 180 / 3.14159265);
      Serial.print(", yaw: ");
      Serial.println(yaw * 180 / 3.14159265);
      */


      // Serial.print(gx);
      // Serial.print(", ");
      // Serial.print(gy);
      // Serial.println(gz);
      // Serial.print(", ");

    }
}

#endif /* APOLLON_FC_UNIT_TESTS_TEST_MADGWICK_H */
