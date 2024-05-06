#ifndef APOLLON_FC_SRC_SENSOR_H
#define APOLLON_FC_SRC_SENSOR_H

#include <Arduino.h>

struct Connection {
  const bool keepAlive;
  const bool close;
};

class Sensor { // Abstract

public:
  /* Sensor update function
   * To be called once every loop, handles sensor value updating in a step-based
   * asynchronous process.
   * Outputs true once all new data is fully ready,
   * Outputs false otherwise
   */
  virtual bool update() = 0;
};


/* Sensor class which communicates over I2C
 *
 * Read example
 * beginTransmission(0x55); {
 *   request(2);
 *   read16(temp);
 * } endTransmission(connection.close);
 *
 * Write example
 * beginTransmission(0x55); {
 *   write8(0x82);
 *   changeRegister(0x66);
 *   write8(0x93);
 * } endTransmission(connection.close);
 *
 */

class I2C_Sensor : Sensor { // Abstract
private:
  const byte address; // Device I2C address

protected:
  /* Allows connection.keepAlive and connection.close to be used
   * for closing I2C connections or keeping them alive between reads/writes
   *
   * e.g. read8(BMP180_REGISTER_OUT_MSB, data, connection.keepAlive);
   */
  static constexpr Connection connection = {false, true}; // {keepAlive, close}

  // Initialize I2C with device address
  I2C_Sensor(const byte addr);

  /* Begins a transmission to/from register 'reg'
   * Leave 0 to use previously set register (most useful for auto-incrementing devices)
   */
  void beginTransmission(const byte reg = 0x00);

  // Finishes a transmission. Use connection.keepAlive to reserve the bus
  void endTransmission(const bool closeConnection);

  // Changes register targeted by a transmission
  void changeRegister(const byte reg);

  // Request n bytes from device to be read by the master
  void request(const byte n);

  // Write one byte to the current register
  void write8(const byte value);

  /* Read 1 byte from the current register and store in value
   * Should be preceded by a request() call
   */
  template<typename T>
  void read8(T &value);

  /* Read 2 bytes from the current register and store in value
   * Should be preceded by a request() call
   */
  template<typename T>
  void read16(T &value);

  /* Read n * sizeof(T) bytes from the current register and store in *arr
   * Should be preceded by a request() call
   */
  template<typename T>
  void readIntoArray(T *arr, const uint8_t n);
};

#include "sensor.tpp"

#endif /* APOLLON_FC_SRC_SENSORS_H */
