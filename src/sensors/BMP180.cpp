#include "BMP180.h"
#include "../error.h"

void Sensor_BMP180::readCalibrationData(const bool closeConnection) {
  selectRegister(BMP180_REGISTER_AC1, connection.keepAlive);
  read16(ac1, connection.keepAlive);
  read16(ac2, connection.keepAlive);
  read16(ac3, connection.keepAlive);
  read16(ac4, connection.keepAlive);
  read16(ac5, connection.keepAlive);
  read16(ac6, connection.keepAlive);
  read16(b1, connection.keepAlive);
  read16(b2, connection.keepAlive);
  read16(mb, connection.keepAlive);
  read16(mc, connection.keepAlive);
  read16(md, closeConnection);
}

bool Sensor_BMP180::readSCO(const bool closeConnection) {
  byte ctrl_meas;
  selectRegister(BMP180_REGISTER_CTRL_MEAS, connection.keepAlive);
  read8(ctrl_meas, closeConnection);
  return 0x20 & ctrl_meas; // return bit 5, sco
}

void Sensor_BMP180::computeB5() {
  int32_t x1 = ((ut - (int32_t) ac6) * (int32_t) ac5) >> 15;
  int32_t x2 = ((int32_t) mc << 11) / (x1 + (int32_t) md);
  b5 = x1 + x2;
}

void Sensor_BMP180::requestTemperature(const bool closeConnection) {
  write8(BMP180_REGISTER_CTRL_MEAS, BMP180_CMD_TEMPERATURE, closeConnection);
}

void Sensor_BMP180::requestPressure(const bool closeConnection) {
  write8(BMP180_REGISTER_CTRL_MEAS, BMP180_CMD_PRESSURE + (mode << 6), closeConnection);
}

void Sensor_BMP180::getRawTemperature(const bool closeConnection) {
  selectRegister(BMP180_REGISTER_OUT_MSB, connection.keepAlive);
  read16(ut, closeConnection);
}

void Sensor_BMP180::getRawPressure(const bool closeConnection) {
  uint16_t msb;
  uint8_t xlsb;
  selectRegister(BMP180_REGISTER_OUT_MSB, connection.keepAlive);
  read16(msb, connection.keepAlive);
  read8(xlsb, closeConnection);
  up = (((uint32_t) msb << 8) + xlsb) >> (8 - mode);
}

void Sensor_BMP180::compensateTemperature() {
  temperature = (b5 + 8) >> 4; // temperature in 0.1°C
}

void Sensor_BMP180::compensatePressure() {
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

void Sensor_BMP180::pressureToAltitude() {
  // Uses international barometric formula found in the BMP180 datasheet
  altitude = 44330.0 * (1.0 - pow((float) pressure / seaLevel, 0.19029));
}

Sensor_BMP180::Sensor_BMP180(const bmp180_mode_t m, const uint32_t _seaLevel)
    : I2C_Sensor(BMP180_ADDRESS), mode(m), seaLevel(_seaLevel)
{
  uint8_t id;
  selectRegister(BMP180_REGISTER_CHIPID, connection.keepAlive);
  read8(id, connection.keepAlive);
  if (id != BMP180_CHIP_ID)
    ERROR(3); // I2C device misconfiguration error

  readCalibrationData(connection.close);
}

bool Sensor_BMP180::update() {
  switch(step) {
    case BMP180_STEP_GET_UT:
      if (readSCO(connection.close)) // Close connection in case of break
        break; // Still converting

      // UT is finished converting
      getRawTemperature(connection.keepAlive);
      requestPressure(connection.close);
      step = BMP180_STEP_GET_UP;
      break;

    case BMP180_STEP_GET_UP:
      if (readSCO(connection.close)) // Close connection in case of break
        break; // Still converting

      // UP is finished converting
      getRawPressure(connection.keepAlive);
      requestTemperature(connection.close); // Start measurements for next cycle
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
