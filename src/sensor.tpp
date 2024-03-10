/* I2C_Sensor template functions */
#include <Wire.h>

template<typename T>
void I2C_Sensor::read8(T &value, const bool closeConnection) {
  Wire.requestFrom(address, (byte) 1);
  value = (T) Wire.read();
  Wire.endTransmission(closeConnection);
}

template<typename T>
void I2C_Sensor::read16(T &value, const bool closeConnection) {
  Wire.requestFrom(address, (byte) 2);
  value = (T) ((Wire.read() << 8) | Wire.read());
  Wire.endTransmission(closeConnection);
}

template<typename T>
void I2C_Sensor::readIntoArray(T *arr, const uint8_t n, const bool closeConnection) {
  Wire.requestFrom(address, (byte) n * sizeof(T));
  for (uint8_t i = 0; i < n; i++) {
    arr[i] = 0;
    for (uint8_t j = sizeof(T); j > 0; j--) {
      arr[i] |= (T) Wire.read() << (8 * (j - 1));
    }
  }
  Wire.endTransmission(closeConnection);
}
