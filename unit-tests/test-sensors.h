#ifndef APOLLON_FC_UNIT_TESTS_TEST_SENSORS_H
#define APOLLON_FC_UNIT_TESTS_TEST_SENSORS_H

#include "../src/sensors/BMP180.h"

void unitSensors() {
  Sensor_BMP180 bmp180(BMP180_MODE_ULTRA_HIGH_RES, 102070);
  unsigned long t = millis(); // time of last update

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

#endif
