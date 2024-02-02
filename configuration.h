#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* System Configuration
 *
 * Configures system wide settings which are agnostic of hardware
 */

#include "src/macros.h"

#define ERROR_MESSAGE_0 "Err0: Vector math error - lengths must match"
#define ERROR_MESSAGE_1 "Err1: Vector index out of bounds"
#define ERROR_MESSAGE_2
#define ERROR_MESSAGE_3
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

#define INPUT_MOTOR_MIN 1000
#define INPUT_MOTOR_MAX 2000

// Input deadzones (snapping points)
#define INPUT_DEADZONE_WIDTH 30
#define INPUT_DEADZONE_STOPS {1000, 1500, 2000}

// Input ranges



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
#define ESC_PINS {13, 10, 11, 12} //TODO: CHECK PINS AGAINST MOTOR NUMBERS


#endif
