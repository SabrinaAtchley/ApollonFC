#include "sensor.h"
#include <Wire.h>

/* Abstract class I2C_Sensor */
constexpr Connection I2C_Sensor::connection;

I2C_Sensor::I2C_Sensor(const byte addr) : address(addr) {
  Wire.begin(); // Enable I2C
}


void I2C_Sensor::beginTransmission(const byte reg = 0x00) {
  Wire.beginTransmission(address);

  if (reg != 0x00) {
    Wire.write(reg);
  }
}

void I2C_Sensor::endTransmission(const bool closeConnection) {
  Wire.endTransmission(closeConnection);
}

void I2C_Sensor::changeRegister(const byte reg) {
  Wire.endTransmission(connection.keepAlive); // send repeat start
  Wire.beginTransmission(address);
  Wire.write(reg);
}

void I2C_Sensor::request(const byte bytes) {
  Wire.endTransmission(connection.keepAlive);
  Wire.requestFrom(address, bytes);
}

void I2C_Sensor::write8(const byte value) {
  Wire.write(value);
}
