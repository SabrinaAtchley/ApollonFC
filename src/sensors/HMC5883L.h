#ifndef APOLLON_FC_SRC_SENSORS_HMC5883L_H
#define APOLLON_FC_SRC_SENSORS_HMC5883L_H

#include "../sensor.h"

enum : byte {
  HMC5883L_ADDRESS = 0x1E,
  HMC5883L_MODE = 0x01, // 1-bit mode: 0 continuous measurement, 1 takes a new measurement only after the old one was read
  HMC5883L_HIGH_SPEED = 0x00, // Set to 0x01 to use high speed I2C
  HMC5883L_REGISTER_CFGA = 0X00,
  HMC5883L_REGISTER_CFGB = 0X01,
  HMC5883L_REGISTER_MODE = 0X02,
  HMC5883L_REGISTER_X_MSB = 0X03,
  HMC5883L_REGISTER_X_LSB = 0X04,
  HMC5883L_REGISTER_Z_MSB = 0X05,
  HMC5883L_REGISTER_Z_LSB = 0X06,
  HMC5883L_REGISTER_Y_MSB = 0X07,
  HMC5883L_REGISTER_Y_LSB = 0X08,
  HMC5883L_REGISTER_STATUS = 0X09,
  HMC5883L_REGISTER_IDA = 0X0A,
  HMC5883L_REGISTER_IDB = 0X0B,
  HMC5883L_REGISTER_IDC = 0X0C,
  HMC5883L_REGISTER_TEMP_MSB = 0X31,
  HMC5883L_REGISTER_TEMP_LSB = 0X32,
};



class Sensor_HMC5883L : I2C_Sensor {
private:
  byte lock; // Status bit 1, write locks output registers
  byte ready; // Status bit 0, high when measurements are ready
  byte gain; // 3 bits, Valid values from 0x00 - 0x07. Higher gain # decreases actual gain and increases signal range
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
  Sensor_HMC5883L(byte _gain = 0x01, byte oss = 0x00, byte outRate = 0x04, byte bias = 0x00);

  void setGain(const byte g, const bool closeConnection);

  bool update();
};
constexpr int16_t Sensor_HMC5883L::gainLSB_Gauss[]; // Fixed undefined reference error



/* Implementation */

Sensor_HMC5883L::Sensor_HMC5883L(
    byte _gain = 0x01,
    byte oss = 0x00,
    byte outRate = 0x04,
    byte bias = 0x00
) : I2C_Sensor(HMC5883L_ADDRESS) {
  byte id[3];

  // Check ID = H43
  selectRegister(HMC5883L_REGISTER_IDA, connection.keepAlive);
  readIntoArray(id, 3, connection.close);
  if (id[0] != 0x48 || id[1] != 0x34 || id[2] != 0x33)
    ERROR(3); // I2C configuration error

  // Restrict configuration variables to correct bitsize
  oss &= 0x03; // 2 bits
  outRate &= 0x07; // 3 bits
  bias &= 0x03; // 2 bits

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
  write8(HMC5883L_REGISTER_MODE, (HMC5883L_HIGH_SPEED << 7) | HMC5883L_MODE, closeConnection);
}

void Sensor_HMC5883L::setGain(const byte g, const bool closeConnection) {
  gain = min(g, 0x07);
  write8(HMC5883L_REGISTER_CFGB, gain << 5, closeConnection);
}

bool Sensor_HMC5883L::update() {
  int8_t data[6]; // stores x<15:0>, z<15:0>, y<15:0>
  int16_t data_x, data_y, data_z;

  readStatus(connection.keepAlive);
  if (ready) {
    // Read magnetometer data
    selectRegister(HMC5883L_REGISTER_X_MSB, connection.keepAlive);
    readIntoArray(data, 6, connection.keepAlive);
    startMeasurement(connection.close);
    data_x = ((int16_t) data[0] << 8) | data[1];
    data_z = ((int16_t) data[2] << 8) | data[3];
    data_y = ((int16_t) data[4] << 8) | data[5];

    // If any value is -4096, it has overflowed
    if (data_x == -4096 || data_y == -4096 || data_z == -4096) {
      Serial.println("WARNING HMC5883L: integer overflow detected (magnetic field outside sensor range). Try a higher gain value if this problem persists.");
      return false;
    }

    // Convert from count (LSB) to Gauss measurements
    x = ( (int32_t) data_x * 1000) / gainLSB_Gauss[gain];
    z = ( (int32_t) data_z * 1000) / gainLSB_Gauss[gain];
    y = ( (int32_t) data_y * 1000) / gainLSB_Gauss[gain];

    return true;
  }
  return false;
}

#endif
