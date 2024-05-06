#ifndef APOLLON_FC_SENSOR_CONFIGURATION_H
#define APOLLON_FC_SENSOR_CONFIGURATION_H

#include "configuration.h"

/* Configures certain sensor hardware variables such as device address
 *
 */

#ifdef SENSOR_MPU6050
/* Accepted addresses:
 * AD0: 0x68 - 0110 1000
 * AD1: 0x69 - 0110 1001
 */
#define MPU6050_DEVICE_ADDRESS 0x68
#endif

#endif /* APOLLON_FC_SENSOR_CONFIGURATION_H */
