#ifndef APOLLON_FC_SRC_SENSOR_H
#define APOLLON_FC_SRC_SENSOR_H

#include <Wire.h>

struct Connection {
  const bool keepAlive;
  const bool close;
};

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

protected:
  static constexpr Connection connection = {false, true};

  I2C_Sensor(const byte addr) : address(addr) {
    // Enable I2C
    Wire.begin();
  };

  void selectRegister(const byte reg, const bool closeConnection) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission(closeConnection);
  }

  void write8(const byte reg, const byte value, const bool closeConnection) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(closeConnection);
  }

  template<typename T>
  void read8(T &value, const bool closeConnection) {
    Wire.requestFrom(address, (byte) 1);
    value = (T) Wire.read();
    Wire.endTransmission(closeConnection);
  }

  template<typename T>
  void read16(T &value, const bool closeConnection) {
    Wire.requestFrom(address, (byte) 2);
    value = (T) (Wire.read() << 8 | Wire.read());
    Wire.endTransmission(closeConnection);
  }

  template<typename T>
  void readIntoArray(T *arr, const uint8_t n, const bool closeConnection) {
    Wire.requestFrom(address, (byte) n * sizeof(T));
    for (uint8_t i = 0; i < n; i++) {
      arr[i] = 0;
      for (uint8_t j = sizeof(T); j > 0; j--) {
        arr[i] |= (T) Wire.read() << (8 * (j - 1));
      }
    }
    Wire.endTransmission(closeConnection);
  }
};

#endif /* APOLLON_FC_SRC_SENSORS_H */
