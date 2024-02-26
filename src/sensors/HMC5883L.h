#ifndef APOLLON_FC_SRC_SENSORS_HMC5883L_H
#define APOLLON_FC_SRC_SENSORS_HMC5883L_H

#include "../sensor.h"

/* HMC5883L Sensor
 * Interfaces: I2C
 * Measures: 3-axis magnetic field, internal temperature
 * Estimates: n/a
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
 */

enum : byte {
  HMC5883L_ADDRESS = 0x1E,
  HMC5883L_HIGH_SPEED = 0x00, // Set to 0x01 to use high speed I2C
  HMC5883L_REGISTER_CFGA = 0x00,
  HMC5883L_REGISTER_CFGB = 0x01,
  HMC5883L_REGISTER_MODE = 0x02,
  HMC5883L_REGISTER_X_MSB = 0x03,
  HMC5883L_REGISTER_X_LSB = 0x04,
  HMC5883L_REGISTER_Z_MSB = 0x05,
  HMC5883L_REGISTER_Z_LSB = 0x06,
  HMC5883L_REGISTER_Y_MSB = 0x07,
  HMC5883L_REGISTER_Y_LSB = 0x08,
  HMC5883L_REGISTER_STATUS = 0x09,
  HMC5883L_REGISTER_IDA = 0x0A,
  HMC5883L_REGISTER_IDB = 0x0B,
  HMC5883L_REGISTER_IDC = 0x0C,
  HMC5883L_REGISTER_TEMP_MSB = 0x31,
  HMC5883L_REGISTER_TEMP_LSB = 0x32,
};

typedef enum : byte { // 1-bit mode
  HMC5883L_MODE_CONTINUOUS = 0b0,
  HMC5883L_MODE_SINGLE = 0b1,
} hmc5883l_mode_t;

typedef enum : byte { // 3-bit gain value
  HMC5883L_GAIN_0 = 0x0,
  HMC5883L_GAIN_1 = 0x1,
  HMC5883L_GAIN_2 = 0x2,
  HMC5883L_GAIN_3 = 0x3,
  HMC5883L_GAIN_4 = 0x4,
  HMC5883L_GAIN_5 = 0x5,
  HMC5883L_GAIN_6 = 0x6,
  HMC5883L_GAIN_7 = 0x7,
} hmc5883l_gain_t;

typedef enum : byte { // 2-bits
  HMC5883L_OSS_0 = 0x0,
  HMC5883L_OSS_1 = 0x1,
  HMC5883L_OSS_2 = 0x2,
  HMC5883L_OSS_3 = 0x3,
} hmc5883l_oss_t;

typedef enum : byte { // 3-bits
  HMC5883L_OUTRATE_0 = 0x0,
  HMC5883L_OUTRATE_1 = 0x1,
  HMC5883L_OUTRATE_2 = 0x2,
  HMC5883L_OUTRATE_3 = 0x3,
  HMC5883L_OUTRATE_4 = 0x4,
  HMC5883L_OUTRATE_5 = 0x5,
  HMC5883L_OUTRATE_6 = 0x6,
  HMC5883L_OUTRATE_7 = 0x7,
} hmc5883l_outrate_t;

typedef enum : byte { // 2-bits, excluding 0b11
  HMC5883L_BIAS_0 = 0x0,
  HMC5883L_BIAS_1 = 0x1,
  HMC5883L_BIAS_2 = 0x2,
} hmc5883l_bias_t;


class Sensor_HMC5883L : I2C_Sensor {
private:
  byte lock; // Status bit 1, write locks output registers
  byte ready; // Status bit 0, high when measurements are ready
  hmc5883l_gain_t gain; // Higher gain # decreases actual gain and increases signal range
  const hmc5883l_mode_t mode;
  static constexpr int16_t gainLSB_Gauss[] = {1370, 1090, 820, 660, 440, 390, 330, 230};
public:
  int16_t x = 0, y = 0, z = 0; // magnetometer values in microGauss

private:
  /* Read status register for lock and ready bits */
  void readStatus(const bool closeConnection);

  /* Sets mode register, which begins measurement
   * For continuous-mode operation, this only needs to be called once
   * For single-measurement operation, this needs to be called after data is read
   */
  void startMeasurement(const bool closeConnection);

public:
  Sensor_HMC5883L(
    const hmc5883l_mode_t _mode = HMC5883L_MODE_SINGLE,
    const hmc5883l_gain_t _gain = HMC5883L_GAIN_1,
    const hmc5883l_oss_t oss = HMC5883L_OSS_0,
    const hmc5883l_outrate_t outRate = HMC5883L_OUTRATE_4,
    const hmc5883l_bias_t bias = HMC5883L_BIAS_0
  );

  void setGain(const hmc5883l_gain_t g, const bool closeConnection);

  bool update();
};
constexpr int16_t Sensor_HMC5883L::gainLSB_Gauss[]; // Fixed undefined reference error



/* Implementation */

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
    readIntoArray(data, 3, connection.keepAlive);
    startMeasurement(connection.close); // Start next measurement cycle

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

#endif
