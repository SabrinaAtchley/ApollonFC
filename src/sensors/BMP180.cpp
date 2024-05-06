#include "BMP180.h"
#include "../error.h"

bool Sensor_BMP180::update() {
  // Read SCO bit - 1 when converting, 0 when finished (data ready)
  byte sco;

  beginTransmission(BMP180_REGISTER_CTRL_MEAS); {
    request(1);
    read8(sco);
    sco &= 0x20; // include only bit 5, sco
  } endTransmission(sco ? connection.close : connection.keepAlive);

  if (sco) { // Still converting
    return false;
  }

  // Grab data and handle updates
  switch(step) {
    case BMP180_STEP_GET_UT:
      // Read UT (raw temperature)
      beginTransmission(BMP180_REGISTER_OUT_MSB); {
        request(2);
        read16(ut);
        // Send pressure request command
        changeRegister(BMP180_REGISTER_CTRL_MEAS);
        write8(BMP180_CMD_PRESSURE + (mode << 6));
      } endTransmission(connection.close);

      step = BMP180_STEP_GET_UP;
      break;

    case BMP180_STEP_GET_UP:
      // Read UP (raw pressure)
      beginTransmission(BMP180_REGISTER_OUT_MSB); {
        uint16_t msb;
        uint8_t xlsb;
        request(3);
        read16(msb);
        read8(xlsb);
        up = (((uint32_t) msb << 8) + xlsb) >> (8 - mode);
        // Send temperature request command
        changeRegister(BMP180_REGISTER_CTRL_MEAS);
        write8(BMP180_CMD_TEMPERATURE);
      } endTransmission(connection.close);

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

void Sensor_BMP180::computeB5() {
  int32_t x1 = ((ut - (int32_t) ac6) * (int32_t) ac5) >> 15;
  int32_t x2 = ((int32_t) mc << 11) / (x1 + (int32_t) md);
  b5 = x1 + x2;
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
  beginTransmission(BMP180_REGISTER_CHIPID); {
    request(1);
    read8(id);
  } endTransmission(connection.keepAlive);

  if (id != BMP180_CHIP_ID) {
    ERROR(3); // I2C device misconfiguration error
  }

  // Read calibration data
  beginTransmission(BMP180_REGISTER_AC1); {
    request(22);
    read16(ac1);
    read16(ac2);
    read16(ac3);
    read16(ac4);
    read16(ac5);
    read16(ac6);
    read16(b1);
    read16(b2);
    read16(mb);
    read16(mc);
    read16(md);
  } endTransmission(connection.close);
}
