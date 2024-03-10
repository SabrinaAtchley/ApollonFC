#include "sensor.h"
#include <Wire.h>

/* Abstract class I2C_Sensor */
constexpr Connection I2C_Sensor::connection;

I2C_Sensor::I2C_Sensor(const byte addr) : address(addr) {
  Wire.begin(); // Enable I2C
}

void I2C_Sensor::selectRegister(const byte reg, const bool closeConnection) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(closeConnection);
}

void I2C_Sensor::write8(const byte reg, const byte value, const bool closeConnection) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(closeConnection);
}
