#ifndef APOLLON_FC_SRC_SENSORS_BMP180_H
#define APOLLON_FC_SRC_SENSORS_BMP180_H

#include <Arduino.h>
#include "../sensor.h"

/* BMP180 Sensor module/chip
 * Interfaces: I2C
 * Measures: barometric pressure, atmospheric temperature
 * Estimates: Altitude
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
 */

enum : byte {
  BMP180_ADDRESS = 0x77, // I2C address
  BMP180_CHIP_ID = 0x55, // ID held in ID register, static across all BMP180's
  BMP180_REGISTER_AC1 = 0XAA, // R int16_t, calibration data
  BMP180_REGISTER_AC2 = 0XAC, // R int16_t, calibration data
  BMP180_REGISTER_AC3 = 0XAE, // R int16_t, calibration data
  BMP180_REGISTER_AC4 = 0XB0, // R uint16_t, calibration data
  BMP180_REGISTER_AC5 = 0XB2, // R uint16_t, calibration data
  BMP180_REGISTER_AC6 = 0XB4, // R uint16_t, calibration data
  BMP180_REGISTER_B1 = 0XB6, // R int16_t, calibration data
  BMP180_REGISTER_B2 = 0XB8, // R int16_t, calibration data
  BMP180_REGISTER_MB = 0XBA, // R int16_t, calibration data
  BMP180_REGISTER_MC = 0XBC, // R int16_t, calibration data
  BMP180_REGISTER_MD = 0XBE, // R int16_t, calibration data
  BMP180_REGISTER_CHIPID = 0XD0, // R byte, chip id (fixed at 0x55)
  BMP180_REGISTER_VERSION = 0XD1, // R byte?, chip version
  BMP180_REGISTER_SOFTRESET = 0XE0, // RW byte, soft resets if 0xB6 written to
  BMP180_REGISTER_CTRL_MEAS = 0XF4, // RW byte, controls oversampling, conversion, measurements
  BMP180_REGISTER_OUT_MSB = 0XF6, // R byte, most significant byte of data (signed)
  BMP180_REGISTER_OUT_LSB = 0XF7, // R byte, least significant byte of data (signed)
  BMP180_REGISTER_OUT_XLSB = 0xF8, // R byte<7:3>, extra 5 bits (low) for ultra high resolution pressure data
  BMP180_CMD_TEMPERATURE = 0X2E, // command to write to CTRL_MEAS to read temperature
  BMP180_CMD_PRESSURE = 0X34, // bits<5:0> of pressure command to write to CTRL_MEAS
};

typedef enum : byte {
  BMP180_MODE_ULTRA_LOW_POWER = 0,
  BMP180_MODE_STANDARD = 1,
  BMP180_MODE_HIGH_RES = 2,
  BMP180_MODE_ULTRA_HIGH_RES = 3,
} bmp180_mode_t;

typedef enum : byte {
  BMP180_STEP_GET_UT = 0,
  BMP180_STEP_GET_UP = 1,
  BMP180_STEP_COMPENSATE = 2,
} bmp180_conversion_step_t;


/* BMP180 Barometer and Temperature sensor
 * Measures: barometric pressure (+-6-3Pa), ambient temperature (+-0.1°C)
 * Estimates: altitude above sea level (+-1m)
 */
class Sensor_BMP180 : I2C_Sensor {
private:
  // Calibration data
  int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
  uint16_t ac4, ac5, ac6;
  int32_t b5 = 0;

  const uint32_t seaLevel;
  bmp180_mode_t mode; // Over sampling mode for pressure measurements (OSS)
  bmp180_conversion_step_t step = BMP180_STEP_GET_UT;
public:
  int16_t ut = 0; // Uncompensated temperature
  int32_t up = 0; // Uncompensated pressure (16-21 bits
  int16_t temperature = 0; // Temperature in 0.01°C
  int32_t pressure = 0; // Pressure in Pa
  float altitude = 0; // Altitude in meters

private:
  /* Reads calibration data and sets calibration variables
   * ac1, ac2, ac3, ac4, ac5, ac6, b1, b2, mb, mc, md
   */
  void readCalibrationData(const bool closeConnection);

  // reads SCO bit to see if conversion is finished
  bool readSCO(const bool closeConnection);

  // param: ut, calibration data
  void computeB5();

  // Begins temperature conversion
  void requestTemperature(const bool closeConnection);

  // Begins pressure conversion
  void requestPressure(const bool closeConnection);

  // Reads raw temperature (assumes ready)
  void getRawTemperature(const bool closeConnection);

  // Reads raw pressure (assumes ready)
  void getRawPressure(const bool closeConnection);

  // Compensates UT reading
  void compensateTemperature();

  // Param: ut, up, calibration data
  void compensatePressure();

  // Estimates altitude from UP and sea level using International Barometric formula
  void pressureToAltitude();

public:
  Sensor_BMP180(const bmp180_mode_t m, const uint32_t _seaLevel);

  /* Called once per loop to update sensor values */
  bool update();
};

#endif
