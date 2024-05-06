#include "MPU6050.h"
#include "../../sensor-configuration.h"

// Private functions

bool Sensor_MPU6050::isDataReady() {
  return status & 0x01; // return bit 1 (DATA_RDY_INT)
}

float Sensor_MPU6050::getAccelFT(const uint16_t &accel_test) {
  if (accel_test == 0) {
    return 0;
  }

  return 4096 * pow(0.92, (accel_test - 1) / 30.0);
}

float Sensor_MPU6050::getGyroFT(const uint16_t &gyro_test) {
  if (gyro_test == 0) {
    return 0;
  }

  return 3275 * pow(1.046, (float) (gyro_test - 1));
}


// Public functions

Sensor_MPU6050::Sensor_MPU6050(
    const bool useExternalSensors,
    const bool useFIFOBuffer,
    const bool useMotionDetection,
    const mpu6050_gyro_scale_t _gyroScale,
    const mpu6050_accel_scale_t _accelScale
) : gyroScale(_gyroScale), accelScale(_accelScale), I2C_Sensor(MPU6050_DEVICE_ADDRESS) {

  // Configure device
  beginTransmission(MPU6050_REGISTER_GYRO_CONFIG); {
    // Configure gyro and accel scale
    write8(gyroScale);
    write8(accelScale);

    // Configure interrupts
    changeRegister(MPU6050_REGISTER_INTERRUPT_ENABLE);
    write8((useMotionDetection << 6) | (useFIFOBuffer << 4) | (useExternalSensors << 3) | 0x01);

    // Wake up device
    changeRegister(MPU6050_REGISTER_POWER_MANAGEMENT_1);
    write8(0x01); // sleep disabled, cycle disabled, clock uses gyro x-axis PLL
  } endTransmission(connection.close);
}

bool Sensor_MPU6050::update() {
  beginTransmission(MPU6050_REGISTER_INTERRUPT_STATUS); {
    request(1);
    read8(status);
  } endTransmission(isDataReady() ? connection.keepAlive : connection.close);

  if (!isDataReady()) {
    return false;
  }

  // Fetch raw sensor data
  beginTransmission(MPU6050_REGISTER_ACCEL_XOUT_MSB); {
    request(14);
    read16(accel.x);
    read16(accel.y);
    read16(accel.z);
    read16(temp);
    read16(gyro.x);
    read16(gyro.y);
    read16(gyro.z);
  } endTransmission(connection.close);

  // Compensate raw sensor data
  // Temperature - Stores in tenths of degrees celsius (e.g. 241 ~= 24.1°C)
  // Typical value: -3872 = 25.1°C
  temp = (temp + 12420) / 34; // datasheet formula: (temp / 340) + 36.53 Shouldn't overflow with expected temperature ranges

  // Accelerometer - Stores in 0.001g (0.001 * 9.81m/s^2)
  switch (accelScale) {
    case MPU6050_ACCEL_SCALE_2G:
      accel.x = (int32_t) accel.x * 1000 >> 14;
      accel.y = (int32_t) accel.y * 1000 >> 14;
      accel.z = (int32_t) accel.z * 1000 >> 14;
      break;

    case MPU6050_ACCEL_SCALE_4G:
      accel.x = (int32_t) accel.x * 1000 >> 13;
      accel.y = (int32_t) accel.y * 1000 >> 13;
      accel.z = (int32_t) accel.z * 1000 >> 13;
      break;

    case MPU6050_ACCEL_SCALE_8G:
      accel.x = (int32_t) accel.x * 1000 >> 12;
      accel.y = (int32_t) accel.y * 1000 >> 12;
      accel.z = (int32_t) accel.z * 1000 >> 12;
      break;

    case MPU6050_ACCEL_SCALE_16G:
      accel.x = (int32_t) accel.x * 1000 >> 11;
      accel.y = (int32_t) accel.y * 1000 >> 11;
      accel.z = (int32_t) accel.z * 1000 >> 11;
      break;
  }


  // Gyroscope -Stores in 0.001°/s
  switch (gyroScale) {
    case MPU6050_GYRO_SCALE_250:
      gyro.x = (int64_t) gyro.x * 1000000 >> 17;
      gyro.y = (int64_t) gyro.y * 1000000 >> 17;
      gyro.z = (int64_t) gyro.z * 1000000 >> 17;
      break;

    case MPU6050_GYRO_SCALE_500:
      gyro.x = (int64_t) gyro.x * 1000000 >> 16;
      gyro.y = (int64_t) gyro.y * 1000000 >> 16;
      gyro.z = (int64_t) gyro.z * 1000000 >> 16;
      break;

    case MPU6050_GYRO_SCALE_1000:
      gyro.x = (int64_t) gyro.x * 1000000 >> 15;
      gyro.y = (int64_t) gyro.y * 1000000 >> 15;
      gyro.z = (int64_t) gyro.z * 1000000 >> 15;
      break;

    case MPU6050_GYRO_SCALE_2000:
      gyro.x = (int64_t) gyro.x * 1000000 >> 14;
      gyro.y = (int64_t) gyro.y * 1000000 >> 14;
      gyro.z = (int64_t) gyro.z * 1000000 >> 14;
      break;
  }

  return true;
}



