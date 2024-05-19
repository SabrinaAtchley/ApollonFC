#ifndef APOLLON_FC_CONFIGURATION_H
#define APOLLON_FC_CONFIGURATION_H

/* System Configuration
 *
 * Configures system wide settings which are agnostic of hardware
 */

#include "src/macros.h"

#define ERROR_MESSAGE_0
#define ERROR_MESSAGE_1
#define ERROR_MESSAGE_2 "Err2: Receiver channel number out of bounds"
#define ERROR_MESSAGE_3 "Err3: I2C device misconfiguration - check wires and addresses"
#define ERROR_MESSAGE_4
#define ERROR_MESSAGE_5
#define ERROR_MESSAGE_6
#define ERROR_MESSAGE_7
#define ERROR_MESSAGE_8
#define ERROR_MESSAGE_9
#define ERROR_MESSAGE_10
#define ERROR_MESSAGE_11
#define ERROR_MESSAGE_12
#define ERROR_MESSAGE_13
#define ERROR_MESSAGE_14
#define ERROR_MESSAGE_15 "Err15: Unspecified runtime error"



/* Input Configuration
 *
 * Configures pilot input parameters, e.g. deadzone width,
 * receiver type and setup, channels
 */

// Input ranges
#define INPUT_MOTOR_MIN 1000
#define INPUT_MOTOR_MAX 2000

// Input deadzones (snapping points)
#define INPUT_DEADZONE_WIDTH 30
#define INPUT_DEADZONE_STOPS (1000, 1500, 2000)



// Receiver configuration

// Receiver type
#define INPUT_RECEIVER_PPM
//#define INPUT_RECEIVER_PWM
//#define INPUT_RECEIVER_SBUS
//#define INPUT_RECEIVER_IBUS

#define INPUT_RECEIVER_IS_SERIAL

// PPM Receiver setup
//#define PPM_MAX_ERROR 10 // Default: 10; Small leeway to account for minor errors
//#define PPM_BLANK_TIME 2100 // Default: 2100; Needs to be greater than INPUT_MOTOR_MAX + PPM_MAX_ERROR


#if ENABLED(INPUT_RECEIVER_IS_SERIAL)
  #define SERIAL_RECEIVER_PIN 2
#else
  #define PARALLEL_RECEIVER_PINS {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
#endif



// Receiver Channel setup
// Total number of supported channels
#define INPUT_CHANNELS 10
// Sets up channel control aliases; Does *not* need one for each channel
#define INPUT_CHANNEL_ROLL 1
#define INPUT_CHANNEL_PITCH 2
#define INPUT_CHANNEL_THROTTLE 3
#define INPUT_CHANNEL_YAW 4
#define INPUT_CHANNEL_SWA 5
#define INPUT_CHANNEL_SWB 6
#define INPUT_CHANNEL_SWC 7
#define INPUT_CHANNEL_SWD 8


/* Peripheral Configuration
 *
 * Configures small peripherals such as buzzers, buttons, LEDs
 */

// Buzzer
#define PERIPHERAL_BUZZER_PIN 8
// Used for error beeps
#define PERIPHERAL_BUZZER_HIGH 784
#define PERIPHERAL_BUZZER_LOW 392
#define PERIPHERAL_BUZZER_TONE_LENGTH 200
#define PERIPHERAL_BUZZER_INTERTONE_DELAY 200
#define PERIPHERAL_BUZZER_REPEAT_DELAY 2000
// Uses uint8_t, so max is 8 bits
#define PERIPHERAL_BUZZER_MAX_BITS 4
// quarter note length (ms)
#define PERIPHERAL_BUZZER_QUARTER 800
// Drone startup melody
// Metroid! A5 D6 C6 A5 G5
#define PERIPHERAL_BUZZER_STARTUP ((880, PERIPHERAL_BUZZER_QUARTER), (1175, PERIPHERAL_BUZZER_QUARTER), (1047, PERIPHERAL_BUZZER_QUARTER), (880, PERIPHERAL_BUZZER_QUARTER), (784, 1.5*PERIPHERAL_BUZZER_QUARTER))


/* ESC and Motor Configuration
 *
 * Configuration for Electronic Speed Controllers and motors
 */
