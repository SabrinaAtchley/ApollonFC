#ifndef APOLLON_FC_UNIT_TESTS_TEST_SENSORS_H
#define APOLLON_FC_UNIT_TESTS_TEST_SENSORS_H

#include "../src/sensors/BMP180.h"
#include "../src/sensors/HMC5883L.h"
#include "../src/sensors/MPU6050.h"

/* Tests various sensor libraries
 * Options:
 * APOLLON_FC_UNIT_TESTS_TEST_SENSORS_BMP180
 * APOLLON_FC_UNIT_TESTS_TEST_SENSORS_HMC5883L
 * APOLLON_FC_UNIT_TESTS_TEST_SENSORS_MPU6050
 */

#define APOLLON_FC_UNIT_TESTS_TEST_SENSORS_MPU6050

void test_mpu6050() {
  unsigned long t = millis(); // time of last update
  Sensor_MPU6050 mpu6050(false, false, false, MPU6050_GYRO_SCALE_500, MPU6050_ACCEL_SCALE_4G);

  Serial.println("MPU6050");
  Serial.println("Initiating self-test");
  byte selfTestResult = mpu6050.selfTest();

  if (selfTestResult ^ 0x3F) { // test failed
    Serial.print("Self-test failed, result: ");
    Serial.println(selfTestResult, BIN);
    return;
  }
  Serial.println("Self test passed");

  while (true) {
    if (mpu6050.update()) {
      Serial.print("Δt:");
      Serial.print(millis() - t);
      t = millis();
      Serial.print(",accel_x:");
      Serial.print(mpu6050.accel.x / 1000.0);
      Serial.print(",accel_y:");
      Serial.print(mpu6050.accel.y / 1000.0);
      Serial.print(",accel_z:");
      Serial.print(mpu6050.accel.z / 1000.0);
      Serial.print(",gyro_x:");
      Serial.print(mpu6050.gyro.x / 1000.0);
      Serial.print(",gyro_y:");
      Serial.print(mpu6050.gyro.y / 1000.0);
      Serial.print(",gyro_z:");
      Serial.print(mpu6050.gyro.z / 1000.0);
      Serial.print(",temp:");
      Serial.println(mpu6050.temp / 10.0);
    }
  }
}

void test_hmc5883l() {
  unsigned long t = millis(); // time of last update
  Sensor_HMC5883L hmc5883l(HMC5883L_MODE_SINGLE, HMC5883L_GAIN_1, HMC5883L_OSS_3); // mode, gain, oversampling, outRate, bias
  float heading;

  Serial.println("HMC5883L");
  while (true) {
    if (hmc5883l.update()) { // If data is available
      Serial.print("Δt:");
      Serial.print(millis() - t);
      t = millis();
      Serial.print(",x:");
      Serial.print(hmc5883l.x);
      Serial.print(",y:");
      Serial.print(hmc5883l.y);
      Serial.print(",z:");
      Serial.print(hmc5883l.z);
      Serial.print(",heading:");
      heading = atan2(hmc5883l.x, hmc5883l.y);
      heading = heading < 0 ? heading + 2 * PI : heading;
      Serial.println(heading * 180 / PI);
    }
  }
}

void test_bmp180() {
  unsigned long t = millis(); // time of last update
  Sensor_BMP180 bmp180(BMP180_MODE_ULTRA_HIGH_RES, 102070);

  Serial.println("BMP180");
  while (true) {
    if (bmp180.update()) { // If data is available
      Serial.print("Δt:");
      Serial.print(millis() - t);
      t = millis();
      Serial.print(",UT:");
      Serial.print(bmp180.ut);
      Serial.print(",UP:");
      Serial.print(bmp180.up);
      Serial.print(",temp:");
      Serial.print(bmp180.temperature);
      Serial.print(",pressure:");
      Serial.print(bmp180.pressure);
      Serial.print(",altitude:");
      Serial.println(bmp180.altitude);
    }
  }
}

void unitSensors() {
  #ifdef APOLLON_FC_UNIT_TESTS_TEST_SENSORS_BMP180
    test_bmp180();
  #endif
  #ifdef APOLLON_FC_UNIT_TESTS_TEST_SENSORS_HMC5883L
    test_hmc5883l();
  #endif
  #ifdef APOLLON_FC_UNIT_TESTS_TEST_SENSORS_MPU6050
    test_mpu6050();
  #endif
}

#endif
