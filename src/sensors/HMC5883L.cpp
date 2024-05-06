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

  // Check ID = H43
  byte id[3];
  beginTransmission(HMC5883L_REGISTER_IDA); {
    request(3);
    read8(id[0]);
    read8(id[1]);
    read8(id[2]);
  } endTransmission(connection.keepAlive);
  if (id[0] != 0x48 || id[1] != 0x34 || id[2] != 0x33) {
    ERROR(3); // I2C configuration error
  }

  // Configure device
  beginTransmission(HMC5883L_REGISTER_CFGA); {
    write8((oss << 5) | (outRate << 3) | bias); // CFGA
    write8(gain << 5); // CFGB
    write8((HMC5883L_HIGH_SPEED << 7) | mode); // MODE
  } endTransmission(connection.close);
}

void Sensor_HMC5883L::setGain(const hmc5883l_gain_t g, const bool closeConnection) {
  gain = g;
  beginTransmission(HMC5883L_REGISTER_CFGB); {
    write8(gain << 5);
  } endTransmission(closeConnection);
}

bool Sensor_HMC5883L::update() {
  // Read device status
  beginTransmission(HMC5883L_REGISTER_STATUS); {
    byte status;
    request(1);
    read8(status);
    lock = status & 0x02;
    ready = status & 0x01;
  } endTransmission(ready ? connection.keepAlive : connection.close);

  if (!ready) { // Exit if not ready
    return false;
  }

  // Read magnetometer data
  beginTransmission(HMC5883L_REGISTER_X_MSB); {
    request(6);
    read16(x);
    read16(y);
    read16(z);
    if (mode == HMC5883L_MODE_SINGLE) { // start next measurement cycle
      changeRegister(HMC5883L_REGISTER_MODE);
      write8((HMC5883L_HIGH_SPEED << 7) | mode);
    }
  } endTransmission(connection.close);

  // If any value is -4096, it has overflowed
  if (x == -4096 || y == -4096 || z == -4096) {
    Serial.println("WARNING HMC5883L: integer overflow detected (magnetic field outside sensor range). Try a higher gain value if this problem persists.");
    return false;
  }

  // Convert from count (LSB) to microGauss measurements
  x = ( (int32_t) x * 1000) / gainLSB_Gauss[gain];
  z = ( (int32_t) y * 1000) / gainLSB_Gauss[gain];
  y = ( (int32_t) z * 1000) / gainLSB_Gauss[gain];

  return true;
}
