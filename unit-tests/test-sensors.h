#ifndef APOLLON_FC_UNIT_TESTS_TEST_SENSORS_H
#define APOLLON_FC_UNIT_TESTS_TEST_SENSORS_H

#include "../src/sensors/BMP180.h"
#include "../src/sensors/HMC5883L.h"

/* Tests various sensor libraries
 * Options:
 * APOLLON_FC_UNIT_TESTS_TEST_SENSORS_BMP180
 * APOLLON_FC_UNIT_TESTS_TEST_SENSORS_HMC5883L
 */

#define APOLLON_FC_UNIT_TESTS_TEST_SENSORS_HMC5883L

void test_hmc5883l() {
  unsigned long t = millis(); // time of last update
  Sensor_HMC5883L hmc5883l(0x01, 0x03); // gain, oversampling, outRate, bias
  float heading;

  Serial.println("HMC5883L");
  while(true) {
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
  while(true) {
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
}

#endif