byte Sensor_MPU6050::selfTest() {
  float ft_xa, ft_ya, ft_za, ft_xg, ft_yg, ft_zg,
        xa_resp, ya_resp, za_resp, xg_resp, yg_resp, zg_resp;
  uint8_t xa_test, ya_test, za_test, xg_test, yg_test, zg_test;
  uint16_t gyroX_base, gyroY_base, gyroZ_base, gyroX_test, gyroY_test, gyroZ_test,
           accelX_base, accelY_base, accelZ_base, accelX_test, accelY_test, accelZ_test;

  beginTransmission(MPU6050_REGISTER_GYRO_CONFIG); {
    // Set scale to +-250dps and +-8g, and disable self-test
    write8(MPU6050_GYRO_SCALE_250);
    write8(MPU6050_ACCEL_SCALE_8G);

    // Wait for data to be ready
    do {
      delay(5);

      changeRegister(MPU6050_REGISTER_INTERRUPT_STATUS);
      request(1);
      read8(status);
    } while (!isDataReady());

    // Read baseline data
    changeRegister(MPU6050_REGISTER_ACCEL_XOUT_MSB);
    request(6);
    read16(accelX_base);
    read16(accelY_base);
    read16(accelZ_base);

    changeRegister(MPU6050_REGISTER_GYRO_XOUT_MSB);
    request(6);
    read16(gyroX_base);
    read16(gyroY_base);
    read16(gyroZ_base);

    // Enable self-test on all axis
    changeRegister(MPU6050_REGISTER_GYRO_CONFIG);
    write8(0xD0 | MPU6050_GYRO_SCALE_250);
    write8(0xD0 | MPU6050_ACCEL_SCALE_8G);

    // Wait for data to be ready
    do {
      delay(5);

      changeRegister(MPU6050_REGISTER_INTERRUPT_STATUS);
      request(1);
      read8(status);
    } while (!isDataReady());

    // Read self test data
    changeRegister(MPU6050_REGISTER_ACCEL_XOUT_MSB);
    request(6);
    read16(accelX_test);
    read16(accelY_test);
    read16(accelZ_test);

    changeRegister(MPU6050_REGISTER_GYRO_XOUT_MSB);
    request(6);
    read16(gyroX_test);
    read16(gyroY_test);
    read16(gyroZ_test);

    changeRegister(MPU6050_REGISTER_SELF_TEST_X);
    request(4);
    read8(xg_test); // XA_TEST[4:2], XG_TEST[4:0]
    read8(yg_test); // YA_TEST[4:2], YG_TEST[4:0]
    read8(zg_test); // ZA_TEST[4:2], ZG_TEST[4:0]
    read8(za_test); // 00, XA_TEST[1:0], YA_TEST[1:0], ZA_TEST[1:0]

    // Reset gyro and accelerometer scale to previous values
    changeRegister(MPU6050_REGISTER_GYRO_CONFIG);
    write8(gyroScale);
    write8(accelScale);
  } endTransmission(connection.close);

  // Reconstruct self-test data
  xa_test = (xg_test & 0xE0) | (za_test & 0x30);
  ya_test = (yg_test & 0xE0) | (za_test & 0x0C);
  za_test = (zg_test & 0xE0) | (za_test & 0x03);
  xg_test &= 0x1F;
  yg_test &= 0x1F;
  zg_test &= 0x1F;

  // Calculate factory trims
  ft_xa = getAccelFT(xa_test);
  ft_ya = getAccelFT(ya_test);
  ft_za = getAccelFT(za_test);
  ft_xg = getGyroFT(xg_test);
  ft_yg = -getGyroFT(yg_test); // Not a typo, this one is negative
  ft_zg = getGyroFT(zg_test);

  // Calculate change from FT (this is what must be within limits)
  xa_resp = ((int32_t) accelX_test - accelX_base - ft_xa) / ft_xa;
  ya_resp = ((int32_t) accelY_test - accelY_base - ft_ya) / ft_ya;
  za_resp = ((int32_t) accelZ_test - accelZ_base - ft_za) / ft_za;
  xg_resp = ((int32_t)  gyroX_test -  gyroX_base - ft_xg) / ft_xg;
  yg_resp = ((int32_t)  gyroY_test -  gyroY_base - ft_yg) / ft_yg;
  zg_resp = ((int32_t)  gyroZ_test -  gyroZ_base - ft_zg) / ft_zg;

  return
      ((accel_self_test_min < xa_resp && xa_resp < accel_self_test_max) << 5)
    | ((accel_self_test_min < ya_resp && ya_resp < accel_self_test_max) << 4)
    | ((accel_self_test_min < za_resp && za_resp < accel_self_test_max) << 3)
    | ((gyro_self_test_min < xg_resp && xg_resp < gyro_self_test_max) << 2)
    | ((gyro_self_test_min < yg_resp && yg_resp < gyro_self_test_max) << 1)
    | ((gyro_self_test_min < zg_resp && zg_resp < gyro_self_test_max));
}
