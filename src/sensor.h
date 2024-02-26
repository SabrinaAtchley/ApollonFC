#ifndef APOLLON_FC_SRC_SENSOR_H
#define APOLLON_FC_SRC_SENSOR_H

#include <Wire.h>

// Abstract
class Sensor {

public:
  /* Sensor update function
   * To be called once every loop, handles sensor value updating in a step-based
   * asynchronous process.
   * Outputs true once all new data is fully ready,
   * Outputs false otherwise
   */
  virtual bool update() = 0;
};


// Abstract
class I2C_Sensor : Sensor {
private:
  const byte address; // Device I2C address

  void selectRegister(const byte reg, const bool closeConnection = false) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission(closeConnection);
  }

protected:
  I2C_Sensor(const byte addr) : address(addr) {
    // Enable I2C
    Wire.begin();
  };

  void write8(const byte reg, const byte value, const bool closeConnection = true) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(closeConnection);
  }

  void read8(const byte reg, byte &value, const bool closeConnection = true) {
    selectRegister(reg, false);

    Wire.requestFrom(address, (byte) 1);
    value = Wire.read();
    Wire.endTransmission(closeConnection);
  }


  void read16(const byte reg, uint16_t &value, const bool closeConnection = true) {
    selectRegister(reg, false);

    Wire.requestFrom(address, (byte) 2);
    value = (Wire.read() << 8 | Wire.read());
    Wire.endTransmission(closeConnection);
  }
  void read16(const byte reg, int16_t &value, const bool closeConnection = true) {
    selectRegister(reg, false);

    Wire.requestFrom(address, (byte) 2);
    value = (Wire.read() << 8 | Wire.read());
    Wire.endTransmission(closeConnection);
  }

  void readIntoArray(
      const byte reg,
      byte *arr,
      const uint8_t n,
      const bool closeConnection = true
  ) {
    selectRegister(reg, false);
    Wire.requestFrom(address, (byte) n);
    for (uint8_t i = 0; i < n; i++)
      arr[i] = Wire.read();
    Wire.endTransmission(closeConnection);
  }
};

#endif /* APOLLON_FC_SRC_SENSORS_H */
