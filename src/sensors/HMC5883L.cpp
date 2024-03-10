#include "HMC5883L.h"
#include "../error.h"

constexpr int16_t Sensor_HMC5883L::gainLSB_Gauss[];

Sensor_HMC5883L::Sensor_HMC5883L(
    const hmc5883l_mode_t _mode = HMC5883L_MODE_SINGLE,
    const hmc5883l_gain_t _gain = HMC5883L_GAIN_1,
    const hmc5883l_oss_t oss = HMC5883L_OSS_0,
    const hmc5883l_outrate_t outRate = HMC5883L_OUTRATE_4,
    const hmc5883l_bias_t bias = HMC5883L_BIAS_0
) : I2C_Sensor(HMC5883L_ADDRESS), mode(_mode) {
  byte id[3];

  // Check ID = H43
  selectRegister(HMC5883L_REGISTER_IDA, connection.keepAlive);
  readIntoArray(id, 3, connection.close);
  if (id[0] != 0x48 || id[1] != 0x34 || id[2] != 0x33)
    ERROR(3); // I2C configuration error

  // Configure device
  setGain(_gain, connection.keepAlive);
  write8(HMC5883L_REGISTER_CFGA, (oss << 5) | (outRate << 3) | bias, connection.keepAlive);
  startMeasurement(connection.close);
}

void Sensor_HMC5883L::readStatus(const bool closeConnection) {
  byte status;
  selectRegister(HMC5883L_REGISTER_STATUS, connection.keepAlive);
  read8(status, closeConnection);
  lock = status & 0x02;
  ready = status & 0x01;
}

void Sensor_HMC5883L::startMeasurement(const bool closeConnection) {
  write8(HMC5883L_REGISTER_MODE, (HMC5883L_HIGH_SPEED << 7) | mode, closeConnection);
}

void Sensor_HMC5883L::setGain(const hmc5883l_gain_t g, const bool closeConnection) {
  gain = g;
  write8(HMC5883L_REGISTER_CFGB, gain << 5, closeConnection);
}

bool Sensor_HMC5883L::update() {
  int16_t data[3];

  readStatus(connection.close); // Close connection in case not ready
  if (ready) {
    // Read magnetometer data
    selectRegister(HMC5883L_REGISTER_X_MSB, connection.keepAlive);
    switch (mode) {
      case HMC5883L_MODE_CONTINUOUS:
        readIntoArray(data, 3, connection.close);
        break;

      case HMC5883L_MODE_SINGLE:
        readIntoArray(data, 3, connection.keepAlive);
        startMeasurement(connection.close); // Start next measurement cycle
    }

    // If any value is -4096, it has overflowed
    if (data[0] == -4096 || data[1] == -4096 || data[2] == -4096) {
      Serial.println("WARNING HMC5883L: integer overflow detected (magnetic field outside sensor range). Try a higher gain value if this problem persists.");
      return false;
    }

    // Convert from count (LSB) to microGauss measurements
    x = ( (int32_t) data[0] * 1000) / gainLSB_Gauss[gain];
    z = ( (int32_t) data[1] * 1000) / gainLSB_Gauss[gain];
    y = ( (int32_t) data[2] * 1000) / gainLSB_Gauss[gain];

    return true;
  }
  return false;
}
