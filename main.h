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
#include "src/motor-controller.h"
#include "src/pilot-control.h"
#include "src/peripherals.h"
#include "src/error.h"
#include "src/q16x16/q16x16.h"
#include "src/madgwick.h"

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

void setup();
void loop();

void slowUpdate(const Q16x16 deltaT);
void fastUpdate(const Q16x16 deltaT);

#endif /* APOLLON_FC_MAIN_H */
