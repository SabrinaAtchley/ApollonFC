#ifndef APOLLON_FC_MAIN_H
#define APOLLON_FC_MAIN_H

/* Apollon Flight Controller main module
 *
 * author: Sabrina Atchley (https://github.com/SabrinaAtchley)
 * project: https://github.com/SabrinaAtchley/ApollonFC/tree/main
 *
 * This is the main module for the Apollon Flight Controller
 * It represents the drone as a whole from an object-oriented viewpoint,
 * or more procedurally, this is where all the various modules and systems
 * are brought together, coordinated, and used to provide all of the services
 * the drone requires for proper flight.
 */

#include "configuration.h"
#include "src/receivers.h"
#include "src/pid.h"
#include "src/motor-controller.h"
#include "src/peripherals.h"

// Sensor boards
#ifdef SENSOR_BMP180
#include "src/sensors/BMP180.h"
#endif
#ifdef SENSOR_HMC5883L
#include "src/sensors/HMC5883L.h"
#endif
#ifdef SENSOR_MPU6050
#include "src/sensors/MPU6050.h"
#endif

void setup() {
  Serial.begin(9600);

  RECEIVER_T receiver;

  // Sensor initialization
  // TODO: Revisit sensor configuration for the case of multiple sensors (e.g. 2 barometers)
  #ifdef SENSOR_BMP180
  Sensor_BMP180 bmp180(BMP180_MODE_ULTRA_HIGH_RES, 102070); // oss, pressure at sea level
  #endif
  #ifdef SENSOR_HMC5883L
  Sensor_HMC5883L hmc5883l(HMC5883L_MODE_SINGLE, HMC5883L_GAIN_1, HMC5883L_OSS_3);
  #endif
  #ifdef SENSOR_MPU6050
  Sensor_MPU6050 mpu6050(false, false, false, MPU6050_GYRO_SCALE_500, MPU6050_ACCEL_SCALE_4G);
  #endif

  // Placeholder PIDs
  PID pidPitch(0, 0, 0, 0, 0);
  PID pidRoll(0, 0, 0, 0, 0);
  PID pidYaw(0, 0, 0, 0, 0);
  PID pidThrottle(0, 0, 0, 0, 0);

  // Motor control module initialization here

  // Calibration code and any other setup here

  // Play start up jingle when finished initializing!
  Buzzer::ready();
}

void loop() {

}

#endif /* APOLLON_FC_MAIN_H */
