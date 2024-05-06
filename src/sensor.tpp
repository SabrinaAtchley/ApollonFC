/* I2C_Sensor template functions */
#include <Wire.h>

template<typename T>
void I2C_Sensor::read8(T &value) {
  value = (T) Wire.read();
}

template<typename T>
void I2C_Sensor::read16(T &value) {
  value = (T) ((Wire.read() << 8) | Wire.read());
}

template<typename T>
void I2C_Sensor::readIntoArray(T *arr, const uint8_t n) {
  for (uint8_t i = 0; i < n; i++) {
    arr[i] = 0;
    for (uint8_t j = sizeof(T); j > 0; j--) {
      arr[i] |= (T) Wire.read() << (8 * (j - 1));
    }
  }
}
