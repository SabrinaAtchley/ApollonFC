#ifndef APOLLON_FC_SRC_SENSOR_H
#define APOLLON_FC_SRC_SENSOR_H

#include <Wire.h>

/* FIFO byte buffer */
class ByteBuffer {
private:
  const uint8_t length; // Max 255 elements
  byte *data;
  uint8_t start = 0;
  uint8_t end = 0;

public:

  constexpr ByteBuffer(byte* _data, const uint8_t _n)
    : length(_n), data(_data) {};

  byte pop() {
    if (start == end) // No data available
      return 0x00;

    const byte b = data[start];
    start = (start < length) ? start + 1 : start = 0;
    return b;
  }

  void push(const byte b) {
    data[end] = b;
    end = (end < length) ? end + 1 : end = 0;
  }

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

  selectRegister(const byte reg) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
  }

protected:
  I2C_Sensor(const byte addr) : address(addr) {
    // Enable I2C
    Wire.begin();
  };

  void write8(const byte reg, const byte value) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
  }

  void read8(const byte reg, byte &value) {
    selectRegister(reg);

    Wire.requestFrom(address, (byte) 1);
    value = Wire.read();
    Wire.endTransmission();
  }


  void read16(const byte reg, uint16_t &value) {
    selectRegister(reg);

    Wire.requestFrom(address, (byte) 2);
    value = (Wire.read() << 8 | Wire.read());
    Wire.endTransmission();
  }
  void read16(const byte reg, int16_t &value) {
    selectRegister(reg);

    Wire.requestFrom(address, (byte) 2);
    value = (Wire.read() << 8 | Wire.read());
    Wire.endTransmission();
  }


  void readIntoBuffer(const byte reg, const uint8_t n, ByteBuffer &buffer) {
    selectRegister(reg);

    Wire.requestFrom(address, (byte) n);
    for (uint8_t i = 0; i < n; i++)
      buffer.push(Wire.read());
    Wire.endTransmission();
  }


};

#endif /* APOLLON_FC_SRC_SENSORS_H */
