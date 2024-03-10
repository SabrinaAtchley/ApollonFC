#ifndef APOLLON_FC_SRC_SENSOR_H
#define APOLLON_FC_SRC_SENSOR_H

#include <Arduino.h>

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
  /* Allows connection.keepAlive and connection.close to be used
   * for closing I2C connections or keeping them alive between reads/writes
   *
   * e.g. read8(BMP180_REGISTER_OUT_MSB, data, connection.keepAlive);
   */
  static constexpr Connection connection = {false, true};

  // Initialize I2C with device address
  I2C_Sensor(const byte addr);

  // Send register to slave before r/w
  void selectRegister(const byte reg, const bool closeConnection);

  // Write one byte to the given register
  void write8(const byte reg, const byte value, const bool closeConnection);

  // Read 1 byte from the current register and store in value
  template<typename T>
  void read8(T &value, const bool closeConnection);

  // Read 2 bytes from the current register and store in value
  template<typename T>
  void read16(T &value, const bool closeConnection);

  // Read n * sizeof(T) bytes from the current register and store in *arr
  template<typename T>
  void readIntoArray(T *arr, const uint8_t n, const bool closeConnection);
};

#include "sensor.tpp"

#endif /* APOLLON_FC_SRC_SENSORS_H */