#define MOTOR_AMOUNT 4
// motor 1, 2, 3, 4
#define ESC_PINS (3, 10, 11, 12) //TODO: CHECK PINS AGAINST MOTOR NUMBERS
// #define ESC_DO_CALIBRATION // Uncomment to automatically calibrate ESCs on startup every time

/* Possibile configurations:
 * QUAD_X
 * QUAD_PLUS
 *
 */
#define DRONE_CONFIGURATION QUAD_X

/* Motor mixing matrices
 * row 1 - contribution of motor 1 to yaw, pitch, roll, throttle
 * row 2 - contribution of motor 2 to yaw, pitch, roll, throttle
 * row 3 - contribution of motor 3 to yaw, pitch, roll, throttle
 * row 4 - contribution of motor 4 to yaw, pitch, roll, throttle
 * and so on for higher rotor counts
 */
#define MOTOR_MIXING_MATRIX_QUAD_X { { 1.0, -1.0,  1.0,  1.0}, \
                                     { 1.0,  1.0, -1.0,  1.0}, \
                                     { 1.0,  1.0,  1.0, -1.0}, \
                                     { 1.0, -1.0, -1.0, -1.0} }

#define MOTOR_MIXING_MATRIX_QUAD_PLUS { { 1.0, -1.0,  0.0, -1.0}, \
                                        { 1.0,  1.0,  0.0, -1.0}, \
                                        { 1.0,  0.0, -1.0,  1.0}, \
                                        { 1.0,  0.0,  1.0,  1.0} }


#define MOTOR_MIXING_MATRIX CAT(MOTOR_MIXING_MATRIX_, DRONE_CONFIGURATION)


/* Sensor modules
 *
 * Defines which sensors should be loaded and used
 * In-depth sensor configuration such as device address
 * can be found in sensor-configuration.h
 */

#define SENSOR_BMP180
#define SENSOR_HMC5883L
#define SENSOR_MPU6050


/* PID Configuration
 *
 * PID controllers take the error between the target state and estimate state
 * as output by the sensor fusion algorithm and pilot control module
 */

 /* PID output range is mostly arbitrary. It can affect aggressiveness of response,
  * but this is probably better tuned with the coefficients and/or motor mixing matrix
  */
 #define PID_OUTPUT_RANGE_MIN -100
 #define PID_OUTPUT_RANGE_MAX 100
 /* PID coefficients
  * These are the main PID tuning settings.
  * Each tuple contains the (kp, ki, kd) coefficients, for the
  * proportional, integral, and derivative terms, respectively.
  * In general:
  *   the proportional term determine how aggressive the response is to current error
  *   the integral term corrects for long-term and steady state errors over time
  *   the derivative term dampens the over and undershoots typical of the integral term
  * For more information, please consult a PID tuning guide online
  */
 #define PID_YAW_COEFFICIENTS   ARG_LIST(10, 1, 5)
 #define PID_PITCH_COEFFICIENTS ARG_LIST(10, 1, 5)
 #define PID_ROLL_COEFFICIENTS  ARG_LIST(10, 1, 5)
 /* PID scales
  * These scales are for more convenient coefficients. The "true" coefficient
  * used is just k * scale, e.g. kp * pScale
  * The use here is instead of using a kp of 120, you can have
  * kp = 1.2 and pScale = 100. If you want, you can set these to 1 and just use
  * the coefficients on their own.
  *
  * (pScale, iScale, dScale)
  */
 #define PID_YAW_SCALE   ARG_LIST(10, 1, 10)
 #define PID_PITCH_SCALE ARG_LIST(10, 1, 10)
 #define PID_ROLL_SCALE  ARG_LIST(10, 1, 10)
 /* Lower and upper limits for integral terms
  * WARNING: large limits can and produce very large over and undershoots
  * and present a safety hazard to you and your equipment. Modify with caution
  */
 #define PID_YAW_INTEGRAL_LIMITS   ARG_LIST(-5, 5)
 #define PID_PITCH_INTEGRAL_LIMITS ARG_LIST(-5, 5)
 #define PID_ROLL_INTEGRAL_LIMITS  ARG_LIST(-5, 5)


#endif
