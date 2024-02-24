#ifndef APOLLON_FC_SRC_SENSORS_BMP180_H
#define APOLLON_FC_SRC_SENSORS_BMP180_H

#include "../sensor.h"
#include "../macros.h"

/* BMP180 Sensor module/chip
 * Interfaces: I2C
 * Measures: barometric pressure, atmospheric temperature
 * Estimates: Altitude
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
 */

enum : byte {
  BMP180_ADDRESS =- 0x77, // I2C address
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
  int16_t ac1, ac2, ac3, b1, b2, mb, mc, md; // Calibration data
  uint16_t ac4, ac5, ac6; // Calibration data
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
  void readCalibrationData() {
    read16(BMP180_REGISTER_AC1, ac1);
    read16(BMP180_REGISTER_AC2, ac2);
    read16(BMP180_REGISTER_AC3, ac3);
    read16(BMP180_REGISTER_AC4, ac4);
    read16(BMP180_REGISTER_AC5, ac5);
    read16(BMP180_REGISTER_AC6, ac6);
    read16(BMP180_REGISTER_B1, b1);
    read16(BMP180_REGISTER_B2, b2);
    read16(BMP180_REGISTER_MB, mb);
    read16(BMP180_REGISTER_MC, mc);
    read16(BMP180_REGISTER_MD, md);
  }

  // reads SCO bit to see if conversion is finished
  bool readSCO() {
    byte ctrl_meas;
    read8(BMP180_REGISTER_CTRL_MEAS, ctrl_meas);
    return 0x20 & ctrl_meas; // return bit 5, sco
  }

  // param: ut, calibration data
  void computeB5() {
    int32_t x1 = ((ut - (int32_t) ac6) * (int32_t) ac5) >> 15;
    int32_t x2 = ((int32_t) mc << 11) / (x1 + (int32_t) md);
    b5 = x1 + x2;
  }

  // Begins temperature conversion
  void requestTemperature() {
    write8(BMP180_REGISTER_CTRL_MEAS, BMP180_CMD_TEMPERATURE);
  }

  // Begins pressure conversion
  void requestPressure() {
    write8(BMP180_REGISTER_CTRL_MEAS, BMP180_CMD_PRESSURE + (mode << 6));
  }

  void getRawTemperature() {
    read16(BMP180_REGISTER_OUT_MSB, ut);
  }

  void getRawPressure() {
    uint16_t msb;
    uint8_t xlsb;
    read16(BMP180_REGISTER_OUT_MSB, msb);
    read8(BMP180_REGISTER_OUT_XLSB, xlsb);
    up = (((uint32_t) msb << 8) + xlsb) >> (8 - mode);
  }

  void compensateTemperature() {
    temperature = (b5 + 8) >> 4; // temperature in 0.1°C
  }

  // Param: ut, up, calibration data
  void compensatePressure() {
    int32_t x1, x2, b6, x3, b3, p;
    uint32_t b4, b7;

    /* Check datasheet for compensation algorithm */
    b6 = b5 - 4000;
    x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
    x2 = (ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = ((( (int32_t) ac1 * 4 + x3) << mode) + 2) >> 2;
    x1 = (ac3 * b6) >> 13;
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
    b7 = (uint32_t) (up - b3) * (50000 >> mode);

    if (b7 < 0x80000000) {
      p = (b7 << 1) / b4;
    } else {
      p = (b7 / b4) << 1;
    }

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    pressure = p + ((x1 + x2 + 3791) >> 4);
  }

  void pressureToAltitude() {
    // Uses international barometric formula found in the BMP180 datasheet
    altitude = 44330.0 * (1.0 - pow((float) pressure / seaLevel, 0.19029));
  }

public:
  Sensor_BMP180(const bmp180_mode_t m, const uint32_t _seaLevel)
      : I2C_Sensor(BMP180_ADDRESS), mode(m), seaLevel(_seaLevel)
    {
    uint8_t id;
    read8(BMP180_REGISTER_CHIPID, id);
    if (id != BMP180_CHIP_ID)
      ERROR(3); // I2C device misconfiguration error

    readCalibrationData();
  }

  /* Called once per loop to update sensor values */
  bool update() {
    switch(step) {
      case BMP180_STEP_GET_UT:
        if (readSCO())
          break; // Still converting

        // UT is finished converting
        getRawTemperature();
        requestPressure();
        step = BMP180_STEP_GET_UP;
        break;

      case BMP180_STEP_GET_UP:
        if (readSCO())
          break; // Still converting

        // UP is finished converting
        getRawPressure();
        requestTemperature();
        step = BMP180_STEP_COMPENSATE;

      case BMP180_STEP_COMPENSATE:
        computeB5();
        compensateTemperature();
        compensatePressure();
        pressureToAltitude();
        step = BMP180_STEP_GET_UT;
        return true; // Signal to FC that new data is available
    }
    return false;
  }
};


#endif
